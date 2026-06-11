#include "Headers/UI/Forms/EventForm.h"
#include "Headers/event.h"
#include <QMessageBox>

EventForm::EventForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent)
{
    buildCommonFields(tm);

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");

    endDateEdit = new QDateEdit(QDate::currentDate(), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");

    allDayCheck = new QCheckBox("All Day", this);

    startTimeEdit = new QTimeEdit(QTime(9, 0), this);
    startTimeEdit->setDisplayFormat("HH:mm");

    endTimeEdit = new QTimeEdit(QTime(10, 0), this);
    endTimeEdit->setDisplayFormat("HH:mm");

    locationEdit = new QLineEdit(this);
    locationEdit->setPlaceholderText("Location (optional)");
    addTimeRow("Starts",  startDateEdit, startTimeEdit);
    addTimeRow("Ends",    endDateEdit, endTimeEdit);
    addRow("", allDayCheck);
    addRow("Location", locationEdit);

    mainLayout->addStretch();

    connect(allDayCheck, &QCheckBox::toggled, this, &EventForm::onAllDayToggled);
}

void EventForm::onAllDayToggled(bool checked) {
    startTimeEdit->setEnabled(!checked);
    endTimeEdit->setEnabled(!checked);
}

bool EventForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campo obbligatorio", "Inserisci un nome per l'evento.");
        nameEdit->setFocus();
        return false;
    }
    if (endDateEdit->date() < startDateEdit->date()) {
        QMessageBox::warning(this, "Date non valide",
                             "La data di fine deve essere uguale o successiva a quella di inizio.");
        endDateEdit->setFocus();
        return false;
    }
    return true;
}

AbstractActivity* EventForm::createActivity(tagManager& tm) {
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const std::string loc  = locationEdit->text().trimmed().toStdString();
    const tag* t           = tagCombo->getSelectedTag();

    const QDate qsd = startDateEdit->date();
    const QDate qed = endDateEdit->date();
    date sd(qsd.day(), qsd.month(), qsd.year());
    date ed(qed.day(), qed.month(), qed.year());

    if (allDayCheck->isChecked())
        return new Event(name, desc, t, sd, ed, loc);

    const QTime qst = startTimeEdit->time();
    const QTime qet = endTimeEdit->time();
    return new Event(name, desc, t, sd, ed,
                     HourMinute(qst.hour(), qst.minute()),
                     HourMinute(qet.hour(), qet.minute()),
                     loc);
}