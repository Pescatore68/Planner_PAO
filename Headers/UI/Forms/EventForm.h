#ifndef EVENTFORM_H
#define EVENTFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>
#include <QCheckBox>

class EventForm : public ActivityForm {
    Q_OBJECT

public:
    explicit EventForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;

    QDateEdit* getStartDateEdit() const { return startDateEdit; }
    QDateEdit* getEndDateEdit()   const { return endDateEdit; }
    QTimeEdit* getStartTimeEdit() const { return startTimeEdit; }
    QTimeEdit* getEndTimeEdit()   const { return endTimeEdit; }
    QLineEdit* getLocationEdit()   const { return locationEdit; }
    QCheckBox* getAllDayCheck()   const { return allDayCheck; }

    void loadFromActivity(AbstractActivity* act) override;
    void saveToActivity(AbstractActivity* act) override;


private slots:
    void onAllDayToggled(bool checked);

private:
    QDateEdit* startDateEdit;
    QDateEdit* endDateEdit;
    QTimeEdit* startTimeEdit;
    QTimeEdit* endTimeEdit;
    QCheckBox* allDayCheck;
    QLineEdit* locationEdit;
};

#endif // EVENTFORM_H