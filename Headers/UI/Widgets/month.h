#ifndef MONTHWIDGET_H
#define MONTHWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QListWidget>
#include "Headers/Model/ActivityManager.h"
#include "Headers/UI/Widgets/ActivityDelete.h"
#include "qboxlayout.h"
#include "qlabel.h"

class MonthWidget : public QWidget {
    Q_OBJECT
private:
    QCalendarWidget* calendar;
    QListWidget* activityList;
    ActivityManager& am;
    tagManager& tm;
    ActivityDelete activityDelete;

    QVBoxLayout* sideLayout;
    QLabel* titleLabel;

public:

    explicit MonthWidget(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);

    void setup();
    void updateCalendarView();
    ActivityDelete& getActivityDelete() { return activityDelete; }

    QLabel* getTitleLabel()        const { return titleLabel; }
    QListWidget* getActivityList() const { return activityList; }
    QVBoxLayout* getSideLayout()   const { return sideLayout; }
    tagManager& getTagManager()    const { return tm; }

private slots:
    void onDateChanged(const QDate& date);

signals:
    void dateClicked(const QDate& date);
    void activityUpdated();
    void dayRequested(const QDate& date);
};

#endif // MONTH_H