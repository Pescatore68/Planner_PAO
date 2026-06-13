#include "Headers/UI/Forms/RoutineForm.h"
#include "Headers/routine.h"
#include <QMessageBox>

RoutineForm::RoutineForm(tagManager& tm, QWidget* parent)
    :  ActivityForm(tm, parent)
{

    freqCombo = new QComboBox(this);
    freqCombo->addItem("Daily",   static_cast<int>(Routine::Frequency::Daily));
    freqCombo->addItem("Weekly",  static_cast<int>(Routine::Frequency::Weekly));
    freqCombo->addItem("Monthly", static_cast<int>(Routine::Frequency::Monthly));
    freqCombo->addItem("Yearly",  static_cast<int>(Routine::Frequency::Yearly));

    startDateEdit = new QDateEdit(QDate::currentDate(), this);
    startDateEdit->setCalendarPopup(true);
    startDateEdit->setDisplayFormat("dd/MM/yyyy");

    endDateEdit = new QDateEdit(QDate::currentDate().addMonths(1), this);
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");

    startTimeEdit = new QTimeEdit(QTime::currentTime(), this);
    startTimeEdit->setDisplayFormat("HH:mm");

    endTimeEdit = new QTimeEdit(QTime::currentTime().addSecs(3600), this);
    endTimeEdit->setDisplayFormat("HH:mm");

    addRow(freqCombo, "Frequency");
    addTimeRow("Starts", startDateEdit, startTimeEdit);
    addTimeRow("Ends",   endDateEdit, endTimeEdit);

    mainLayout->addStretch();
}

bool RoutineForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Required field", "Please enter a name for the routine.");
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

AbstractActivity* RoutineForm::createActivity() {
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const tag* t           = tagCombo->getSelectedTag();

    const QDate qsd = startDateEdit->date();
    const QDate qed = endDateEdit->date();
    const QTime qst = startTimeEdit->time();
    const QTime qet = endTimeEdit->time();

    auto freq = static_cast<Routine::Frequency>(freqCombo->currentData().toInt());

    return new Routine(name, desc, t,
                       HourMinute(qst.hour(), qst.minute()),
                       HourMinute(qet.hour(), qet.minute()),
                       date(qsd.day(), qsd.month(), qsd.year()),
                       date(qed.day(), qed.month(), qed.year()),
                       freq);
}

void RoutineForm::reset() {
    ActivityForm::reset();
    freqCombo->setCurrentIndex(0);
    startDateEdit->setDate(QDate::currentDate());
    endDateEdit->setDate(QDate::currentDate());
    startTimeEdit->setTime(QTime::currentTime());
    endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
}