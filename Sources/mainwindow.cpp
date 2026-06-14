#include <QMainWindow>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QHBoxLayout>

#include "Headers/mainwindow.h"
#include "Headers/UI/TagWidget.h"
#include "Headers/UI/navBar.h"
#include "Headers/UI/TaskWidget.h"
#include "Headers/Dialog/AddDialog.h"
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/UI/calendar.h"
#include "Headers/UI/ActivityDelete.h"
#include "Headers/UI/ActivitySearch.h"

#include "Headers/UI/DayWidget.h"


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

    // 1️⃣ Prima crea i widget (addView nasce qui)
    setupStackedWidget();
    // 2️⃣ Poi la navbar (usa addView già esistente)
    setupNavBar();

    // 3️⃣ navContainer/navStack creati UNA SOLA VOLTA
    navStack->addWidget(navigationBar); // index 0
    navStack->addWidget(addView);       // index 1
    navStack->addWidget(tagView);       // index 2

    mainLayout->addWidget(navContainer);
    mainLayout->addWidget(stackedWidget);

    connect(calendarView->getMonthWidget(), &MonthWidget::activityUpdated,
            this, [this]() {
                taskWidget->refresh();
            });

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


    connect(tagView, &TagWidget::tagsChanged, this, [this](){
        addView->refreshTagCombo();

        // have to block signal for tagWidget
        navigationBar->getFilterCombo()->getCombo()->blockSignals(true);
        navigationBar->getFilterCombo()->tagPopulation();
        navigationBar->getFilterCombo()->getCombo()->blockSignals(false);
    });


    connect(navigationBar, &navBar::searchTextChanged, this, [this, &am](const QString &text) {
        if (text.isEmpty()) {
            showCalendar(); // O nascondi i risultati
            return;
        }

        // Cerca, aggiorna e mostra
        auto results = ActivitySearch::findByName(am, text.toStdString());
        updateSearchUI(results);
        showSearch();
    });

    connect(navigationBar->getFilterCombo()->getCombo(), &QComboBox::currentTextChanged, this, [this, &am](const QString& text){
        std::string name = text.toStdString();

        // Filtra e aggiorna
        auto results = ActivitySearch::findByTag(am, name);
        updateSearchUI(results);
        showSearch(); // Passa alla pagina dei risultati
    });

    resize(1200, 800);
    setWindowTitle("Activity Manager");

    showCalendar();
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

    // view navigation
    connect(navigationBar, &navBar::calendarClicked, this, &MainWindow::showCalendar);
    connect(navigationBar, &navBar::taskProjectClicked, this, &MainWindow::showTaskProject);
    connect(navigationBar, &navBar::searchClicked, this, &MainWindow::showSearch);
    connect(navigationBar, &navBar::tagsClicked, this, &MainWindow::showTags); // quando avrai la vista

    connect(navigationBar, &navBar::addClicked, this, [this]{
        navStack->setCurrentIndex(1);
    });

    // back to navbar
    connect(addView, &AddDialog::activityCreated,
            this, [this](AbstractActivity* a){
                if (!a) return;
                am.add(a);
                taskWidget->refresh();
                navStack->setCurrentIndex(0);
                calendarView->refresh();
                //showTaskProject();
            });
    connect(addView, &AddDialog::activityCancelled, this, [this]{
        navStack->setCurrentIndex(0);
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

void MainWindow::showTags()
{
    // Facciamo il cast di tagView al suo tipo reale TagWidget per chiamare il refresh
    if (tagView) {
        tagView->refresh();
    }

    // Mostra il widget nel QStackedWidget
    navStack->setCurrentIndex(2);
}


void MainWindow::updateSearchUI(const std::vector<AbstractActivity*>& results) {
    // 1. Pulisci i risultati precedenti
    QLayoutItem* child;
    while ((child = searchResultLayout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // 2. Crea i widget per ogni attività trovata usando il Visitor
    for (AbstractActivity* act : results) {
        QWidget* container = new QWidget(this);
        QVBoxLayout* vLayout = new QVBoxLayout(container);

        DisplayVisitor visitor;
        act->accept(visitor);
        visitor.applyToLayout(vLayout, this);

        searchResultLayout->addWidget(container);
    }
}