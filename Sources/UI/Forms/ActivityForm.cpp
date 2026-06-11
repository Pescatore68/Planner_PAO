#include "Headers/UI/Forms/ActivityForm.h"

ActivityForm::ActivityForm(QWidget* parent)
    : QWidget(parent),
      mainLayout(new QVBoxLayout(this)),
      nameEdit(new QLineEdit(this)),
      descEdit(new QLineEdit(this)),
      tagCombo(nullptr)
{
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);
    setLayout(mainLayout);
}

void ActivityForm::addRow(const QString& labelText, QWidget* field) {
    auto* row   = new QHBoxLayout();
    auto* label = new QLabel(labelText, this);
    label->setFixedWidth(110);
    label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    row->addWidget(label);
    row->addWidget(field);
    mainLayout->addLayout(row);
}

void ActivityForm::addTimeRow(const QString& labelText, QWidget* dateField, QWidget* timeField) {
    auto* row   = new QHBoxLayout();
    auto* label = new QLabel(labelText, this);

    row->addWidget(label);
    row->addStretch();          // spinge data+ora a destra
    row->addWidget(dateField);
    row->addWidget(timeField);

    mainLayout->addLayout(row);
}

void ActivityForm::buildCommonFields(tagManager& tm) {
    nameEdit->setPlaceholderText("Nome *");
    descEdit->setPlaceholderText("Descrizione (opzionale)");
    tagCombo = new TagComboBox(tm, this);

    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(descEdit);
    mainLayout->addWidget(tagCombo);
}

QVBoxLayout* ActivityForm::getMainLayout() {
    return mainLayout;
}