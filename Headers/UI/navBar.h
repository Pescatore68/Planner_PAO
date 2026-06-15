#ifndef NAVBAR_H
#define NAVBAR_H

#include "Headers/tagManager.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "Headers/tagManager.h"
#include "Headers/UI/TagComboBox.h"

class navBar : public QWidget {
    Q_OBJECT

private:
    tagManager& tm;
    QVBoxLayout* layout;
    QPushButton* btnCalendar;
    QPushButton* btnSearch;
    QPushButton* btnFilter;
    QPushButton* btnTags;
    QPushButton* btnAdd;
    QLineEdit* searchEdit;
    TagComboBox* filterCombo;

    void setup();

public:
    explicit navBar(tagManager& tm, QWidget* parent = nullptr);
    TagComboBox* getFilterCombo() const { return filterCombo; }
signals:
    void calendarClicked();
    void searchClicked();
    void tagsClicked();
    void addClicked();
    void searchTextChanged(const QString& text);
};

#endif