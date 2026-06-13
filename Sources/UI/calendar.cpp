#include "Headers/UI/calendar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

calendar::calendar(ActivityManager& am, QWidget* parent)
    : QWidget(parent), selected(date::today()), am(am)
{
    setTopBar();
    setWidgets();
}

void calendar::setTopBar()
{
    bMonth = new QPushButton("Month", this);
    bWeek  = new QPushButton("Week", this);
    bDay   = new QPushButton("Day", this);

    auto topLayout = new QHBoxLayout();

    topLayout->addWidget(bMonth);
    topLayout->addWidget(bWeek);
    topLayout->addWidget(bDay);

    connect(bMonth, &QPushButton::clicked,
            this, &calendar::ShowMonth);

    connect(bWeek, &QPushButton::clicked,
            this, &calendar::ShowWeek);

    connect(bDay, &QPushButton::clicked,
            this, &calendar::ShowDay);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
}

void calendar::setWidgets()
{
    wStack = new QStackedWidget(this);

    wMonth = new MonthWidget(am, this);
    wWeek  = new WeekWidget(this);
    wDayContainer = new QWidget(this);
    auto dayLayout = new QHBoxLayout(wDayContainer);
    dayLayout->setContentsMargins(0, 0, 0, 0);
    dayLayout->setSpacing(8);
    wDay = new DayWidget(am, wDayContainer);
    wTaskWidget = new TaskWidget(am, wDayContainer);

    dayLayout->addWidget(wDay, 7);
    dayLayout->addWidget(wTaskWidget, 3);

    wStack->addWidget(wMonth);
    wStack->addWidget(wWeek);
    wStack->addWidget(wDayContainer);

    auto mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout) {
        mainLayout->addWidget(wStack, 1);
    }
}

void calendar::ShowMonth()
{
    wStack->setCurrentWidget(wMonth);
}

void calendar::ShowWeek() {
    wStack->setCurrentWidget(wWeek);
}

void calendar::ShowDay() {
    wDay->setDate(selected);
    //wTaskWidget->setDate(selected); //non so come meglio implementare
    wStack->setCurrentWidget(wDayContainer);
}

void calendar::onMonthDateClicked(const QDate& d)
{
    selected = date(d.day(), d.month(), d.year());
    wWeek->setWeek(selected);
    wDay->setDate(selected);
    emit dateSelected(selected);
    wStack->setCurrentWidget(wWeek);
}

void calendar::refresh() {
    wMonth->updateCalendarView();
    wDay->refresh();
    wTaskWidget->refresh();
}


