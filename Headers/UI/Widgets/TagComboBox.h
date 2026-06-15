#ifndef TAGCOMBOBOX_H
#define TAGCOMBOBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include "Headers/Model/tagManager.h"

class TagComboBox : public QWidget{
    Q_OBJECT

private:
    tagManager& tm;
    QComboBox* combo;
    //QPushButton* bAdd;

public:
    explicit TagComboBox(tagManager& tm, QWidget* parent = nullptr);
    tag* getSelectedTag() const;
    QComboBox* getCombo() const { return combo; }
    void setCurrentIndex(const unsigned int&) const;
    void tagPopulation();
    void setCurrentTagByName(const std::string& tagName) const;

/*private slots:
void onAddTag();*/
};

#endif // TAGCOMBOBOX_H
