#include "Headers/UI/TagComboBox.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QColorDialog>
#include <QVBoxLayout>

TagComboBox::TagComboBox(tagManager& tm, QWidget* parent): QWidget(parent), tm(tm) {

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    combo  = new QComboBox(this);
    /*bAdd = new QPushButton("+", this);
    bAdd->setFixedWidth(30);*/

    layout->addWidget(combo);
    ///layout->addWidget(bAdd);

    tagPopulation();

    //connect(bAdd, &QPushButton::clicked, this, &TagComboBox::onAddTag);
}

void TagComboBox::tagPopulation(){
    combo->clear();
    for (tag* t : tm.getTags()) {
        QPixmap px(16, 16);
        px.fill(t->getColor());
        combo->addItem(QIcon(px), QString::fromStdString(t->getName()));
    }
}

tag* TagComboBox::getSelectedTag() const {
    tag* t = tm.findTag(combo->currentText().toStdString());
    if (!t) t = tm.getDefaultTag();
    return t;
}

/*void TagComboBox::onAddTag() {
    QDialog dialog(this);
    dialog.setWindowTitle("New tag");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    QFormLayout* form   = new QFormLayout();

    QLineEdit* name  = new QLineEdit(&dialog);
    QPushButton* colorBtn = new QPushButton("Color", &dialog);
    QColor selectedColor  = QColor(128, 128, 128);

    form->addRow("Name:",   name);
    form->addRow("Color:", colorBtn);
    layout->addLayout(form);

    connect(colorBtn, &QPushButton::clicked, this, [&]() {
        QColor c = QColorDialog::getColor(selectedColor, this);
        if (c.isValid()) {
            selectedColor = c;
            colorBtn->setStyleSheet("background-color: " + c.name());
        }
    });

    QHBoxLayout* bLayout  = new QHBoxLayout();
    QPushButton* bConfirm = new QPushButton("Confirm", &dialog);
    QPushButton* bCancel  = new QPushButton("Cancel", &dialog);
    bLayout->addWidget(bCancel);
    bLayout->addWidget(bConfirm);
    layout->addLayout(bLayout);

    connect(bCancel,  &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(bConfirm, &QPushButton::clicked, &dialog, [&]() {
        if (!name->text().isEmpty()) {
            tm.newTag(name->text().toStdString(), selectedColor);
            //add tag on combo
            tagPopulation();
            //select the new tag
            combo->setCurrentText(name->text());
            dialog.accept();
        }
    });

    dialog.exec();
}*/


void TagComboBox::setCurrentIndex(const unsigned int& idx) const {
    combo->setCurrentIndex(idx);
}
void TagComboBox::setCurrentTagByName(const std::string& tagName) const {
    if (!combo) return;

    // Cerchiamo il testo all'interno della QComboBox reale e cambiamo l'indice
    int index = combo->findText(QString::fromStdString(tagName));
    if (index != -1) {
        combo->setCurrentIndex(index);
    }
}