#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class AbstractActivity; // forward declaration
class task;             // forward declaration
class project;          // forward declaration

class TaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskWidget(QWidget* parent = nullptr);
    void setActivity(AbstractActivity* a);

signals:
    void backRequested();
    void deleteRequested(AbstractActivity* a);

private slots:
    void onCompleteClicked();
    void onDeleteClicked();

private:
    QVBoxLayout* mainLayout;
    QHBoxLayout* headerLayout;

    QLabel*       labelType;
    QLabel*       labelTitle;
    QLabel*       labelDescription;
    QLabel*       labelDeadline;
    QProgressBar* progressBar;
    QPushButton*  btnComplete;
    QPushButton*  btnDelete;
    QPushButton*  btnBack;

    AbstractActivity* current;
};

#endif
