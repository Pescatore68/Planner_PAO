#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QWidget>

#include "Headers/Model/ActivityManager.h"
#include "Headers/UI/Widgets/TagWidget.h"
#include "Headers/Model/tagManager.h"
#include "Headers/UI/Widgets/navBar.h"
#include "Headers/UI/Widgets/calendar.h"

class TaskWidget;
class AbstractActivity;
class AddDialog;
class ActivityModify;
class QStackedLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    ActivityManager& am;
    tagManager& tm;

    TaskWidget* taskWidget = nullptr;
    AddDialog*  addView    = nullptr;


    QVBoxLayout* searchResultLayout;
    void updateSearchUI(const std::vector<AbstractActivity*>& results);


    QWidget*        centralWidget = nullptr;
    QHBoxLayout*    mainLayout    = nullptr;


    AbstractActivity* selectedActivity = nullptr;

    // navbar
    navBar*         navigationBar = nullptr;
    QWidget*        navContainer  = nullptr;
    QStackedLayout* navStack      = nullptr;

    QStackedWidget* stackedWidget = nullptr;

    calendar*   calendarView = nullptr;
    QWidget*    searchView   = nullptr;
    TagWidget*  tagView      = nullptr;

    // ← NUOVO: widget di modifica corrente nel navStack (index 3)
    ActivityModify* currentModifyWidget = nullptr;

    void setupNavBar();
    void setupStackedWidget();

    // ← NUOVO: indici fissi del navStack
    static constexpr int NAV_IDX_NAVBAR  = 0;
    static constexpr int NAV_IDX_ADD     = 1;
    static constexpr int NAV_IDX_TAGS    = 2;
    static constexpr int NAV_IDX_MODIFY  = 3;

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

    // ← NUOVO
    void showModify(AbstractActivity* a);
};

#endif // MAINWINDOW_H