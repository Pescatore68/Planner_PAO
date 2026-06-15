#include "Headers/UI/DayWidget.h"
#include "Headers/AbstractActivity.h"
#include "Headers/ActivityManager.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/HourMinute.h"
#include "Headers/Visitor/DayWidgetVisitor.h"

#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QEnterEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <cmath>
#include <algorithm>
#include <QPushButton>

// ── ACTIVITY BLOCK (Elemento Grafico per le Attività) ──────────────────────

ActivityBlock::ActivityBlock(AbstractActivity* a, QWidget* parent)
    : QWidget(parent), activity(a), hovered(false), selected(false)
{
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAutoFillBackground(false);
    update();
}

void ActivityBlock::paintEvent(QPaintEvent*)
{
    if (!activity) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Colore base dal tag. Se non valido, fallback sul nostro --steel-blue (#6096ba)
    QColor base = (activity->getTag() && activity->getTag()->getColor().isValid())
                      ? activity->getTag()->getColor()
                      : QColor("#6096ba");

    QColor bg = base;
    // Tonalità piena e solida, opacità elevata per un look professionale senza trasparenze eccessive
    bg.setAlpha(hovered || selected ? 255 : 230);

    const int STRIP = 4;
    QColor strip = base.darker(125); // Accento laterale più scuro e serio
    const int R = 4; // Angoli squadrati e professionali (ridotti a 4px per uno stile geometrico)

    QRectF rect(0, 0, width(), height());

    // Corpo del blocco attività
    QPainterPath path;
    path.addRoundedRect(rect, R, R);
    p.fillPath(path, bg);

    // Striscia di accento verticale netta
    QPainterPath stripPath;
    stripPath.addRoundedRect(QRectF(0, 0, STRIP, height()), R, R);
    p.fillPath(stripPath, strip);
    p.fillRect(QRectF(STRIP / 2.0, 0, STRIP / 2.0, height()), strip);

    // Bordi di separazione netti ad alto contrasto
    p.setPen(QPen(base.darker(115), 1.0));
    p.drawPath(path);

    QString textToDisplay;
    if (selected) {
        DisplayVisitor visitor;
        activity->accept(visitor);
        textToDisplay = QString::fromStdString(visitor.getSummary());
    } else {
        textToDisplay = QString::fromStdString(activity->getName());
    }

    if (textToDisplay.isEmpty()) {
        textToDisplay = "---";
    }

    QFont font = p.font();
    font.setFamily("Helvetica");
    int fontSize = (height() < 24) ? 10 : (height() < 38) ? 11 : 12;
    font.setPixelSize(fontSize);
    font.setBold(true);
    p.setFont(font);

    // Calcolo contrasto rigoroso per interfacce business
    double luminance = 0.299 * base.red() + 0.587 * base.green() + 0.114 * base.blue();
    if (luminance < 135) {
        p.setPen(QColor("#e7ecef")); // Testo --platinum su tag scuri
    } else {
        p.setPen(QColor("#274c77")); // Testo --dusk-blue su tag chiari
    }

    QRectF textRect(STRIP + 8, 4, width() - STRIP - 12, height() - 8);

    if (height() < 16) return;
    p.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, textToDisplay);
}

void ActivityBlock::enterEvent(QEnterEvent* e)
{
    hovered = true;
    update();
    QWidget::enterEvent(e);
}

void ActivityBlock::leaveEvent(QEvent* e)
{
    hovered = false;
    update();
    QWidget::leaveEvent(e);
}

void ActivityBlock::setSelectedState(bool isSelected) {
    if (selected != isSelected) {
        selected = isSelected;
        update();
    }
}

bool ActivityBlock::isSelectedState() const { return selected; }


// ── TIME GRID (Griglia Oraria Rigorosa) ───────────────────────────────────

