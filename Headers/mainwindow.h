#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>

#include "Headers/ActivityManager.h"
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

    // ── CORE WIDGETS ─────────────────────────────────────────────
    TaskWidget* taskWidget = nullptr;
    AddDialog* addView = nullptr;

    QWidget* centralWidget = nullptr;
    QHBoxLayout* mainLayout = nullptr;


    AbstractActivity* selectedActivity = nullptr;

    // ── NAV AREA (navbar swap) ───────────────────────────────────
    navBar* navigationBar = nullptr;
    QWidget* navContainer = nullptr;
    QStackedLayout* navStack = nullptr;

    // ── CONTENT AREA ─────────────────────────────────────────────
    QStackedWidget* stackedWidget = nullptr;

    calendar* calendarView = nullptr;
    QWidget* searchView = nullptr;
    QWidget* tagView = nullptr;
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
    void onActivitySelected(AbstractActivity* a);
};

#endif // MAINWINDOW_H