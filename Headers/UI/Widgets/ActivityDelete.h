#ifndef ACTIVITYDELETE_H
#define ACTIVITYDELETE_H

#include <QObject>
#include <QWidget>
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/AbstractActivity.h"

class ActivityDelete : public QObject{
    Q_OBJECT
private:
    ActivityManager& am;

public:
    explicit ActivityDelete(ActivityManager& activityMgr, QObject* parent = nullptr);

    bool execute(AbstractActivity* act, QWidget* parent);

signals:

    void activityDeleted();
};

#endif // ACTIVITYDELETE_H