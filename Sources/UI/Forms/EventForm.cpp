#include "Headers/UI/Forms/EventForm.h"
#include "Headers/event.h"
#include <QMessageBox>

EventForm::EventForm(tagManager& tm, QWidget* parent)
    :  ActivityForm(tm, parent)
{

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");

    endDateEdit = new QDateEdit(QDate::currentDate(), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");

    allDayCheck = new QCheckBox("All Day", this);

    startTimeEdit = new QTimeEdit(QTime::currentTime(), this);
    startTimeEdit->setDisplayFormat("HH:mm");

    endTimeEdit = new QTimeEdit(QTime::currentTime().addSecs(3600), this);
    endTimeEdit->setDisplayFormat("HH:mm");

    locationEdit = new QLineEdit(this);
    locationEdit->setPlaceholderText("Location (optional)");
    addRow(locationEdit);
    addTimeRow("Starts",  startDateEdit, startTimeEdit);
    addTimeRow("Ends",    endDateEdit, endTimeEdit);
    addRow(allDayCheck);


    mainLayout->addStretch();

    connect(allDayCheck, &QCheckBox::toggled, this, &EventForm::onAllDayToggled);
}

void EventForm::onAllDayToggled(bool checked) {
    startTimeEdit->setEnabled(!checked);
    endTimeEdit->setEnabled(!checked);
}

bool EventForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Required field", "Please enter a name for the event.");
        nameEdit->setFocus();
        return false;
    }
    if (endDateEdit->date() < startDateEdit->date()) {
        QMessageBox::warning(this, "Invalid date",
                             "End date cannot exceed start date.");
        endDateEdit->setFocus();
        return false;
    }

    if (endDateEdit->date() == startDateEdit->date()) {
        if (endTimeEdit->time() < startTimeEdit->time()) {
            QMessageBox::warning(this, "Invalid Time Range",
                                 "End time cannot exceed start time.");
            endTimeEdit->setFocus();
            return false;
        }
    }
    return true;
}

AbstractActivity* EventForm::createActivity() {
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

void EventForm::reset() {
    ActivityForm::reset();
    startDateEdit->setDate(QDate::currentDate());
    endDateEdit->setDate(QDate::currentDate());
    startTimeEdit->setTime(QTime::currentTime());
    endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
    allDayCheck->setChecked(false);
    locationEdit->clear();
}