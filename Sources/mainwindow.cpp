#include "Headers/mainwindow.h"
#include "Headers/AbstractActivity.h"
#include "Headers/UI/addDialog.h"
#include "Headers/UI/navBar.h"
#include <QLabel>

MainWindow::MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QMainWindow(parent), am(am), tm(tm) {

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setupNavBar();
    setupStackedWidget();

    mainLayout->addWidget(navigationBar);
    mainLayout->addWidget(stackedWidget);

    resize(1200, 800);
    setWindowTitle("Activity Manager");
}

void MainWindow::setupNavBar() {
    navigationBar = new navBar(this);

    connect(navigationBar, &navBar::calendarClicked, this, &MainWindow::showCalendar);
    connect(navigationBar, &navBar::taskProjectClicked, this, &MainWindow::showTaskProject);
    connect(navigationBar, &navBar::searchClicked, this, &MainWindow::showSearch);
    connect(navigationBar, &navBar::tagsClicked, this, [this]{ stackedWidget->setCurrentWidget(tagView); });
    connect(navigationBar, &navBar::addClicked, this, [this]{ showForm(nullptr); });
}

void MainWindow::setupStackedWidget() {
    stackedWidget = new QStackedWidget();

    calendarView = new calendar(this);
    taskProjectView = new QWidget();
    searchView = new QWidget();
    detailView = new QWidget();
    formView = new QWidget();
    tagView = new QWidget();

    stackedWidget->addWidget(calendarView);    // 0
    stackedWidget->addWidget(taskProjectView); // 1
    stackedWidget->addWidget(searchView);      // 2
    stackedWidget->addWidget(detailView);      // 3
    stackedWidget->addWidget(formView);        // 4
    stackedWidget->addWidget(tagView);         // 5

    // connetti segnale del calendario
    connect(calendarView, &calendar::dateSelected,
            this, [this](const date& d) {
                // per ora stampa solo la data selezionata
                qDebug() << "Data selezionata:"
                         << QString::fromStdString(d.toString());
            });

    showCalendar();
    connect(navigationBar, &navBar::addClicked, this, [this]() {
        addDialog dialog(am, tm, this);
        dialog.exec();
    });
}

void MainWindow::showCalendar()    { stackedWidget->setCurrentWidget(calendarView); }
void MainWindow::showTaskProject() { stackedWidget->setCurrentWidget(taskProjectView); }
void MainWindow::showSearch()      { stackedWidget->setCurrentWidget(searchView); }
void MainWindow::showDetail(AbstractActivity* a) { stackedWidget->setCurrentWidget(detailView); }
void MainWindow::showForm(AbstractActivity* a)   { stackedWidget->setCurrentWidget(formView); }