TimeGrid::TimeGrid(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(TOTAL_HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

int TimeGrid::minutesToY(int totalMinutes)
{
    return (totalMinutes * HOUR_HEIGHT) / 60;
}

void TimeGrid::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    const int W = width();

    // Sfondo della griglia: Bianco tecnico pulito
    p.fillRect(rect(), QColor("#FFFFFF"));

    QFont labelFont("Helvetica");
    labelFont.setPixelSize(11);
    labelFont.setBold(true);
    p.setFont(labelFont);

    for (int h = 0; h < TOTAL_HOURS; h++) {
        int y = h * HOUR_HEIGHT;

        // Linee ore piene: Grigio tecnico pulito e sottile
        p.setPen(QPen(QColor("#e7ecef"), 1.0, Qt::SolidLine)); // --platinum usato come linea
        p.drawLine(LABEL_WIDTH, y, W, y);

        QString label = (h == 0) ? QString() : QString("%1:00").arg(h, 2, 10, QChar('0'));

        if (!label.isEmpty()) {
            p.setPen(QColor("#8b8c89")); // Orari scritti in --grey-olive nitido
            QRectF labelRect(4, y - 7, LABEL_WIDTH - 12, 14);
            p.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
        }

        // Linee di mezz'ora: Tratteggio minimale grigio finissimo
        int yHalf = y + HOUR_HEIGHT / 2;
        p.setPen(QPen(QColor("#f4f6f8"), 1.0, Qt::DashLine));
        p.drawLine(LABEL_WIDTH, yHalf, W, yHalf);
    }

    // Linea verticale divisoria della timeline (In --dusk-blue istituzionale)
    p.setPen(QPen(QColor("#274c77"), 1.5));
    p.drawLine(LABEL_WIDTH, 0, LABEL_WIDTH, height());
}


// ── DAY WIDGET (Pannello Giornaliero Principale) ───────────────────────────

DayWidget::DayWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am), currentDate(date::today())
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    buildHeader();
    buildAllDayArea();
    buildScrollArea();

    QTimer::singleShot(50, this, [this] {
        refresh();
        int currentHour = QTime::currentTime().hour();
        int scrollTo    = std::max(0, TimeGrid::minutesToY(currentHour * 60) - 120);
        scrollArea->verticalScrollBar()->setValue(scrollTo);
    });
}

void DayWidget::buildHeader()
{
    QWidget* headerContainer = new QWidget(this);
    headerContainer->setFixedHeight(48);
    headerContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #274c77;" // --dusk-blue (testata scura, formale)
        "  border-bottom: 2px solid #6096ba;" // --steel-blue
        "}"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerContainer);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    QPushButton* btnPrev = new QPushButton("<", headerContainer);
    btnPrev->setFixedSize(28, 28);
    btnPrev->setCursor(Qt::PointingHandCursor);
    btnPrev->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #6096ba;"
        "  background: #274c77;"
        "  color: #e7ecef;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "  border-radius: 4px;" /* Squadrato, stile gestionale serio */
        "}"
        "QPushButton:hover { background-color: #6096ba; }"
        );

    headerLabel = new QLabel(headerContainer);
    headerLabel->setAlignment(Qt::AlignCenter);
    QFont f("Helvetica");
    f.setPixelSize(14);
    f.setBold(true);
    headerLabel->setFont(f);
    headerLabel->setStyleSheet("QLabel { color: #e7ecef; border: none; background: transparent; }");

    QPushButton* btnNext = new QPushButton(">", headerContainer);
    btnNext->setFixedSize(28, 28);
    btnNext->setCursor(Qt::PointingHandCursor);
    btnNext->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #6096ba;"
        "  background: #274c77;"
        "  color: #e7ecef;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover { background-color: #6096ba; }"
        );

    headerLayout->addWidget(btnPrev);
    headerLayout->addWidget(headerLabel, 1);
    headerLayout->addWidget(btnNext);

    connect(btnPrev, &QPushButton::clicked, this, [this]() {
        QDate qd(currentDate.getYear(), currentDate.getMonth(), currentDate.getDay());
        qd = qd.addDays(-1);
        setDate(date(qd.day(), qd.month(), qd.year()));
    });

    connect(btnNext, &QPushButton::clicked, this, [this]() {
        QDate qd(currentDate.getYear(), currentDate.getMonth(), currentDate.getDay());
        qd = qd.addDays(1);
        setDate(date(qd.day(), qd.month(), qd.year()));
    });

    mainLayout->addWidget(headerContainer);
}

