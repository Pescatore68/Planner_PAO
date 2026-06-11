#ifndef MONTHWIDGET_H
#define MONTHWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QListWidget>
#include "Headers/ActivityManager.h"

class MonthWidget : public QWidget {
    Q_OBJECT
private:
    QCalendarWidget* calendar;
    QListWidget* activityList;
    ActivityManager& am;

public:

    explicit MonthWidget(ActivityManager& am, QWidget* parent = nullptr);

    void setup();
    void updateCalendarView();

private slots:
    void onDateChanged(const QDate& date);

signals:
    void dateClicked(const QDate& date);
};

#endif // MONTH_H