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