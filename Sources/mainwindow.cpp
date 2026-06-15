#include <QMainWindow>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QHBoxLayout>

#include "Headers/mainwindow.h"
#include "Headers/UI/Widgets/TagWidget.h"
#include "Headers/UI/Widgets/navBar.h"
#include "Headers/UI/Widgets/TaskWidget.h"
#include "Headers/UI/Widgets/AddWidget.h"
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/UI/Widgets/calendar.h"
#include "Headers/UI/Widgets/ActivityDelete.h"
#include "Headers/UI/Widgets/ActivitySearch.h"
#include "Headers/UI/Widgets/DayWidget.h"
#include "Headers/UI/Widgets/ActivityModify.h"
#include "Headers/Model/Persistence/PersistenceManager.h"

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

    connect(calendarView->getMonthWidget(), &MonthWidget::dayRequested, this, [this](const QDate& date) {
        showCalendar();
    });

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

    connect(tagView, &TagWidget::tagViewClosed, this, [this]{
        navStack->setCurrentIndex(0);
    });

    connect(tagView, &TagWidget::tagViewClosed,
            this, [this]{ navStack->setCurrentIndex(NAV_IDX_NAVBAR); });

    connect(tagView, &TagWidget::tagsChanged, this, [this](){
        addView->refreshTagCombo();
        navigationBar->getFilterCombo()->getCombo()->blockSignals(true);
        navigationBar->getFilterCombo()->tagPopulation();
        navigationBar->getFilterCombo()->getCombo()->blockSignals(false);
    });

    connect(navigationBar, &navBar::searchTextChanged, this, [this, &am](const QString &text) {
        if (text.isEmpty()) {
            showCalendar();
            return;
        }
        auto results = ActivitySearch::findByName(am, text.toStdString());
        updateSearchUI(results);
        showSearch();
    });

    connect(navigationBar->getFilterCombo()->getCombo(), &QComboBox::currentTextChanged, this, [this, &am](const QString& text){
        std::string name = text.toStdString();
        auto results = ActivitySearch::findByTag(am, name);
        updateSearchUI(results);
        showSearch();
    });

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

    calendarView->getTaskWidget()->setDeleteEnabled(false);

    currentModifyWidget = new ActivityModify(a, tm, navContainer);
    navStack->addWidget(currentModifyWidget);
    navStack->setCurrentWidget(currentModifyWidget);

    connect(currentModifyWidget, &ActivityModify::modificationFinished,
            this, [this]() {
                navStack->setCurrentIndex(NAV_IDX_NAVBAR);
                calendarView->refresh();
                calendarView->getMonthWidget()->updateCalendarView();
                taskWidget->refresh();
                calendarView->getTaskWidget()->setDeleteEnabled(true);

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

    calendarView  = new calendar(am, tm, this);
    taskWidget    = new TaskWidget(am, this);
    searchView    = new QWidget(this);

    searchResultLayout = new QVBoxLayout(searchView);
    searchResultLayout->setAlignment(Qt::AlignTop);

    tagView       = new TagWidget(tm, am, this);
    addView       = new AddDialog(tm, this);

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
    navigationBar = new navBar(tm, this);

    connect(navigationBar, &navBar::calendarClicked,    this, &MainWindow::showCalendar);
    connect(navigationBar, &navBar::searchClicked,      this, &MainWindow::showSearch);
    connect(navigationBar, &navBar::tagsClicked,        this, &MainWindow::showTags);

    connect(navigationBar, &navBar::addClicked, this, [this]{
        navStack->setCurrentIndex(NAV_IDX_ADD);
    });

    connect(navigationBar, &navBar::loadClicked, this, [this]() {
        if (PersistenceManager::loadDataWithDialog(am, tm, this)) {
            taskWidget->refresh();
            calendarView->refresh();
            calendarView->getMonthWidget()->updateCalendarView();
            addView->refreshTagCombo();

            navigationBar->getFilterCombo()->getCombo()->blockSignals(true);
            navigationBar->getFilterCombo()->tagPopulation();
            navigationBar->getFilterCombo()->getCombo()->blockSignals(false);

            showCalendar();
        }
    });

    connect(navigationBar, &navBar::saveClicked, this, [this]() {
        PersistenceManager::saveDataWithDialog(am, tm, this);
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

void MainWindow::updateSearchUI(const std::vector<AbstractActivity*>& results) {
    QLayoutItem* child;
    while ((child = searchResultLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    for (AbstractActivity* act : results) {
        QWidget* container = new QWidget(this);
        QVBoxLayout* vLayout = new QVBoxLayout(container);

        DisplayVisitor visitor;
        act->accept(visitor);
        visitor.applyToLayout(vLayout, this);

        searchResultLayout->addWidget(container);
    }
}