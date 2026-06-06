#include "Headers/mainwindow.h"

MainWindow::MainWindow(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QMainWindow(parent), am(am), tm(tm) {

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    setupNavBar();
    setupStackedWidget();

    mainLayout->addWidget(navBar);
    mainLayout->addWidget(stackedWidget);

    resize(1200, 800);
    setWindowTitle("Activity Manager");

    showCalendar();
}

void MainWindow::setupNavBar() {
    navBar = new QWidget();
    navBar->setFixedWidth(200);
    navLayout = new QVBoxLayout(navBar);
    navLayout->setAlignment(Qt::AlignTop);
    navLayout->setSpacing(10);
    navLayout->setContentsMargins(10, 20, 10, 20);

    btnCalendar    = new QPushButton("Calendario", navBar);
    btnTaskProject = new QPushButton("Task/Project", navBar);
    btnSearch      = new QPushButton("Ricerca", navBar);
    btnTags        = new QPushButton("Tag", navBar);

    navLayout->addWidget(btnCalendar);
    navLayout->addWidget(btnTaskProject);
    navLayout->addWidget(btnSearch);
    navLayout->addWidget(btnTags);
    navLayout->addStretch();

    connect(btnCalendar,    &QPushButton::clicked, this, &MainWindow::showCalendar);
    connect(btnTaskProject, &QPushButton::clicked, this, &MainWindow::showTaskProject);
    connect(btnSearch,      &QPushButton::clicked, this, &MainWindow::showSearch);
}

void MainWindow::setupStackedWidget() {
    stackedWidget = new QStackedWidget();

    calendarView    = new QWidget();
    taskProjectView = new QWidget();
    searchView      = new QWidget();
    detailView      = new QWidget();
    formView        = new QWidget();
    tagView         = new QWidget();

    stackedWidget->addWidget(calendarView);    // 0
    stackedWidget->addWidget(taskProjectView); // 1
    stackedWidget->addWidget(searchView);      // 2
    stackedWidget->addWidget(detailView);      // 3
    stackedWidget->addWidget(formView);        // 4
    stackedWidget->addWidget(tagView);         // 5

    showCalendar();
}

void MainWindow::showCalendar()     { stackedWidget->setCurrentWidget(calendarView); }
void MainWindow::showTaskProject()  { stackedWidget->setCurrentWidget(taskProjectView); }
void MainWindow::showSearch()       { stackedWidget->setCurrentWidget(searchView); }
void MainWindow::showDetail(AbstractActivity* a) { stackedWidget->setCurrentWidget(detailView); }
void MainWindow::showForm(AbstractActivity* a)   { stackedWidget->setCurrentWidget(formView); }