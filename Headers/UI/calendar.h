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
    QPushButton*    bMonth;
    QPushButton*    bWeek;
    QPushButton*    bDay;
    QStackedWidget* wStack;
    MonthWidget*    wMonth;
    WeekWidget*     wWeek;
    QWidget*        wDayContainer;
    DayWidget*      wDay;
    TaskWidget*     wTaskWidget;
    date            selected;
    ActivityManager& am;

    void setTopBar();
    void setWidgets(tagManager& tm);
    void setupConnections();

public:
    explicit calendar(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    void refresh();
    MonthWidget* getMonthWidget() const { return wMonth; }
    DayWidget*   getDayWidget()   const { return wDay; }
    TaskWidget*  getTaskWidget()  const { return wTaskWidget; }

signals:
    void dateSelected(const date& d);
    void activityModifyRequested(AbstractActivity* a);

private slots:
    void ShowMonth();
    void ShowWeek();
    void ShowDay();
    void onMonthDateClicked(const QDate& d);
};

#endif
