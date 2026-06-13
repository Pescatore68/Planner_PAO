#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>

#include "Headers/ActivityManager.h"
#include "Headers/AbstractActivity.h"

class task;
class project;

class TaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskWidget(ActivityManager& am, QWidget* parent = nullptr);

    void refresh();

signals:
    void activitySelected(AbstractActivity* a);
    void deleteRequested(AbstractActivity* a);

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onDeleteClicked();

private:
    ActivityManager& am;

    QVBoxLayout* mainLayout;
    QHBoxLayout* toolbarLayout;
    QTreeWidget* tree;

    QPushButton* btnDelete;

    AbstractActivity* current = nullptr;

    void buildTree();
    void addTaskItem(QTreeWidgetItem* parent, const task* t, AbstractActivity* owner);
    void addProjectItem(QTreeWidget* tree, project* p, unsigned int amIndex);

    AbstractActivity* activityFromItem(QTreeWidgetItem* item) const;
};

#endif // TASKWIDGET_H