#include "Headers/UI/calendar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

calendar::calendar(ActivityManager& am, QWidget* parent)
    : QWidget(parent), selected(1, 1, 2026), am(am)
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
    wDay   = new QWidget(this); // placeholder

    wStack->addWidget(wMonth);
    wStack->addWidget(wWeek);
    wStack->addWidget(wDay);

    layout()->addWidget(wStack);
}
void calendar::ShowMonth()
{
    wStack->setCurrentWidget(wMonth);
}

void calendar::ShowWeek()
{
    wStack->setCurrentWidget(wWeek);
}

void calendar::ShowDay()
{
    wStack->setCurrentWidget(wDay);
}

void calendar::onMonthDateClicked(const QDate& d)
{
    selected = date(d.day(), d.month(), d.year());

    // aggiorna week automaticamente
    wWeek->setWeek(selected);

    emit dateSelected(selected);

    wStack->setCurrentWidget(wWeek);
}

void calendar::refresh() {
    wMonth->updateCalendarView();
}
