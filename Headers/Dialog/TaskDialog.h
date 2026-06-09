#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class TaskDialog : public QDialog {
    Q_OBJECT

public:
    TaskDialog(QWidget* parent = nullptr);

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
