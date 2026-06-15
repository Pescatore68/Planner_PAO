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

    // Risolve il bug di inizializzazione forzando il ridisegno immediato all'istanza
    setAutoFillBackground(false);
    update();
}

void ActivityBlock::paintEvent(QPaintEvent*)
{
    if (!activity) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Gestione Palette Pastello Dinamica - Fallback su --powder-blue (#99c1de)
    QColor base = (activity->getTag() && activity->getTag()->getColor().isValid())
                      ? activity->getTag()->getColor()
                      : QColor("#99c1de");

    QColor bg = base;
    // Opacità calibrata per l'effetto pastello morbido ma coprente
    bg.setAlpha(hovered || selected ? 245 : 200);

    const int STRIP = 5;
    QColor strip = base.darker(115);
    const int R = 10; // Angoli elegantemente stondati (Curvatura Coerente)

    QRectF rect(0, 0, width(), height());

    // Sfondo stondato del blocco attività
    QPainterPath path;
    path.addRoundedRect(rect, R, R);
    p.fillPath(path, bg);

    // Barra verticale sinistra decorativa
    QPainterPath stripPath;
    stripPath.addRoundedRect(QRectF(0, 0, STRIP, height()), R, R);
    p.fillPath(stripPath, strip);
    p.fillRect(QRectF(STRIP / 2.0, 0, STRIP / 2.0, height()), strip);

    // Contorno chiaro protettivo per separare i blocchi sovrapposti
    p.setPen(QPen(base.darker(108), 1.0));
    p.drawPath(path);

    // Estrazione del testo dell'attività
    QString textToDisplay;
    if (selected) {
        DisplayVisitor visitor;
        activity->accept(visitor);
        textToDisplay = QString::fromStdString(visitor.getSummary());
    } else {
        textToDisplay = QString::fromStdString(activity->getName());
    }

    // Forza la visibilità del testo se vuoto o all'avvio
    if (textToDisplay.isEmpty()) {
        textToDisplay = "Unnamed Activity";
    }

    QFont font = p.font();
    font.setFamily("Segoe UI");
    int fontSize = (height() < 24) ? 10 : (height() < 38) ? 11 : 13;
    font.setPixelSize(fontSize);
    font.setBold(true);
    p.setFont(font);

    // Calcolo del contrasto dinamico per sfondo chiaro/scuro
    double luminance = 0.299 * base.red() + 0.587 * base.green() + 0.114 * base.blue();
    if (luminance < 145) {
        p.setPen(QColor("#fff1e6")); // --linen (chiaro) se il tag è scuro
    } else {
        p.setPen(QColor("#4A3E4D")); // Vinaccia scuro ad alto contrasto per i pastello
    }

    QRectF textRect(STRIP + 10, 4, width() - STRIP - 14, height() - 8);

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


// ── TIME GRID (Griglia Oraria di Sfondo) ───────────────────────────────────

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

    // Sfondo della griglia bianca pulita (fitta con il panna esterno)
    p.fillRect(rect(), QColor("#FFFFFF"));

    QFont labelFont("Segoe UI");
    labelFont.setPixelSize(11);
    labelFont.setBold(true);
    p.setFont(labelFont);

    for (int h = 0; h < TOTAL_HOURS; h++) {
        int y = h * HOUR_HEIGHT;

        // Linee delle ore piene: Sottili e pulite
        p.setPen(QPen(QColor("#f0efeb"), 1.2, Qt::SolidLine)); // --parchment
        p.drawLine(LABEL_WIDTH, y, W, y);

        QString label = (h == 0) ? QString() : QString("%1:00").arg(h, 2, 10, QChar('0'));

        if (!label.isEmpty()) {
            p.setPen(QColor("#7D6B7F")); // Testo orari perfettamente leggibile
            QRectF labelRect(4, y - 7, LABEL_WIDTH - 12, 14);
            p.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
        }

        // Linee delle mezz'ore: Tratteggiate chiarissime
        int yHalf = y + HOUR_HEIGHT / 2;
        p.setPen(QPen(QColor("#fff1e6"), 1.0, Qt::DashLine)); // --linen
        p.drawLine(LABEL_WIDTH, yHalf, W, yHalf);
    }

    // Linea verticale principale dell'asse orario
    p.setPen(QPen(QColor("#eddcd2"), 1.5)); // --powder-petal
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

    // Risolve il bug di mancato rendering iniziale forzando la sequenza di caricamento
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
    headerContainer->setFixedHeight(52);
    headerContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #eddcd2;" // --powder-petal
        "  border-bottom: 2px solid #fff1e6;" // --linen
        "}"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerContainer);
    headerLayout->setContentsMargins(16, 0, 16, 0);

    QPushButton* btnPrev = new QPushButton("<", headerContainer);
    btnPrev->setFixedSize(32, 32);
    btnPrev->setCursor(Qt::PointingHandCursor);
    btnPrev->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #4A3E4D;"
        "  background: #fff1e6;"
        "  color: #4A3E4D;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  border-radius: 16px;"
        "}"
        "QPushButton:hover { background-color: #4A3E4D; color: #fff1e6; }"
        );

    headerLabel = new QLabel(headerContainer);
    headerLabel->setAlignment(Qt::AlignCenter);
    QFont f("Segoe UI");
    f.setPixelSize(15);
    f.setBold(true);
    headerLabel->setFont(f);
    headerLabel->setStyleSheet("QLabel { color: #4A3E4D; border: none; background: transparent; }");

    QPushButton* btnNext = new QPushButton(">", headerContainer);
    btnNext->setFixedSize(32, 32);
    btnNext->setCursor(Qt::PointingHandCursor);
    btnNext->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #4A3E4D;"
        "  background: #fff1e6;"
        "  color: #4A3E4D;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  border-radius: 16px;"
        "}"
        "QPushButton:hover { background-color: #4A3E4D; color: #fff1e6; }"
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
        "QScrollBar::handle:vertical { background: #eddcd2; border-radius: 4px; }"
        "QScrollBar::handle:vertical:hover { background: #99c1de; }"
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
    allDayLayout->setContentsMargins(TimeGrid::LABEL_WIDTH + 8, 8, 8, 8);
    allDayLayout->setSpacing(6);

    allDayContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #f0efeb;" // --parchment
        "  border-bottom: 2px solid #eddcd2;"
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
    g.height = std::max(TimeGrid::minutesToY(endMin) - g.top, 28);
    g.left   = gridLeft + colIdx * colW + 3;
    g.width  = colW - 6;
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
            block->setFixedHeight(32);

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