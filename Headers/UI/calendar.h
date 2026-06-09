#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "Headers/ActivityManager.h"
#include "Headers/date.h"
#include "Headers/tagManager.h"

#include "Headers/UI/month.h"
#include "Headers/UI/week.h"

class calendar : public QWidget {
    Q_OBJECT

private:
    // buttons
    QPushButton* bMonth;
    QPushButton* bWeek;
    QPushButton* bDay;

    // stack
    QStackedWidget* wStack;

    // views
    MonthWidget* wMonth;
    WeekWidget* wWeek;
    QWidget* wDay;

    date selected;

    void setTopBar();
    void setWidgets();

public:
    calendar(QWidget* parent = nullptr);

signals:
    void dateSelected(const date& d);

private slots:
    void ShowMonth();
    void ShowWeek();
    void ShowDay();

    void onMonthDateClicked(const QDate& d);
};

#endif
