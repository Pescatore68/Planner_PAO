#ifndef MONTHWIDGET_H
#define MONTHWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QListWidget>
#include "Headers/ActivityManager.h"
#include "Headers/UI/ActivityDelete.h"

class MonthWidget : public QWidget {
    Q_OBJECT
private:
    QCalendarWidget* calendar;
    QListWidget* activityList;
    ActivityManager& am;
    ActivityDelete activityDelete;

public:

    explicit MonthWidget(ActivityManager& am, QWidget* parent = nullptr);

    void setup();
    void updateCalendarView();
    ActivityDelete& getActivityDelete() { return activityDelete; }

private slots:
    void onDateChanged(const QDate& date);

signals:
    void dateClicked(const QDate& date);
};

#endif // MONTH_H