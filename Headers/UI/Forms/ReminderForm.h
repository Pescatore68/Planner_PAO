#ifndef REMINDERFORM_H
#define REMINDERFORM_H

#include "Headers/UI/Forms/ActivityForm.h"
#include <QDateEdit>
#include <QTimeEdit>

class ReminderForm : public ActivityForm {
    Q_OBJECT

public:
    explicit ReminderForm(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;

    QDateEdit* getDateEdit()     const { return dateEdit; }
    QTimeEdit* getTimeEdit()     const { return timeEdit; }
    QLineEdit* getLocationEdit() const { return locationEdit; }

    void loadFromActivity(AbstractActivity* act) override;
    void saveToActivity(AbstractActivity* act) override;


private:
    QDateEdit* dateEdit;
    QTimeEdit* timeEdit;
    QLineEdit* locationEdit;
};

#endif // REMINDERFORM_H
