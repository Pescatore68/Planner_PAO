#include "Headers/UI/month.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/date.h"
#include "Headers/AbstractActivity.h"
#include "qcheckbox.h"
#include "qtextformat.h"

MonthWidget::MonthWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am)
{
    setup();
}

void MonthWidget::setup()
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);


    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar, 2);

    QWidget* sidePanel = new QWidget(this);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* titleLabel = new QLabel("Attività del giorno:", this);

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

    // Connessione al CLICK della riga per mostrare i dettagli col Visitor
    connect(activityList, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        if (!item) return;

        // Recuperiamo i puntatori grafici che abbiamo salvato nell'item
        QWidget* expansionWidget = static_cast<QWidget*>(item->data(Qt::UserRole).value<void*>());
        QWidget* rowContainer = static_cast<QWidget*>(item->data(Qt::UserRole + 1).value<void*>());

        if (expansionWidget && rowContainer) {
            // Invertiamo lo stato di visibilità: se era aperto si chiude, se era chiuso si apre
            bool isCurrentlyVisible = expansionWidget->isVisible();
            expansionWidget->setVisible(!isCurrentlyVisible);

            // Chiediamo a Qt di ricalcolare immediatamente lo spazio occupato dal widget modificato
            rowContainer->adjustSize();

            // AGGIORNAMENTO CRITICO: Diciamo alla QListWidget di aggiornare la dimensione della riga
            // altrimenti il widget si espanderebbe ma verrebbe tagliato visivamente!
            item->setSizeHint(rowContainer->sizeHint());

            // Forza il ridisegno grafico immediato della lista
            activityList->update();
        }
    });
}
void MonthWidget::onDateChanged(const QDate& qd)
{
    activityList->clear();

    date backendDate(qd.day(), qd.month(), qd.year());
    auto activities = am.getOnDate(backendDate);

    for (AbstractActivity* act : activities) {
        if (!act) continue;

        QListWidgetItem* item = new QListWidgetItem(activityList);

        QWidget* rowContainer = new QWidget(this);
        QVBoxLayout* rowLayout = new QVBoxLayout(rowContainer);
        rowLayout->setContentsMargins(5, 5, 5, 5);
        rowLayout->setSpacing(0);

        // Intestazione
        QLabel* titleLabel = new QLabel("• " + QString::fromStdString(act->getName()), this);
        titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 13px; padding: 4px; }");
        rowLayout->addWidget(titleLabel);

        // AREA DETTAGLI
        QWidget* expansionWidget = new QWidget(this);
        QVBoxLayout* expansionLayout = new QVBoxLayout(expansionWidget);
        expansionLayout->setContentsMargins(15, 5, 5, 5);
        expansionLayout->setSpacing(5);

        DisplayVisitor visitor;
        act->accept(visitor);

        // Label del Summary (La mettiamo dentro un puntatore così possiamo aggiornarla al click del check!)
        QLabel* summaryLabel = new QLabel(QString::fromStdString(visitor.getSummary()), this);
        summaryLabel->setWordWrap(true);
        summaryLabel->setStyleSheet("QLabel { color: #555; background-color: #fcfcfc; padding: 5px; border-radius: 4px; }");
        expansionLayout->addWidget(summaryLabel);

        // PROBLEMA 1 RISOLTO: Se l'attività è checkabile (Task, Project, Routine), mostra la checkbox
        if (visitor.isCheckable()) {
            QCheckBox* statusCheck = new QCheckBox("Completata", this);
            statusCheck->setChecked(visitor.getCheckedState());
            expansionLayout->addWidget(statusCheck);

            // Salvataggio polimorfico del check quando viene cliccato
            connect(statusCheck, &QCheckBox::toggled, this, [act, summaryLabel, this](bool checked) {
                // Proviamo a vedere se è un Task/Project
                task* t = dynamic_cast<task*>(act);
                if (t) {
                    t->setCompleted(checked);
                } else {
                    // Altrimenti è una Routine
                    Routine* r = dynamic_cast<Routine*>(act);
                    if (r) {
                        r->setCheck(checked);
                    }
                }

                // Aggiorna il testo del summary al volo per mostrare [✓] o ● senza dover ricaricare tutto
                DisplayVisitor v;
                act->accept(v);
                summaryLabel->setText(QString::fromStdString(v.getSummary()));

                // Rinfresca i quadratini del calendario (se un task completato cambia colore)
                this->updateCalendarView();
            });
        }



        expansionWidget->setVisible(false);
        rowLayout->addWidget(expansionWidget);

        activityList->addItem(item);
        activityList->setItemWidget(item, rowContainer);

        item->setSizeHint(rowContainer->sizeHint());

        // Salviamo i riferimenti per il click alternato (Accordion)
        item->setData(Qt::UserRole, QVariant::fromValue(static_cast<void*>(expansionWidget)));
        item->setData(Qt::UserRole + 1, QVariant::fromValue(static_cast<void*>(rowContainer)));
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