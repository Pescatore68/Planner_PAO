#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "Headers/ActivityManager.h"
#include "Headers/date.h"
#include "Headers/tagManager.h"
#include "Headers/UI/TaskWidget.h"

#include "Headers/UI/month.h"
#include "Headers/UI/week.h"
#include "Headers/UI/DayWidget.h"


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
    WeekWidget*  wWeek;
    QWidget*     wDayContainer;
    DayWidget*   wDay;            // ← ora è un DayWidget vero
    TaskWidget* wTaskWidget;

    date selected;
    ActivityManager& am;

    void setTopBar();
    void setWidgets(tagManager& tm);

public:
    explicit calendar(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    void refresh();
    MonthWidget* getMonthWidget() const { return wMonth; }

signals:
    void dateSelected(const date& d);

private slots:
    void ShowMonth();
    void ShowWeek();
    void ShowDay();

    void onMonthDateClicked(const QDate& d);

};

#endif
