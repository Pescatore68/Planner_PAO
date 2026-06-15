#include "Headers/UI/Forms/EventForm.h"
#include "Headers/Model/event.h"
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

void EventForm::loadFromActivity(AbstractActivity* act) {
    auto* e = dynamic_cast<Event*>(act);
    if (!e) return;
    fillCommonFields(e);
    getStartDateEdit()->setDate(QDate(e->getStartDate().getYear(), e->getStartDate().getMonth(), e->getStartDate().getDay()));
    getEndDateEdit()->setDate(QDate(e->getEndDate().getYear(), e->getEndDate().getMonth(), e->getEndDate().getDay()));

    bool allDay = !e->hasTime();
    getAllDayCheck()->setChecked(allDay);      // ← imposta il checkbox
    onAllDayToggled(allDay);

    getStartTimeEdit()->setTime(QTime(e->getStartTime().getOre(), e->getStartTime().getMin()));
    getEndTimeEdit()->setTime(QTime(e->getEndTime().getOre(), e->getEndTime().getMin()));
    if(getLocationEdit()) getLocationEdit()->setText(QString::fromStdString(e->getLocation()));

}

void EventForm::saveToActivity(AbstractActivity* act) {
    auto* e = dynamic_cast<Event*>(act);
    if (!e) return;
    e->setName(nameEdit->text().toStdString());
    e->setDesc(descEdit->text().toStdString());
    e->setTag(tagCombo->getSelectedTag());
    date newStart(startDateEdit->date().day(), startDateEdit->date().month(), startDateEdit->date().year());
    date newEnd(endDateEdit->date().day(), endDateEdit->date().month(), endDateEdit->date().year());

    if (newStart > e->getEndDate()) {
        e->setEndDate(newEnd);
        e->setStartDate(newStart);
    }

    else if (e->getStartDate() > newEnd ) {
        e->setStartDate(newStart);
        e->setEndDate(newEnd);
    }
    else {
        e->setStartDate(newStart);
        e->setEndDate(newEnd);
    }    e->setLocation(getLocationEdit()->text().toStdString());


    if (allDayCheck->isChecked()) {
        //if all day time star and time end equal to 00:00
        e->setStartTime(HourMinute(0, 0));
        e->setEndTime(HourMinute(23, 59));
    } else {
        e->setStartTime(HourMinute(startTimeEdit->time().hour(), startTimeEdit->time().minute()));
        e->setEndTime(HourMinute(endTimeEdit->time().hour(), endTimeEdit->time().minute()));
    }

}