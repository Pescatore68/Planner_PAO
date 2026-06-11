#ifndef ROUTINEFORM_H
#define ROUTINEFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>
#include <QComboBox>

class RoutineForm : public ActivityForm {
    Q_OBJECT

public:
    explicit RoutineForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity(tagManager& tm) override;
    bool validate() override;

private:
    QComboBox* freqCombo;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QTimeEdit* startTimeEdit;
    QTimeEdit* endTimeEdit;
};

#endif // ROUTINEFORM_H