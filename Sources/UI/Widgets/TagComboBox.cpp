#include "Headers/UI/Widgets/TagComboBox.h"
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
    //layout->addWidget(bAdd);

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


void TagComboBox::setCurrentIndex(const unsigned int& idx) const {
    combo->setCurrentIndex(idx);
}

void TagComboBox::setCurrentTagByName(const std::string& tagName) const {
    if (!combo) return;

    //assign indx to any tag
    int index = combo->findText(QString::fromStdString(tagName));
    if (index != -1) {
        combo->setCurrentIndex(index);
    }
}