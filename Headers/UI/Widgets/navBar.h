#ifndef NAVBAR_H
#define NAVBAR_H

#include "Headers/Model/tagManager.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "Headers/Model/tagManager.h"
#include "Headers/UI/Widgets/TagComboBox.h"

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
    QPushButton* loadBtn;
    QPushButton* saveBtn;

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
    void loadClicked();
    void saveClicked();

};

#endif