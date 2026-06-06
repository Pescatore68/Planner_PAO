#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ActivityManager& am;
    tagManager& tm;

    QWidget* centralWidget;
    QHBoxLayout* mainLayout;

    // Nav bar
    QWidget* navBar;
    QVBoxLayout* navLayout;
    QPushButton* btnCalendar;
    QPushButton* btnTaskProject;
    QPushButton* btnSearch;
    QPushButton* btnTags;

    // Schermate
    QStackedWidget* stackedWidget;
    QWidget* calendarView;
    QWidget* taskProjectView;
    QWidget* searchView;
    QWidget* detailView;
    QWidget* formView;
    QWidget* tagView;

    void setupNavBar();
    void setupStackedWidget();

public:
    MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    ~MainWindow() = default;

    void showCalendar();
    void showTaskProject();
    void showSearch();
    void showDetail(AbstractActivity* a);
    void showForm(AbstractActivity* a = nullptr);
};

#endif