#ifndef TAGCOMBOBOX_H
#define TAGCOMBOBOX_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include "Headers/tagManager.h"

class TagComboBox : public QWidget{
    Q_OBJECT

private:
    tagManager& tm;
    QComboBox* combo;
    //QPushButton* bAdd;

public:
    explicit TagComboBox(tagManager& tm, QWidget* parent = nullptr);
    tag* getSelectedTag() const;
    void setCurrentIndex(const unsigned int&) const;
    void tagPopulation();

/*private slots:
void onAddTag();*/
};

#endif // TAGCOMBOBOX_H
