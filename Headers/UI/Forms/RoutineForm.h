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

    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;

    QDateEdit* getStartDateEdit() const { return startDateEdit; }
    QDateEdit* getEndDateEdit()   const { return endDateEdit; }
    QTimeEdit* getStartTimeEdit() const { return startTimeEdit; }
    QTimeEdit* getEndTimeEdit()   const { return endTimeEdit; }
    QComboBox* getFreqCombo()     const { return freqCombo; }

    void loadFromActivity(AbstractActivity* act) override;
    void saveToActivity(AbstractActivity* act) override;


private:
    QComboBox* freqCombo;
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QTimeEdit* startTimeEdit;
    QTimeEdit* endTimeEdit;
};

#endif // ROUTINEFORM_H