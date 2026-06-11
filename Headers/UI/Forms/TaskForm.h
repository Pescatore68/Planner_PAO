#ifndef TASKFORM_H
#define TASKFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>

class TaskForm : public ActivityForm {
    Q_OBJECT

public:
    explicit TaskForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity(tagManager& tm) override;
    bool validate() override;

private:
    QDateEdit* deadlineEdit;
    QTimeEdit* oDeadlineEdit;
};

#endif // TASKFORM_H
