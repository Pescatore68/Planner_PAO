#include <QMainWindow>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QHBoxLayout>

#include "Headers/mainwindow.h"
#include "Headers/UI/TagWidget.h"
#include "Headers/UI/navBar.h"
#include "Headers/UI/TaskWidget.h"
#include "Headers/Dialog/AddDialog.h"
#include "Headers/routine.h"
#include "Headers/UI/calendar.h"
#include "Headers/UI/ActivityDelete.h"
#include "Headers/UI/DayWidget.h"
#include "Headers/UI/ActivityModify.h"

MainWindow::MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QMainWindow(parent), am(am), tm(tm)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    navContainer = new QWidget(this);
    navContainer->setFixedWidth(300);
    navStack = new QStackedLayout(navContainer);

    setupStackedWidget();
    setupNavBar();

    navStack->addWidget(navigationBar);
    navStack->addWidget(addView);
    navStack->addWidget(tagView);

    mainLayout->addWidget(navContainer);
    mainLayout->addWidget(stackedWidget);

    connect(calendarView->getMonthWidget(), &MonthWidget::activityUpdated,
            this, [this]() { taskWidget->refresh(); });

    connect(taskWidget, &TaskWidget::activityUpdated,
            this, [this]() {
                calendarView->refresh();
                calendarView->getMonthWidget()->updateCalendarView();
            });

    connect(&(calendarView->getMonthWidget()->getActivityDelete()), &ActivityDelete::activityDeleted,
            this, [this]() {
                taskWidget->refresh();
                calendarView->refresh();
            });

    connect(&(taskWidget->getActivityDelete()), &ActivityDelete::activityDeleted,
            this, [this]() {
                calendarView->refresh();
                calendarView->getMonthWidget()->updateCalendarView();
            });

    connect(tagView, &TagWidget::tagViewClosed,
            this, [this]{ navStack->setCurrentIndex(NAV_IDX_NAVBAR); });

    connect(tagView, &TagWidget::tagsChanged, addView, &AddDialog::refreshTagCombo);

    connect(calendarView, &calendar::activityModifyRequested,
            this, &MainWindow::showModify);

    connect(taskWidget, &TaskWidget::activityDoubleClicked,
            this, &MainWindow::showModify);

    resize(1200, 800);
    setWindowTitle("Activity Manager");

    showCalendar();
}

void MainWindow::showModify(AbstractActivity* a)
{
    if (!a) return;

    if (currentModifyWidget) {
        navStack->removeWidget(currentModifyWidget);
        delete currentModifyWidget;
        currentModifyWidget = nullptr;
    }

    currentModifyWidget = new ActivityModify(a, tm, navContainer);
    navStack->addWidget(currentModifyWidget);
    navStack->setCurrentWidget(currentModifyWidget);

    connect(currentModifyWidget, &ActivityModify::modificationFinished,
            this, [this]() {
                navStack->setCurrentIndex(NAV_IDX_NAVBAR);
                calendarView->refresh();
                calendarView->getMonthWidget()->updateCalendarView();
                taskWidget->refresh();
                if (currentModifyWidget) {
                    navStack->removeWidget(currentModifyWidget);
                    delete currentModifyWidget;
                    currentModifyWidget = nullptr;
                }
            });
}

void MainWindow::setupStackedWidget()
{
    stackedWidget = new QStackedWidget(this);

    calendarView = new calendar(am, tm, this);
    taskWidget   = new TaskWidget(am, this);
    searchView   = new QWidget(this);
    tagView      = new TagWidget(tm, am, this);
    addView      = new AddDialog(tm, this);

    stackedWidget->addWidget(calendarView);
    stackedWidget->addWidget(taskWidget);
    stackedWidget->addWidget(searchView);
    stackedWidget->addWidget(tagView);
    stackedWidget->addWidget(addView);

    connect(calendarView, &calendar::dateSelected,
            this, [this](const date& d) {
                qDebug() << QString::fromStdString(d.toString());
            });
}

void MainWindow::setupNavBar()
{
    navigationBar = new navBar(this);

    connect(navigationBar, &navBar::calendarClicked,    this, &MainWindow::showCalendar);
    connect(navigationBar, &navBar::taskProjectClicked, this, &MainWindow::showTaskProject);
    connect(navigationBar, &navBar::searchClicked,      this, &MainWindow::showSearch);
    connect(navigationBar, &navBar::tagsClicked,        this, &MainWindow::showTags);

    connect(navigationBar, &navBar::addClicked, this, [this]{
        navStack->setCurrentIndex(NAV_IDX_ADD);
    });

    connect(addView, &AddDialog::activityCreated,
            this, [this](AbstractActivity* a){
                if (!a) return;
                am.add(a);
                taskWidget->refresh();
                navStack->setCurrentIndex(NAV_IDX_NAVBAR);
                calendarView->refresh();
            });

    connect(addView, &AddDialog::activityCancelled, this, [this]{
        navStack->setCurrentIndex(NAV_IDX_NAVBAR);
    });
}

void MainWindow::showCalendar()     { stackedWidget->setCurrentWidget(calendarView); }
void MainWindow::showSearch()       { stackedWidget->setCurrentWidget(searchView);   }
void MainWindow::showTaskProject()  { stackedWidget->setCurrentWidget(taskWidget);   }

void MainWindow::showTags()
{
    if (tagView) tagView->refresh();
    navStack->setCurrentIndex(NAV_IDX_TAGS);
}