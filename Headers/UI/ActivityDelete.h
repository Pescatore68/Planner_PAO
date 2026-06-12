#ifndef ACTIVITYDELETE_H
#define ACTIVITYDELETE_H

#include <QObject>
#include <QWidget>
#include "Headers/ActivityManager.h"
#include "Headers/AbstractActivity.h"

class ActivityDelete : public QObject{
    Q_OBJECT
private:
    ActivityManager& am;

public:
    // Costruttore
    explicit ActivityDelete(ActivityManager& activityMgr, QObject* parent = nullptr);

    bool execute(AbstractActivity* act, QWidget* parent);

signals:

    void activityDeleted();
};

#endif // ACTIVITYDELETE_H