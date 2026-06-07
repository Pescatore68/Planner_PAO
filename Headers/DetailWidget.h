#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class AbstractActivity; // forward declaration
class task;             // forward declaration
class project;          // forward declaration

class DetailWidget : public QWidget {
    Q_OBJECT

public:
    explicit DetailWidget(QWidget* parent = nullptr);
    void setActivity(AbstractActivity* a);

signals:
    void backRequested();

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