void DayWidget::buildScrollArea()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #FFFFFF; border: none; }"
        "QScrollBar:vertical { width: 8px; background: transparent; }"
        "QScrollBar::handle:vertical { background: #6096ba; border-radius: 4px; }"
        "QScrollBar::handle:vertical:hover { background: #6096ba; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        );

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: #FFFFFF;");

    QVBoxLayout* scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->setContentsMargins(0, 0, 0, 0);
    scrollLayout->setSpacing(0);

    timeGrid = new TimeGrid(scrollContent);
    scrollLayout->addWidget(timeGrid);

    mainLayout->addWidget(scrollArea, 1);
    scrollArea->setWidget(scrollContent);
}

void DayWidget::buildAllDayArea()
{
    allDayContainer = new QWidget(this);
    allDayLayout = new QVBoxLayout(allDayContainer);
    allDayLayout->setContentsMargins(TimeGrid::LABEL_WIDTH + 8, 6, 8, 6);
    allDayLayout->setSpacing(4);

    allDayContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #e7ecef;" // --platinum per la barra scadenze fisse
        "  border-bottom: 2px solid #6096ba;"
        "}"
        );

    allDayContainer->setVisible(false);
    mainLayout->addWidget(allDayContainer);
}

static std::pair<int,int> extractMinutes(AbstractActivity* a, const date& d)
{
    if (!a) return {0, 30};
    MinutesExtractorVisitor visitor(d);
    a->accept(visitor);
    return {visitor.startMinutes, visitor.endMinutes};
}

DayWidget::BlockGeometry
DayWidget::computeGeometry(AbstractActivity* a, int colIdx, int nCols) const
{
    auto [startMin, endMin] = extractMinutes(a, currentDate);

    const int gridLeft  = TimeGrid::LABEL_WIDTH + 4;
    const int gridRight = scrollArea->width() - 8;
    const int colW      = (gridRight - gridLeft) / std::max(nCols, 1);

    BlockGeometry g;
    g.top    = TimeGrid::minutesToY(startMin) + 2;
    g.height = std::max(TimeGrid::minutesToY(endMin) - g.top, 26);
    g.left   = gridLeft + colIdx * colW + 2;
    g.width  = colW - 4;
    return g;
}

bool DayWidget::shouldShow(AbstractActivity* a) const
{
    if (!a) return false;
    TypeCheckVisitor visitor;
    a->accept(visitor);
    return visitor.valid;
}

static bool isAllDayActivity(AbstractActivity* a)
{
    if (!a) return false;
    AllDayCheckVisitor visitor;
    a->accept(visitor);
    return visitor.isAllDay;
}

void DayWidget::clearBlocks()
{
    for (ActivityBlock* b : blocks) {
        b->removeEventFilter(this);
        b->deleteLater();
    }
    blocks.clear();
}

static bool overlaps(int s1, int e1, int s2, int e2) {
    return s1 < e2 && s2 < e1;
}

static QList<QList<AbstractActivity*>>
groupOverlapping(const QList<QPair<AbstractActivity*, std::pair<int,int>>>& items)
{
    QList<QList<AbstractActivity*>> columns;

    for (const auto& [act, range] : items) {
        int placed = -1;

        for (int c = 0; c < columns.size(); c++) {
            AbstractActivity* last = columns[c].last();
            bool hasOverlap = false;

            for (const auto& [a2, r2] : items) {
                if (a2 == last) {
                    hasOverlap = overlaps(range.first, range.second, r2.first, r2.second);
                    break;
                }
            }
            if (!hasOverlap) { placed = c; break; }
        }

        if (placed == -1) {
            columns.append({act});
        } else {
            columns[placed].append(act);
        }
    }
    return columns;
}

