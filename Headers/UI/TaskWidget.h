
#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QLabel>

#include "Headers/ActivityManager.h"
#include "Headers/task.h"
#include "Headers/project.h"

class AbstractActivity;

class TaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskWidget(ActivityManager& am, QWidget* parent = nullptr);

    // ricarica l'albero da ActivityManager (da chiamare dopo add/remove/complete)
    void refresh();

signals:
    void activitySelected(AbstractActivity* a);   // utente ha cliccato su una riga
    void deleteRequested(AbstractActivity* a);    // utente ha chiesto di eliminare
    void newTaskRequested();                      // utente vuole creare un task
    void newProjectRequested();                   // utente vuole creare un project

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onItemChanged(QTreeWidgetItem* item, int column); // checkbox spuntata
    void onDeleteClicked();
    void onNewTaskClicked();
    void onNewProjectClicked();

private:
    ActivityManager& am;

    // UI
    QVBoxLayout*  mainLayout;
    QHBoxLayout*  toolbarLayout;
    QTreeWidget*  tree;
    QPushButton*  btnNewTask;
    QPushButton*  btnNewProject;
    QPushButton*  btnDelete;

    // tiene traccia dell'activity selezionata correntemente
    AbstractActivity* current = nullptr;

    // helpers
    void buildTree();
    void addTaskItem   (QTreeWidgetItem* parent, const task* t,    AbstractActivity* owner);
    void addProjectItem(QTreeWidget*     tree,   project* p,       unsigned int amIndex);

    // converte un QTreeWidgetItem → puntatore all'activity originale
    AbstractActivity* activityFromItem(QTreeWidgetItem* item) const;
};

#endif // TASKWIDGET_H
