#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "Headers/ActivityManager.h"
#include "Headers/UI/navBar.h"
#include "Headers/tagManager.h"
#include "Headers/UI/calendar.h"
#include <QString>
#include <QLineEdit>
#include <QInputDialog>
#include <QDate>
#include <QTime>
#include "ActivityManager.h"
#include "tagManager.h"
#include "Dialog/TaskDialog.h"

class TaskWidget;
class AbstractActivity;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ActivityManager& am;
    tagManager& tm;
    TaskWidget* taskWidget;

    QWidget* centralWidget;
    QHBoxLayout* mainLayout;

    AbstractActivity* selectedActivity = nullptr;

    // Nav bar
     navBar* navigationBar;

    // Schermate
    QStackedWidget* stackedWidget;
    calendar* calendarView;
    QWidget* taskProjectView;
    QWidget* searchView;
    QWidget* detailView;
    QWidget* formView;
    QWidget* tagView;

    void setupNavBar();
    void setupStackedWidget();

private slots:
    void onNewTaskRequested();

public:
    MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    ~MainWindow() = default;

    void showCalendar();
    void showTaskProject();
    void showSearch();
    void showDetail(AbstractActivity* a);
    void showForm(AbstractActivity* a = nullptr);
    void onActivitySelected(AbstractActivity* a);
    void onNewSubtaskRequested();
};

#endif