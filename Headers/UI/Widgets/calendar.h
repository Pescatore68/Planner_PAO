#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/date.h"
#include "Headers/Model/tagManager.h"
#include "Headers/UI/Widgets/TaskWidget.h"
#include "Headers/UI/Widgets/month.h"
#include "Headers/UI/Widgets/DayWidget.h"

class calendar : public QWidget {
    Q_OBJECT

private:
    QPushButton*    bMonth;

    QPushButton*    bDay;
    QStackedWidget* wStack;
    MonthWidget*    wMonth;

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
    void ShowDay();
    void onMonthDateClicked(const QDate& d);

public slots:
    void DayRequested(const QDate& date);

};


#endif
