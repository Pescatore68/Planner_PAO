#include "Headers/UI/Forms/ProjectForm.h"
#include "Headers/project.h"

#include <QMessageBox>

ProjectForm::ProjectForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent)
{
    buildCommonFields(tm);

    deadlineEdit = new QDateEdit(QDate::currentDate(), this);
    deadlineEdit->setCalendarPopup(true);
    deadlineEdit->setDisplayFormat("dd/MM/yyyy");

    deadlineTimeEdit = new QTimeEdit(QTime(23, 59), this);
    deadlineTimeEdit->setDisplayFormat("HH:mm");

    completedCheck = new QCheckBox(this);

    addRow("Scadenza", deadlineEdit);
    addRow("Ora scadenza", deadlineTimeEdit);
    addRow("Completato", completedCheck);

    mainLayout->addStretch();
}

bool ProjectForm::validate()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo obbligatorio",
            "Inserisci un nome per il progetto."
            );
        nameEdit->setFocus();
        return false;
    }

    return true;
}

AbstractActivity* ProjectForm::createActivity(tagManager& tm)
{
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const tag* t = tagCombo->getSelectedTag();

    const QDate qd = deadlineEdit->date();
    const QTime qt = deadlineTimeEdit->time();

    return new project(
        name,
        desc,
        t,
        date(qd.day(), qd.month(), qd.year()),
        HourMinute(qt.hour(), qt.minute()),
        completedCheck->isChecked()
        );
}