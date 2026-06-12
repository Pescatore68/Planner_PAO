#include "Headers/Dialog/TaskDialog.h"

TaskDialog::TaskDialog(QWidget* parent)
    : QDialog(parent)
{
    auto* layout = new QVBoxLayout(this);

    titleEdit = new QLineEdit(this);
    titleEdit->setPlaceholderText("Title");

    descEdit = new QLineEdit(this);
    descEdit->setPlaceholderText("Description");

    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);

    timeEdit = new QTimeEdit(QTime::currentTime(), this);

    okBtn = new QPushButton("OK", this);
    cancelBtn = new QPushButton("Annulla", this);

    auto* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    layout->addWidget(titleEdit);
    layout->addWidget(descEdit);
    layout->addWidget(dateEdit);
    layout->addWidget(timeEdit);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

QString TaskDialog::getTitle() const { return titleEdit->text(); }
QString TaskDialog::getDescription() const { return descEdit->text(); }
QDate TaskDialog::getDate() const { return dateEdit->date(); }
QTime TaskDialog::getTime() const { return timeEdit->time(); }
