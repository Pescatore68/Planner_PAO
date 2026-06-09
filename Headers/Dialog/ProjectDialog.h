#ifndef PROJECTDIALOG_H
#define PROJECTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ProjectDialog : public QDialog {
    Q_OBJECT

public:
    ProjectDialog(QWidget* parent = nullptr);

    QString getTitle() const;
    QString getDescription() const;
    QDate getDate() const;
    QTime getTime() const;

private:
    QLineEdit* titleEdit;
    QLineEdit* descEdit;
    QDateEdit* dateEdit;
    QTimeEdit* timeEdit;

    QPushButton* okBtn;
    QPushButton* cancelBtn;
};

#endif // PROJECTDIALOG_H
