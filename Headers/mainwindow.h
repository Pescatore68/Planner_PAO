#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>

#include "Headers/ActivityManager.h"
#include "Headers/UI/TagWidget.h"
#include "Headers/tagManager.h"
#include "Headers/UI/navBar.h"
#include "Headers/UI/calendar.h"

class TaskWidget;
class AbstractActivity;
class AddDialog;
class QStackedLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ActivityManager& am;
    tagManager& tm;


    TaskWidget* taskWidget = nullptr;
    AddDialog* addView = nullptr;

    QWidget* centralWidget = nullptr;
    QHBoxLayout* mainLayout = nullptr;

    QVBoxLayout* searchResultLayout;
    void updateSearchUI(const std::vector<AbstractActivity*>& results);


    AbstractActivity* selectedActivity = nullptr;

    //navbar
    navBar* navigationBar = nullptr;
    QWidget* navContainer = nullptr;
    QStackedLayout* navStack = nullptr;


    QStackedWidget* stackedWidget = nullptr;

    calendar* calendarView = nullptr;
    QWidget* searchView = nullptr;
    TagWidget* tagView = nullptr;
    void setupNavBar();
    void setupStackedWidget();

public:
    MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    ~MainWindow() = default;

    void showCalendar();
    void showTaskProject();
    void showSearch();
    void showTags();
    void showDetail(AbstractActivity* a);
    void showForm(AbstractActivity* a = nullptr);
    void onActivitySelected(AbstractActivity* a);
};

#endif // MAINWINDOW_H