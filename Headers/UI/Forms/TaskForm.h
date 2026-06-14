#ifndef TASKFORM_H
#define TASKFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>

class TaskForm : public ActivityForm {
    Q_OBJECT

public:
    explicit TaskForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;
    QDate getDeadlineDate() const;
    QDateEdit* getDeadlineEdit() const;
    QTime getODeadlineTime() const;
    QTimeEdit* getODeadlineEdit() const;

    void loadFromActivity(AbstractActivity* act) override;
    void saveToActivity(AbstractActivity* act) override;

private:
    QDateEdit* deadlineEdit;
    QTimeEdit* oDeadlineEdit;
};

#endif // TASKFORM_H
