#include "Headers/UI/Widgets/calendar.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

calendar::calendar(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QWidget(parent), selected(date::today()), am(am)
{
    setTopBar();
    setWidgets(tm);
}

void calendar::setTopBar()
{
    bMonth = new QPushButton("Month", this);
    bDay   = new QPushButton("Day",   this);

    auto topLayout = new QHBoxLayout();
    topLayout->addWidget(bMonth);
    topLayout->addWidget(bDay);

    connect(bMonth, &QPushButton::clicked, this, &calendar::ShowMonth);
    connect(bDay,   &QPushButton::clicked, this, &calendar::ShowDay);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
}

void calendar::setWidgets(tagManager& tm)
{
    wStack = new QStackedWidget(this);

    wMonth = new MonthWidget(am, tm, this);
    connect(wMonth, &MonthWidget::dayRequested, this, &calendar::DayRequested);

    wDayContainer = new QWidget(this);
    auto dayLayout = new QHBoxLayout(wDayContainer);
    dayLayout->setContentsMargins(0, 0, 0, 0);
    dayLayout->setSpacing(8);

    wDay        = new DayWidget(am, wDayContainer);
    wTaskWidget = new TaskWidget(am, wDayContainer);

    dayLayout->addWidget(wDay,        7);
    dayLayout->addWidget(wTaskWidget, 3);

    wStack->addWidget(wMonth);
    wStack->addWidget(wDayContainer);

    auto mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (mainLayout)
        mainLayout->addWidget(wStack, 1);

    setupConnections();
}

void calendar::setupConnections()
{
    connect(wDay, &DayWidget::activityClicked,
            wTaskWidget, &TaskWidget::handleExternalActivitySelected);

    connect(wDay, &DayWidget::activityDoubleClicked,
            this, &calendar::activityModifyRequested);

    connect(wTaskWidget, &TaskWidget::activityDoubleClicked,
            this, &calendar::activityModifyRequested);

    connect(wTaskWidget, &TaskWidget::deleteRequested,
            this, &calendar::refresh);
}

void calendar::ShowMonth() { wStack->setCurrentWidget(wMonth); }
void calendar::ShowDay()
{
    wDay->setDate(selected);
    wStack->setCurrentWidget(wDayContainer);
}

void calendar::onMonthDateClicked(const QDate& d)
{
    selected = date(d.day(), d.month(), d.year());
    wDay->setDate(selected);
    emit dateSelected(selected);
}

void calendar::DayRequested(const QDate& d)
{
    // Aggiorna la data selezionata nel calendario principale
    selected = date(d.day(), d.month(), d.year());

    // Passa alla vista giorno
    ShowDay();
}

void calendar::refresh()
{
    wMonth->updateCalendarView();
    wDay->refresh();
    wTaskWidget->refresh();
}

