#include "Headers/UI/month.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/date.h"
#include "Headers/AbstractActivity.h"
#include "qcheckbox.h"
#include "qpushbutton.h"
#include "qtextformat.h"
#include "Headers/UI/ActivityModify.h"

MonthWidget::MonthWidget(ActivityManager& am, tagManager& tm, QWidget* parent)
    : QWidget(parent), am(am), tm(tm), activityDelete(am)
{
    setup();
    updateCalendarView();
}
void MonthWidget::setup()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);


    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar, 2);

    QWidget* sidePanel = new QWidget(this);
    sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setContentsMargins(0, 0, 0, 0);;

    titleLabel = new QLabel("Day Activity:", this);

    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    activityList = new QListWidget(this);

    sideLayout->addWidget(titleLabel);
    sideLayout->addWidget(activityList);

    mainLayout->addWidget(sidePanel, 1);


    connect(calendar, &QCalendarWidget::clicked, this, &MonthWidget::onDateChanged);


    connect(calendar, &QCalendarWidget::clicked, this, &MonthWidget::dateClicked);

    onDateChanged(calendar->selectedDate());


    connect(activityList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        if (!item) return;

        // Recuperiamo i puntatori grafici che abbiamo salvato nell'item
        QWidget* expansionWidget = static_cast<QWidget*>(item->data(Qt::UserRole).value<void*>());
        QWidget* rowContainer = static_cast<QWidget*>(item->data(Qt::UserRole + 1).value<void*>());

        if (expansionWidget && rowContainer) {

            bool isCurrentlyVisible = expansionWidget->isVisible();
            expansionWidget->setVisible(!isCurrentlyVisible);

            rowContainer->adjustSize();

            item->setSizeHint(rowContainer->sizeHint());

            activityList->update();
        }
    });

    connect(activityList, &QListWidget::itemDoubleClicked, this, [](QListWidgetItem* item) {
        if (!item) return;

        QWidget* rowContainer = static_cast<QWidget*>(item->data(Qt::UserRole + 1).value<void*>());
        QPushButton* editBtn = rowContainer->findChild<QPushButton*>();
        if (editBtn) {
            editBtn->click();
        }
    });

    connect(calendar, &QCalendarWidget::activated, this, [this](const QDate& date) {
        emit dayRequested(date); // Notifica al genitore (calendar)
    });
}
void MonthWidget::onDateChanged(const QDate& qd)
{
    activityList->clear();

    date backendDate(qd.day(), qd.month(), qd.year());
    auto activities = am.getOnDate(backendDate);

    for (AbstractActivity* act : activities) {
        if (!act) continue;

        if(dynamic_cast<task*>(act) == nullptr){
            QListWidgetItem* item = new QListWidgetItem(activityList);

            QWidget* rowContainer = new QWidget(this);
            QVBoxLayout* rowLayout = new QVBoxLayout(rowContainer);
            rowLayout->setContentsMargins(5, 5, 5, 5);
            rowLayout->setSpacing(0);


            QLabel* titleLabel = new QLabel("• " + QString::fromStdString(act->getName()), this);
            titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 13px; padding: 4px; }");
            rowLayout->addWidget(titleLabel);

            //expan activity
            QWidget* expansionWidget = new QWidget(this);
            QVBoxLayout* expansionLayout = new QVBoxLayout(expansionWidget);
            expansionLayout->setContentsMargins(15, 5, 5, 5);
            expansionLayout->setSpacing(5);

            DisplayVisitor visitor;
            act->accept(visitor);


            QWidget* contentContainer = new QWidget(this);
            QVBoxLayout* contentLayout = new QVBoxLayout(contentContainer);
            contentLayout->setContentsMargins(0, 0, 0, 0);
            contentLayout->setSpacing(2);

            visitor.applyToLayout(contentLayout, this);
            expansionLayout->addWidget(contentContainer);



            if (visitor.isCheckable()) {
                QCheckBox* statusCheck = new QCheckBox("Complete", this);
                statusCheck->setChecked(visitor.getCheckedState());
                expansionLayout->addWidget(statusCheck);

                connect(statusCheck, &QCheckBox::toggled, this, [act, contentLayout, contentContainer, this](bool checked) {
                    DisplayVisitor writeVisitor;
                    writeVisitor.setWriteMode(checked);
                    act->accept(writeVisitor);


                    QLayoutItem* child;
                    while ((child = contentLayout->takeAt(0)) != nullptr) {
                        if (child->widget()) {
                            child->widget()->hide(); // Lo nasconde immediatamente per evitare glitch visivi
                            delete child->widget();  // Lo elimina dalla memoria
                        }
                        delete child;
                    }

                    writeVisitor.applyToLayout(contentLayout, contentContainer);

                    emit activityUpdated();
                    this->updateCalendarView();
                });
            }


            QWidget* actionButtonsWidget = new QWidget(this);
            QHBoxLayout* actionLayout = new QHBoxLayout(actionButtonsWidget);
            actionLayout->setContentsMargins(0, 5, 0, 0);
            actionLayout->setSpacing(10);
            actionLayout->addStretch();

            QPushButton* editBtn = new QPushButton("Edit", this);
            editBtn->setCursor(Qt::PointingHandCursor);
            editBtn->setStyleSheet("QPushButton { color: white; background-color: #0275d8; border-radius: 3px; padding: 4px 8px; }"
                                   "QPushButton:hover { background-color: #025aa5; }");
            actionLayout->addWidget(editBtn);

            QPushButton* deleteBtn = new QPushButton("Delete", this);
            deleteBtn->setCursor(Qt::PointingHandCursor);
            deleteBtn->setStyleSheet("QPushButton { color: white; background-color: #d9534f; border-radius: 3px; padding: 4px 8px; }"
                                     "QPushButton:hover { background-color: #c9302c; }");
            actionLayout->addWidget(deleteBtn);

            expansionLayout->addWidget(actionButtonsWidget);

            connect(editBtn, &QPushButton::clicked, this, [this, act]() {
                if (!act) return;

                // 1. Nascondiamo temporaneamente la lista e il suo titolo a destra
                this->titleLabel->hide();
                activityList->hide();

                // 2. Istanziamo il guscio di modifica passandogli l'attività e il tagManager della classe
                auto* modifyWidget = new ActivityModify(act, tm, this);
                sideLayout->addWidget(modifyWidget);

                // 3. Quando l'utente finisce (clicca Salva o Annulla) ripristiniamo lo stato precedente
                connect(modifyWidget, &ActivityModify::modificationFinished, this, [=]() {
                    sideLayout->removeWidget(modifyWidget);
                    modifyWidget->deleteLater(); // Pulizia della memoria del widget di modifica

                    this->titleLabel->show();
                    activityList->show(); // Fa ricomparire la lista originale delle attività

                    // Sincronizza i dati aggiornati graficamente sia sul mese che sulle viste della MainWindow
                    updateCalendarView();
                    emit activityUpdated();
                });
            });

            connect(deleteBtn, &QPushButton::clicked, this, [this, act, qd]() {
                // Chiamata alla classe a parte ActivityDelete
                if (activityDelete.execute(act, this)) {
                    this->onDateChanged(qd);      // Rinfresca la lista del giorno corrente
                    this->updateCalendarView();   // Rinfresca il calendario grafico
                }
            });


            expansionWidget->setVisible(false);
            rowLayout->addWidget(expansionWidget);

            activityList->addItem(item);
            activityList->setItemWidget(item, rowContainer);

            item->setSizeHint(rowContainer->sizeHint());


            item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(expansionWidget)));
            item->setData(Qt::UserRole + 1, QVariant::fromValue(static_cast<void*>(rowContainer)));
        }
    }
}
void MonthWidget::updateCalendarView()
{

    calendar->setDateTextFormat(QDate(), QTextCharFormat());

    QTextCharFormat impegnatoFormat;
    impegnatoFormat.setForeground(Qt::blue);
    impegnatoFormat.setFontWeight(QFont::Bold);

    int year = calendar->yearShown();
    int month = calendar->monthShown();

    QDate firstDay(year, month, 1);
    int daysInMonth = firstDay.daysInMonth();

    for (int day = 1; day <= daysInMonth; ++day) {
        QDate qd(year, month, day);
        date backendDate(qd.day(), qd.month(), qd.year());

        auto activities = am.getOnDate(backendDate);

        if (!activities.empty()) {
            calendar->setDateTextFormat(qd, impegnatoFormat);
        } else {
            calendar->setDateTextFormat(qd, QTextCharFormat());
        }
    }

    QDate dataClick = calendar->selectedDate();


    this->onDateChanged(dataClick);
}