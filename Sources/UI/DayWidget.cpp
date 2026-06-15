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

ActivityBlock::ActivityBlock(AbstractActivity* a, QWidget* parent)
    : QWidget(parent), activity(a)
{
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void ActivityBlock::paintEvent(QPaintEvent*)
{
    if (!activity) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    QColor base = activity->getTag()
                      ? activity->getTag()->getColor()
                      : QColor(100, 149, 237);

    QColor bg = base;
    bg.setAlpha(hovered || selected ? 230 : 200);

    const int STRIP = 4;
    QColor strip = base.darker(130);

    const int R = 5;

    QRectF rect(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(rect, R, R);
    p.fillPath(path, bg);

    p.fillRect(QRectF(0, R, STRIP, height() - 2 * R), strip);
    QPainterPath stripPath;
    stripPath.addRoundedRect(QRectF(0, 0, STRIP, height()), R, R);
    p.fillPath(stripPath, strip);

    p.setPen(QPen(base.darker(115), 0.5));
    p.drawPath(path);

    QString textToDisplay;
    if (selected) {
        DisplayVisitor visitor;
        activity->accept(visitor);
        textToDisplay = QString::fromStdString(visitor.getSummary());
    } else {
        textToDisplay = QString::fromStdString(activity->getName());
    }

    QFont font = p.font();
    int fontSize = (height() < 24) ? 9 : (height() < 38) ? 11 : 13;
    font.setPixelSize(fontSize);
    font.setBold(!selected);
    p.setFont(font);

    double luminance = 0.299 * base.red() + 0.587 * base.green() + 0.114 * base.blue();
    p.setPen(luminance < 140 ? QColor(255,255,255,230) : QColor(30,30,30,220));

    QRectF textRect(STRIP + 6, 4, width() - STRIP - 10, height() - 8);

    if (height() < 18) {
        return;
    }

    p.drawText(textRect,
               Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
               textToDisplay);
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

    p.fillRect(rect(), QColor(255, 255, 255));

    QFont labelFont;
    labelFont.setPixelSize(11);
    p.setFont(labelFont);

    for (int h = 0; h < TOTAL_HOURS; h++) {
        int y = h * HOUR_HEIGHT;

        p.setPen(QPen(QColor(210, 210, 215), 0.8));
        p.drawLine(LABEL_WIDTH, y, W, y);

        QString label = (h == 0)
                            ? QString()
                            : QString("%1:%2")
                                  .arg(h, 2, 10, QChar(' '))
                                  .arg(0, 2, 10, QChar('0'));

        if (!label.isEmpty()) {
            p.setPen(QColor(140, 140, 148));
            QRectF labelRect(2, y - 7, LABEL_WIDTH - 6, 14);
            p.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, label);
        }

        int yHalf = y + HOUR_HEIGHT / 2;
        p.setPen(QPen(QColor(230, 230, 234), 0.5));
        p.drawLine(LABEL_WIDTH, yHalf, W, yHalf);
    }

    p.setPen(QPen(QColor(210, 210, 215), 0.8));
    p.drawLine(LABEL_WIDTH, 0, LABEL_WIDTH, height());
}

DayWidget::DayWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am), currentDate(date::today())
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    buildHeader();
    buildAllDayArea();
    buildScrollArea();

    QTimer::singleShot(0, this, [this] {
        refresh();
        int currentHour = QTime::currentTime().hour();
        int scrollTo    = std::max(0, TimeGrid::minutesToY(currentHour * 60) - 120);
        scrollArea->verticalScrollBar()->setValue(scrollTo);
    });
}

void DayWidget::buildHeader()
{
    QWidget* headerContainer = new QWidget(this);
    headerContainer->setFixedHeight(44);
    headerContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #f2f2f7;"
        "  border-bottom: 1px solid #d1d1d6;"
        "}"
        );

    QHBoxLayout* headerLayout = new QHBoxLayout(headerContainer);
    headerLayout->setContentsMargins(12, 0, 12, 0);
    headerLayout->setSpacing(8);

    QPushButton* btnPrev = new QPushButton("<", headerContainer);
    btnPrev->setFixedSize(28, 28);
    btnPrev->setCursor(Qt::PointingHandCursor);
    btnPrev->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background: transparent;"
        "  color: #007aff;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e5e5ea; border-radius: 6px; }"
        );

    headerLabel = new QLabel(headerContainer);
    headerLabel->setAlignment(Qt::AlignCenter);
    QFont f = headerLabel->font();
    f.setPixelSize(15);
    f.setBold(false);
    headerLabel->setFont(f);
    headerLabel->setStyleSheet("QLabel { color: #1c1c1e; border: none; background: transparent; }");

    QPushButton* btnNext = new QPushButton(">", headerContainer);
    btnNext->setFixedSize(28, 28);
    btnNext->setCursor(Qt::PointingHandCursor);
    btnNext->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background: transparent;"
        "  color: #007aff;"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e5e5ea; border-radius: 6px; }"
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
        "QScrollBar:vertical {"
        "  width: 6px;"
        "  background: transparent;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #c7c7cc;"
        "  border-radius: 3px;"
        "  min-height: 30px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0;"
        "}"
        );

    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: white;");

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

    allDayLayout->setContentsMargins(TimeGrid::LABEL_WIDTH + 4, 4, 4, 4);
    allDayLayout->setSpacing(2);

    allDayContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #f2f2f7;"
        "  border-bottom: 1px solid #d1d1d6;"
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

    const int gridLeft  = TimeGrid::LABEL_WIDTH + 2;
    const int gridRight = scrollArea->width() - 4;

    const int colW      = (gridRight - gridLeft) / std::max(nCols, 1);

    BlockGeometry g;
    g.top    = TimeGrid::minutesToY(startMin) + 1;
    g.height = std::max(TimeGrid::minutesToY(endMin) - g.top, 24);
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

static QList<QList<AbstractActivity*>>
groupOverlapping(const QList<QPair<AbstractActivity*, std::pair<int,int>>>& items)
{
    QList<QList<AbstractActivity*>> columns;

    for (const auto& [act, range] : items) {
        int placed = -1;

        for (int c = 0; c < columns.size(); c++) {
            AbstractActivity* last = columns[c].last();
            bool overlaps = false;
            for (const auto& [a2, r2] : items) {
                if (a2 == last) {
                    overlaps = (range.first < r2.second && r2.first < range.second);
                    break;
                }
            }
            if (!overlaps) { placed = c; break; }
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
            block->setFixedHeight(28);

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
                                 "November","Dicember"};
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

void ActivityBlock::setSelectedState(bool isSelected) {
    if (selected != isSelected) {
        selected = isSelected;
        update();
    }
}

bool ActivityBlock::isSelectedState() const { return selected; }