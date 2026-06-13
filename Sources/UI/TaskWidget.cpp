#include "Headers/UI/TaskWidget.h"
#include "Headers/AbstractActivity.h"
#include "Headers/task.h"
#include "Headers/project.h"

#include <QHeaderView>
#include <QMessageBox>

// Salviamo il puntatore all'AbstractActivity direttamente nell'item
// così non dobbiamo cercare per indice ogni volta.
static constexpr int ActivityPtrRole = Qt::UserRole;
// Per i subtask salviamo anche il puntatore al project padre
static constexpr int ParentProjectRole = Qt::UserRole + 1;


TaskWidget::TaskWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am), activityDelete(am)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    //toolbar
    toolbarLayout = new QHBoxLayout();
    btnDelete     = new QPushButton("Elimina",   this);

    btnDelete->setEnabled(false); //disabled if no task selected

    toolbarLayout->addStretch();
    toolbarLayout->addWidget(btnDelete);

    //tree
    tree = new QTreeWidget(this);
    tree->setColumnCount(3);
    tree->setHeaderLabels({"Nome", "Scadenza"});
    tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    tree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    //tree->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setAnimated(true);

    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(tree);

    //connections
    connect(btnDelete,     &QPushButton::clicked, this, &TaskWidget::onDeleteClicked);

    connect(tree, &QTreeWidget::itemClicked,
            this, &TaskWidget::onItemClicked);

    //itemChanged for checked or unchecked task
    connect(tree, &QTreeWidget::itemChanged,
            this, &TaskWidget::onItemChanged);

    buildTree();
}


void TaskWidget::refresh() {
    //itemChanged temporary blocked while refreshing
    tree->blockSignals(true);
    buildTree();
    tree->blockSignals(false);

    current = nullptr;
    btnDelete->setEnabled(false);
}

void TaskWidget::buildTree() {
    tree->clear();

    //root node separated for Task and Project
    auto* rootTasks    = new QTreeWidgetItem(tree, {"Task",     ""});
    auto* rootProjects = new QTreeWidgetItem(tree, {"Project", ""});

    // stile grassetto per le sezioni
    QFont boldFont = rootTasks->font(0);
    boldFont.setBold(true);
    rootTasks->setFont(0, boldFont);
    rootProjects->setFont(0, boldFont);

    // rende le sezioni non selezionabili
    rootTasks->setFlags(Qt::ItemIsEnabled);
    rootProjects->setFlags(Qt::ItemIsEnabled);

    for (unsigned int i = 0; i < am.size(); ++i) {
        AbstractActivity* a = am.get(i);

        if (project* p = dynamic_cast<project*>(a)) {
            auto* projItem = new QTreeWidgetItem(rootProjects);
            projItem->setText(0, QString::fromStdString(p->getName()));
            projItem->setText(1, QString::fromStdString(p->getDeadline().toString()));

            // barra di completamento testuale nella colonna Stato
            float pct = p->completionPercentage();
            projItem->setText(2, QString("%1%").arg(static_cast<int>(pct)));

            // checkbox sul progetto stesso (completato = tutti i subtask fatti)
            projItem->setCheckState(0, p->isCompleted() ? Qt::Checked : Qt::Unchecked);

            // salva il puntatore
            projItem->setData(0, ActivityPtrRole, QVariant::fromValue(static_cast<void*>(p)));

            // aggiungi i subtask come figli
            for (unsigned int j = 0; j < p->size(); ++j) {
                const task* sub = p->getSubtask(j);
                auto* subItem = new QTreeWidgetItem(projItem);
                subItem->setText(0, QString::fromStdString(sub->getName()));
                subItem->setText(1, QString::fromStdString(sub->getDeadline().toString()));
                subItem->setText(2, sub->isCompleted() ? "✓" : "");

                // checkbox interattiva sul subtask
                subItem->setCheckState(0, sub->isCompleted() ? Qt::Checked : Qt::Unchecked);

                // salva puntatori (subtask e project padre)
                subItem->setData(0, ActivityPtrRole,    QVariant::fromValue(static_cast<void*>(const_cast<task*>(sub))));
                subItem->setData(0, ParentProjectRole,  QVariant::fromValue(static_cast<void*>(p)));

                // subtask completati in grigio
                if (sub->isCompleted()) {
                    subItem->setForeground(0, QColor(150, 150, 150));
                    subItem->setForeground(1, QColor(150, 150, 150));
                }
            }
            projItem->setExpanded(true);

        } else if (task* t = dynamic_cast<task*>(a)) {
            // task semplice
            auto* taskItem = new QTreeWidgetItem(rootTasks);
            taskItem->setText(0, QString::fromStdString(t->getName()));
            taskItem->setText(1, QString::fromStdString(t->getDeadline().toString()));
            taskItem->setText(2, t->isCompleted() ? "✓" : "");

            // checkbox interattiva
            taskItem->setCheckState(0, t->isCompleted() ? Qt::Checked : Qt::Unchecked);

            // salva il puntatore
            taskItem->setData(0, ActivityPtrRole, QVariant::fromValue(static_cast<void*>(t)));

            // task scaduto → testo rosso
            if (t->isExpired() && !t->isCompleted()) {
                taskItem->setForeground(0, QColor(200, 50, 50));
                taskItem->setForeground(1, QColor(200, 50, 50));
            }
            // task completato → grigio
            if (t->isCompleted()) {
                taskItem->setForeground(0, QColor(150, 150, 150));
                taskItem->setForeground(1, QColor(150, 150, 150));
            }
        }
        // Event, Reminder, Routine vengono ignorati automaticamente
    }

    rootTasks->setExpanded(true);
    rootProjects->setExpanded(true);
}


