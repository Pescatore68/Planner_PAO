#include "Headers/UI/Forms/ActivityForm.h"

ActivityForm::ActivityForm(tagManager& tm, QWidget* parent)
    : QWidget(parent),
    tm(tm),
    mainLayout(new QVBoxLayout(this)),
    nameEdit(new QLineEdit(this)),
    descEdit(new QLineEdit(this)),
    tagCombo(nullptr)
{
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);
    setLayout(mainLayout);
    buildCommonFields();
}
void ActivityForm::addRow(QWidget* field, const QString& labelText) {
    auto* row = new QHBoxLayout();

    if (!labelText.isEmpty()) {
        auto* label = new QLabel(labelText, this);
        label->setFixedWidth(110);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        row->addWidget(label);
    }
    if (field) {
        row->addWidget(field);
    }

    mainLayout->addLayout(row);
}

void ActivityForm::addTimeRow(const QString& labelText, QWidget* dateField, QWidget* timeField) {
    auto* row   = new QHBoxLayout();
    auto* label = new QLabel(labelText, this);

    row->addWidget(label);
    row->addStretch();
    row->addWidget(dateField);
    row->addWidget(timeField);

    mainLayout->addLayout(row);
}

void ActivityForm::buildCommonFields() {
    nameEdit->setPlaceholderText("Name *");
    descEdit->setPlaceholderText("Description (optional)");
    tagCombo = new TagComboBox(tm, this);

    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(descEdit);
    mainLayout->addWidget(tagCombo);
}

QVBoxLayout* ActivityForm::getMainLayout() {
    return mainLayout;
}

void ActivityForm::reset() {
    nameEdit->clear();
    descEdit->clear();
    tagCombo->setCurrentIndex(0);
}
void ActivityForm::refreshTags() {
    if (tagCombo) {
        tagCombo->tagPopulation();
    }
}