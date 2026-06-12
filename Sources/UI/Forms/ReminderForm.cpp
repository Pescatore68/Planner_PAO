#include "Headers/UI/Forms/ReminderForm.h"
#include "Headers/reminder.h"
#include <QMessageBox>

ReminderForm::ReminderForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent)
{
    buildCommonFields(tm);

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(false);
    dateEdit->setDisplayFormat("dd/MM/yyyy");

    timeEdit = new QTimeEdit(QTime::currentTime(), this);
    timeEdit->setDisplayFormat("HH:mm");

    locationEdit = new QLineEdit(this);
    locationEdit->setPlaceholderText("Luogo (opzionale)");

    addTimeRow("Date",  dateEdit, timeEdit);
    addRow("Location", locationEdit);

    mainLayout->addStretch();
}

bool ReminderForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campo obbligatorio", "Inserisci un nome per il reminder.");
        nameEdit->setFocus();
        return false;
    }
    return true;
}

AbstractActivity* ReminderForm::createActivity() {
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const std::string loc  = locationEdit->text().trimmed().toStdString();
    const tag* t           = tagCombo->getSelectedTag();

    const QDate qd = dateEdit->date();
    const QTime qt = timeEdit->time();
    return new Reminder(name, desc, t,
                        date(qd.day(), qd.month(), qd.year()),
                        HourMinute(qt.hour(), qt.minute()),
                        loc);
}

void ReminderForm::reset() {
    ActivityForm::reset();
    dateEdit->setDate(QDate::currentDate());
    timeEdit->setTime(QTime::currentTime());
    locationEdit->clear();
}