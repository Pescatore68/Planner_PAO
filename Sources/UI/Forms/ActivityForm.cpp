#include "Headers/UI/Forms/ActivityForm.h"

ActivityForm::ActivityForm(QWidget* parent)
    : QWidget(parent),
      mainLayout(new QVBoxLayout(this)),
      nameEdit(new QLineEdit(this)),
      descEdit(new QLineEdit(this)),
      tagCombo(nullptr)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(6);
    setLayout(mainLayout);
}

void ActivityForm::addRow(const QString& labelText, QWidget* field) {
    auto* row   = new QHBoxLayout();
    auto* label = new QLabel(labelText, this);
    label->setFixedWidth(110);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row->addWidget(label);
    row->addWidget(field);
    mainLayout->addLayout(row);
}

void ActivityForm::buildCommonFields(tagManager& tm) {
    nameEdit->setPlaceholderText("Nome");
    descEdit->setPlaceholderText("Descrizione (opzionale)");
    tagCombo = new TagComboBox(tm, this);

    addRow("Nome *",      nameEdit);
    addRow("Descrizione", descEdit);
    addRow("Tag",         tagCombo);
}
