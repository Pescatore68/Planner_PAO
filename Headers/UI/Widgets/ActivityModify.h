#ifndef ACTIVITYMODIFY_H
#define ACTIVITYMODIFY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/AbstractActivity.h"
#include "Headers/UI/Forms/ActivityForm.h"

class ActivityModify : public QWidget {
    Q_OBJECT
private:
    QVBoxLayout* mainLayout;
    ActivityForm* activeForm;
    AbstractActivity* activity;
    QPushButton* btnSave;
    QPushButton* btnCancel;

public:
    ActivityModify(AbstractActivity* act, tagManager& tm, QWidget* parent = nullptr);
    ~ActivityModify();
signals:
    void modificationFinished();
};

#endif // ACTIVITYMODIFY_H