void DayWidget::populateBlocks()
{
    clearBlocks();

    QLayoutItem* item;
    while ((item = allDayLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->removeEventFilter(this);
            item->widget()->deleteLater();
        }
        delete item;
    }

    auto activitiesOnDate = am.getOnDate(currentDate);

    QList<AbstractActivity*> allDayItems;
    QList<QPair<AbstractActivity*, std::pair<int,int>>> timedItems;

    for (AbstractActivity* a : activitiesOnDate) {
        if (!shouldShow(a)) continue;

        if (isAllDayActivity(a)) {
            allDayItems.append(a);
        } else {
            timedItems.append({a, extractMinutes(a, currentDate)});
        }
    }

    if (!allDayItems.isEmpty()) {
        allDayContainer->setVisible(true);
        for (AbstractActivity* act : allDayItems) {
            auto* block = new ActivityBlock(act, allDayContainer);
            block->setFixedHeight(30);

            block->installEventFilter(this);
            block->setProperty("activityPtr", QVariant::fromValue(static_cast<void*>(act)));

            allDayLayout->addWidget(block);
            blocks.append(block);
        }
    } else {
        allDayContainer->setVisible(false);
    }

    if (timedItems.isEmpty()) {
        timeGrid->lower();
        timeGrid->update();
        return;
    }

    auto columns = groupOverlapping(timedItems);
    int nCols = columns.size();

    QMap<AbstractActivity*, int> colMap;
    for (int c = 0; c < nCols; c++) {
        for (AbstractActivity* a : columns[c]) {
            colMap[a] = c;
        }
    }

    for (const auto& [act, range] : timedItems) {
        int colIdx = colMap.value(act, 0);
        auto geo = computeGeometry(act, colIdx, nCols);

        auto* block = new ActivityBlock(act, scrollContent);
        block->setGeometry(geo.left, geo.top, geo.width, geo.height);

        block->installEventFilter(this);
        block->setProperty("activityPtr", QVariant::fromValue(static_cast<void*>(act)));

        block->raise();
        block->show();

        blocks.append(block);
    }

    timeGrid->lower();
    timeGrid->update();
}

bool DayWidget::eventFilter(QObject* obj, QEvent* event)
{
    auto* block = qobject_cast<ActivityBlock*>(obj);
    if (!block) return QWidget::eventFilter(obj, event);

    void* ptr = block->property("activityPtr").value<void*>();
    if (!ptr) return QWidget::eventFilter(obj, event);

    AbstractActivity* act = static_cast<AbstractActivity*>(ptr);

    if (event->type() == QEvent::MouseButtonDblClick) {
        emit activityDoubleClicked(act);
        return true;
    }

    if (event->type() == QEvent::MouseButtonPress) {
        bool nextState = !block->isSelectedState();

        for (ActivityBlock* b : blocks) {
            if (b == block) {
                b->setSelectedState(nextState);
            } else {
                b->setSelectedState(false);
            }
        }

        emit activityClicked(act);
        return true;
    }

    return QWidget::eventFilter(obj, event);
}

void DayWidget::setDate(const date& d)
{
    currentDate = d;

    static const char* giorni[]  = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    static const char* mesi[]    = {"","January","February","March","April","May",
                                 "June","July","August","September","October",
                                 "November","December"};
    QDate qd(d.getYear(), d.getMonth(), d.getDay());
    QString dayName = giorni[qd.dayOfWeek() % 7];
    QString dateStr = QString("%1 %2 %3 %4")
                          .arg(dayName)
                          .arg(d.getDay())
                          .arg(mesi[d.getMonth()])
                          .arg(d.getYear());
    headerLabel->setText(dateStr);

    refresh();
}

void DayWidget::refresh()
{
    scrollContent->setFixedHeight(TimeGrid::TOTAL_HEIGHT);
    timeGrid->setFixedHeight(TimeGrid::TOTAL_HEIGHT);

    populateBlocks();
}

void DayWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    populateBlocks();
}