AbstractActivity* TaskWidget::activityFromItem(QTreeWidgetItem* item) const {
    if (!item) return nullptr;
    QVariant v = item->data(0, ActivityPtrRole);
    if (!v.isValid()) return nullptr;
    return static_cast<AbstractActivity*>(v.value<void*>());
}


void TaskWidget::onItemClicked(QTreeWidgetItem* item, int) {
    AbstractActivity* a = activityFromItem(item);
    current = a;
    btnDelete->setEnabled(a != nullptr);
    if (a)
        emit activitySelected(a);
}

void TaskWidget::onItemChanged(QTreeWidgetItem* item, int column) {
    if (column != 0) return;   // ci interessa solo la colonna con il checkbox

    AbstractActivity* a = activityFromItem(item);
    if (!a) return;

    task* t = dynamic_cast<task*>(a);
    if (!t) return;

    bool checked = (item->checkState(0) == Qt::Checked);
    t->setCompleted(checked);

    // se è un subtask, aggiorna la percentuale del project padre
    QVariant vp = item->data(0, ParentProjectRole);
    if (vp.isValid()) {
        project* p = static_cast<project*>(vp.value<void*>());
        if (p && item->parent()) {
            float pct = p->completionPercentage();
            item->parent()->setText(2, QString("%1%").arg(static_cast<int>(pct)));
            item->parent()->setCheckState(0, p->isCompleted() ? Qt::Checked : Qt::Unchecked);
        }
    }

    // aggiorna il colore della riga
    QColor color = checked ? QColor(150, 150, 150) : QColor();
    item->setForeground(0, color);
    item->setForeground(1, color);
    item->setText(2, checked ? "✓" : "");
    emit activityUpdated();
}

void TaskWidget::onDeleteClicked() {
    if (!current) return;

    AbstractActivity* toDelete = current;
    if (activityDelete.execute(toDelete, this)) {

        current = nullptr;
        btnDelete->setEnabled(false);

        // Chiamiamo il refresh interno per ridisegnare l'albero senza l'elemento
        this->refresh();
    }
}
