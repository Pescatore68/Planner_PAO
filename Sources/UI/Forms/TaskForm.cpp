#include "Headers/UI/Forms/TaskForm.h"
#include "Headers/Model/task.h"
#include <QMessageBox>

TaskForm::TaskForm(tagManager& tm, QWidget* parent)
    : ActivityForm(tm, parent)
{
    deadlineEdit = new QDateEdit(QDate::currentDate(), this);
    deadlineEdit->setCalendarPopup(false);
    deadlineEdit->setDisplayFormat("dd/MM/yyyy");
    oDeadlineEdit = new QTimeEdit(QTime(23, 59), this);
    oDeadlineEdit->setDisplayFormat("HH:mm");
    addTimeRow("Deadline", deadlineEdit, oDeadlineEdit);
    deadlineEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    oDeadlineEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    mainLayout->addStretch();
}

bool TaskForm::validate() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Required field", "Please enter a name for the task.");
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

QDate TaskForm::getDeadlineDate() const {
    return deadlineEdit->date();
}

QDateEdit* TaskForm::getDeadlineEdit() const {
    return deadlineEdit;
}

QTime TaskForm::getODeadlineTime() const {
    return oDeadlineEdit->time();
}

QTimeEdit* TaskForm::getODeadlineEdit() const {
    return oDeadlineEdit;
}


void TaskForm::loadFromActivity(AbstractActivity* act) {
    auto* t = dynamic_cast<task*>(act);
    if (!t) return;
    fillCommonFields(t);
    getDeadlineEdit()->setDate(QDate(t->getDeadline().getYear(), t->getDeadline().getMonth(), t->getDeadline().getDay()));
    getODeadlineEdit()->setTime(QTime(t->getODeadline().getOre(), t->getODeadline().getMin()));
}

void TaskForm::saveToActivity(AbstractActivity* act) {
    auto* t = dynamic_cast<task*>(act);
    if (!t) return;
    t->setName(nameEdit->text().trimmed().toStdString());
    t->setDesc(descEdit->text().trimmed().toStdString());
    t->setTag(tagCombo->getSelectedTag());
    t->setDeadline(date(getDeadlineEdit()->date().day(), getDeadlineEdit()->date().month(), getDeadlineEdit()->date().year()));
    t->setODeadline(HourMinute(getODeadlineEdit()->time().hour(), getODeadlineEdit()->time().minute()));
}