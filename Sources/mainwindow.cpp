#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Headers/mainwindow.h"
#include "Headers/AbstractActivity.h"
#include "Headers/UI/navBar.h"
#include "Headers/UI/TaskWidget.h"
#include "Headers/Dialog/AddDialog.h"
#include "Headers/routine.h"

MainWindow::MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QMainWindow(parent), am(am), tm(tm)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setupStackedWidget();
    setupNavBar();

    mainLayout->addWidget(navigationBar);
    mainLayout->addWidget(stackedWidget);

    resize(1200, 800);
    setWindowTitle("Activity Manager");

}

void MainWindow::setupStackedWidget()
{
    stackedWidget = new QStackedWidget(this);

    calendarView = new calendar(am, this);
    taskWidget   = new TaskWidget(am, this);
    searchView   = new QWidget(this);
    tagView      = new QWidget(this);

    stackedWidget->addWidget(calendarView);
    stackedWidget->addWidget(taskWidget);
    stackedWidget->addWidget(searchView);
    stackedWidget->addWidget(tagView);

    connect(calendarView, &calendar::dateSelected,
            this, [this](const date& d) {
                qDebug() << "Data selezionata:"
                         << QString::fromStdString(d.toString());
            });

    showCalendar();
}

void MainWindow::setupNavBar()
{
    navigationBar = new navBar(this);

    connect(navigationBar, &navBar::calendarClicked,
            this, &MainWindow::showCalendar);

    connect(navigationBar, &navBar::taskProjectClicked,
            this, &MainWindow::showTaskProject);

    connect(navigationBar, &navBar::searchClicked,
            this, &MainWindow::showSearch);

    connect(navigationBar, &navBar::tagsClicked,
            this, [this]{
                stackedWidget->setCurrentWidget(tagView);
            });

    connect(navigationBar, &navBar::addClicked, this, [this](){

        AddDialog dialog(tm, this);

        if (dialog.exec() != QDialog::Accepted)
            return;

        AbstractActivity* a = dialog.createActivity();

        if (!a)
            return;

        am.add(a);
        taskWidget->refresh();
        calendarView->refresh();
    });
}

void MainWindow::showCalendar()
{
    stackedWidget->setCurrentWidget(calendarView);
}

void MainWindow::showSearch()
{
    stackedWidget->setCurrentWidget(searchView);
}

void MainWindow::showTaskProject()
{
    stackedWidget->setCurrentWidget(taskWidget);
}

void MainWindow::onActivitySelected(AbstractActivity* a)
{
    selectedActivity = a;
}