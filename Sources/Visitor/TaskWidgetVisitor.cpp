#include "Headers/Visitor/TaskWidgetVisitor.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include "Headers/UI/TaskWidget.h"
#include <QColor>
#include <QVariant>

TreeBuilderVisitor::TreeBuilderVisitor(QTreeWidgetItem* tasksRoot, QTreeWidgetItem* projectsRoot)
    : rootTasks(tasksRoot), rootProjects(projectsRoot) {}

void TreeBuilderVisitor::visit(project& p) {
    auto* projItem = new QTreeWidgetItem(rootProjects);
    projItem->setText(0, QString::fromStdString(p.getName()));
    projItem->setText(1, QString::fromStdString(p.getDeadline().toString()));

    //CompletionPercentage has type float
    int pct = static_cast<int>(p.completionPercentage());
    //create progress bar with complitionPercentege
    projItem->setData(0, ProgressBarDelegate::ProgressRole, pct);
    projItem->setCheckState(0, p.isCompleted() ? Qt::Checked : Qt::Unchecked);
    //create project
    projItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                      QVariant::fromValue(static_cast<void*>(&p)));

    //create subtasks
    for (unsigned int j = 0; j < p.size(); ++j) {
        const task* sub = p.getSubtask(j);
        if (!sub) continue;

        auto* subItem = new QTreeWidgetItem(projItem);
        subItem->setText(0, QString::fromStdString(sub->getName()));
        subItem->setText(1, QString::fromStdString(sub->getDeadline().toString()));

        subItem->setData(0, ProgressBarDelegate::ProgressRole, -1); // No progress bar
        subItem->setCheckState(0, sub->isCompleted() ? Qt::Checked : Qt::Unchecked);

        subItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                         QVariant::fromValue(static_cast<void*>(const_cast<task*>(sub))));
        subItem->setData(0, TaskWidgetRoles::ParentProjectRole,
                         QVariant::fromValue(static_cast<void*>(&p)));

        if (sub->isCompleted()) { //set color if completed
            subItem->setForeground(0, QColor(150, 150, 150));
            subItem->setForeground(1, QColor(150, 150, 150));
        }
    }
    projItem->setExpanded(true);
}

void TreeBuilderVisitor::visit(task& t) {
    auto* taskItem = new QTreeWidgetItem(rootTasks);
    taskItem->setText(0, QString::fromStdString(t.getName()));
    taskItem->setText(1, QString::fromStdString(t.getDeadline().toString()));

    taskItem->setData(0, ProgressBarDelegate::ProgressRole, -1); // No progress bar
    taskItem->setCheckState(0, t.isCompleted() ? Qt::Checked : Qt::Unchecked);

    taskItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                      QVariant::fromValue(static_cast<void*>(&t)));

    if (t.isExpired() && !t.isCompleted()) {
        taskItem->setForeground(0, QColor(200, 50, 50));
        taskItem->setForeground(1, QColor(200, 50, 50));
    }
    if (t.isCompleted()) {
        taskItem->setForeground(0, QColor(150, 150, 150));
        taskItem->setForeground(1, QColor(150, 150, 150));
    }
}

void TreeBuilderVisitor::visit(Event&) {}
void TreeBuilderVisitor::visit(Reminder&) {}
void TreeBuilderVisitor::visit(Routine&) {}



ItemChangedVisitor::ItemChangedVisitor(QTreeWidgetItem* treeItem)
    : item(treeItem) {}

void ItemChangedVisitor::visit(task& t) {
    if (!item) return;

    bool checked = (item->checkState(0) == Qt::Checked);
    t.setCompleted(checked);

    QVariant vp = item->data(0, TaskWidgetRoles::ParentProjectRole); //check if is a subtask
    if (vp.isValid()) {
        project* p = static_cast<project*>(vp.value<void*>());
        QTreeWidgetItem* parentItem = item->parent();

        if (p && parentItem) {
            if (item->treeWidget())
                item->treeWidget()->blockSignals(true);
            int pct = static_cast<int>(p->completionPercentage());
            parentItem->setData(0, ProgressBarDelegate::ProgressRole, pct);

            //check parent completionPercentage to check or uncheck parent
            parentItem->setCheckState(0, p->completionPercentage()==100 ? Qt::Checked : Qt::Unchecked);

            if (item->treeWidget())
                item->treeWidget()->blockSignals(false);
        }
    }

    QColor color = checked ? QColor(150, 150, 150) : QColor();
    item->setForeground(0, color);
    item->setForeground(1, color);
}

void ItemChangedVisitor::visit(project& p) {
    if (!item) return;

    bool checked = (item->checkState(0) == Qt::Checked);

    if (item->treeWidget())
        item->treeWidget()->blockSignals(true);

    p.setCompleted(checked);


    //if project checked all subtasks checked
    int childCount = item->childCount();
    for (int i = 0; i < childCount; ++i) {
        QTreeWidgetItem* childItem = item->child(i);
        if (!childItem) continue;

        childItem->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);

        QVariant v = childItem->data(0, TaskWidgetRoles::ActivityPtrRole);
        if (v.isValid()) {
            task* sub = static_cast<task*>(v.value<void*>());
            if (sub) sub->setCompleted(checked);
        }

        QColor color = checked ? QColor(150, 150, 150) : QColor();
        childItem->setForeground(0, color);
        childItem->setForeground(1, color);
    }

    item->setData(0, ProgressBarDelegate::ProgressRole, checked ? 100 : 0);

    if (item->treeWidget())
        item->treeWidget()->blockSignals(false);
}


void ItemChangedVisitor::visit(Event&) {}
void ItemChangedVisitor::visit(Reminder&) {}
void ItemChangedVisitor::visit(Routine&) {}