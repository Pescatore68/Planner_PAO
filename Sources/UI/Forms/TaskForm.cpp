#include "Headers/UI/Forms/TaskForm.h"
#include "Headers/task.h"
#include <QMessageBox>

TaskForm::TaskForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent)
{
    buildCommonFields(tm);

    deadlineEdit = new QDateEdit(QDate::currentDate(), this);
    deadlineEdit->setCalendarPopup(false);
    deadlineEdit->setDisplayFormat("dd/MM/yyyy");

    oDeadlineEdit = new QTimeEdit(QTime(23, 59), this);
    oDeadlineEdit->setDisplayFormat("HH:mm");

    addTimeRow("Scadenza", deadlineEdit, oDeadlineEdit);

    mainLayout->addStretch();
}

bool TaskForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campo obbligatorio", "Inserisci un nome per il task.");
        nameEdit->setFocus();
        return false;
    }
    return true;
}

AbstractActivity* TaskForm::createActivity() {
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const tag* t = tagCombo->getSelectedTag();

    const QDate qd = deadlineEdit->date();
    const QTime qt = oDeadlineEdit->time();

    return new task(name, desc, t,
                    date(qd.day(), qd.month(), qd.year()),
                    HourMinute(qt.hour(), qt.minute()));
}

void TaskForm::reset() {
    ActivityForm::reset();
    deadlineEdit->setDate(QDate::currentDate());
    oDeadlineEdit->setTime(QTime(23, 59));
}