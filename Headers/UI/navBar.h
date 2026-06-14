#ifndef NAVBAR_H
#define NAVBAR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class navBar : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout* layout;
    QPushButton* btnCalendar;
    QPushButton* btnTaskProject;
    QPushButton* btnSearch;
    QPushButton* btnTags;
    QPushButton* btnAdd;
    QLineEdit* searchEdit;


    void setup();

public:
    explicit navBar(QWidget* parent = nullptr);

signals:
    void calendarClicked();
    void taskProjectClicked();
    void searchClicked();
    void tagsClicked();
    void addClicked();
    void searchTextChanged(const QString& text);
};

#endif