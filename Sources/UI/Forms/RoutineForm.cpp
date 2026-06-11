#include "Headers/UI/Forms/RoutineForm.h"
#include "Headers/routine.h"
#include <QMessageBox>

RoutineForm::RoutineForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent)
{
    buildCommonFields(tm);

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

    startTimeEdit = new QTimeEdit(QTime(7, 0), this);
    startTimeEdit->setDisplayFormat("HH:mm");

    endTimeEdit = new QTimeEdit(QTime(8, 0), this);
    endTimeEdit->setDisplayFormat("HH:mm");

    addRow("Frequenza",   freqCombo);
    addRow("Data inizio", startDateEdit);
    addRow("Data fine",   endDateEdit);
    addRow("Ora inizio",  startTimeEdit);
    addRow("Ora fine",    endTimeEdit);

    mainLayout->addStretch();
}

bool RoutineForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campo obbligatorio", "Inserisci un nome per la routine.");
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

AbstractActivity* RoutineForm::createActivity(tagManager& tm) {
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