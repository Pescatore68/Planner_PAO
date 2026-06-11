#ifndef PROJECTFORM_H
#define PROJECTFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>
#include <QCheckBox>

class ProjectForm : public ActivityForm {
    Q_OBJECT

public:
    explicit ProjectForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity(tagManager& tm) override;
    bool validate() override;

private:
    QDateEdit* deadlineEdit;
    QTimeEdit* deadlineTimeEdit;
    QCheckBox* completedCheck;
};

#endif // PROJECTFORM_H
