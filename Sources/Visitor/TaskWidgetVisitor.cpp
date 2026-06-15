#include "Headers/Visitor/TaskWidgetVisitor.h"
#include "Headers/Model/task.h"
#include "Headers/Model/project.h"
#include "Headers/UI/Widgets/TaskWidget.h"
#include <QColor>
#include <QVariant>

TreeBuilderVisitor::TreeBuilderVisitor(QTreeWidgetItem* tasksRoot, QTreeWidgetItem* projectsRoot)
    : rootTasks(tasksRoot), rootProjects(projectsRoot) {}

void TreeBuilderVisitor::visit(project& p) {
    auto* projItem = new QTreeWidgetItem(rootProjects);
    projItem->setText(0, QString::fromStdString(p.getName()));
    projItem->setText(1, QString::fromStdString(p.getDeadline().toString()));

    int pct = static_cast<int>(p.completionPercentage());
    projItem->setData(0, ProgressBarDelegate::ProgressRole, pct);
    QColor tagColor(200, 200, 200); // Fallback standard se il progetto non ha un tag
    if (p.getTag()) {
        tagColor = p.getTag()->getColor();
    }
    projItem->setData(0, ProgressBarDelegate::TagColorRole, tagColor);
    projItem->setCheckState(0, p.isCompleted() ? Qt::Checked : Qt::Unchecked);
    projItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                      QVariant::fromValue(static_cast<void*>(&p)));

    if (p.isCompleted()) {
        projItem->setForeground(0, QColor(150, 150, 150));
        projItem->setForeground(1, QColor(150, 150, 150));
    } else if (p.isExpired()) {
        projItem->setForeground(0, QColor(200, 50, 50));
        projItem->setForeground(1, QColor(200, 50, 50));
    }

    for (unsigned int j = 0; j < p.size(); ++j) {
        const task* sub = p.getSubtask(j);
        if (!sub) continue;

        auto* subItem = new QTreeWidgetItem(projItem);
        subItem->setText(0, QString::fromStdString(sub->getName()));
        subItem->setText(1, QString::fromStdString(sub->getDeadline().toString()));

        subItem->setData(0, ProgressBarDelegate::ProgressRole, -1);
        subItem->setCheckState(0, sub->isCompleted() ? Qt::Checked : Qt::Unchecked);

        subItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                         QVariant::fromValue(static_cast<void*>(const_cast<task*>(sub))));
        subItem->setData(0, TaskWidgetRoles::ParentProjectRole,
                         QVariant::fromValue(static_cast<void*>(&p)));

        if (sub->isCompleted()) {
            subItem->setForeground(0, QColor(150, 150, 150));
            subItem->setForeground(1, QColor(150, 150, 150));
        } else if (sub->isExpired()) {
            subItem->setForeground(0, QColor(200, 50, 50));
            subItem->setForeground(1, QColor(200, 50, 50));
        }
    }
    projItem->setExpanded(true);
}

void TreeBuilderVisitor::visit(task& t) {
    auto* taskItem = new QTreeWidgetItem(rootTasks);
    taskItem->setText(0, QString::fromStdString(t.getName()));
    taskItem->setText(1, QString::fromStdString(t.getDeadline().toString()));

    taskItem->setData(0, ProgressBarDelegate::ProgressRole, -1);
    taskItem->setCheckState(0, t.isCompleted() ? Qt::Checked : Qt::Unchecked);

    taskItem->setData(0, TaskWidgetRoles::ActivityPtrRole,
                      QVariant::fromValue(static_cast<void*>(&t)));

    if (t.isCompleted()) {
        taskItem->setForeground(0, QColor(150, 150, 150));
        taskItem->setForeground(1, QColor(150, 150, 150));
    } else if (t.isExpired()) {
        taskItem->setForeground(0, QColor(200, 50, 50));
        taskItem->setForeground(1, QColor(200, 50, 50));
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

    QVariant vp = item->data(0, TaskWidgetRoles::ParentProjectRole);
    if (vp.isValid()) {
        project* p = static_cast<project*>(vp.value<void*>());
        QTreeWidgetItem* parentItem = item->parent();

        if (p && parentItem) {
            if (item->treeWidget())
                item->treeWidget()->blockSignals(true);

            int pct = static_cast<int>(p->completionPercentage());
            parentItem->setData(0, ProgressBarDelegate::ProgressRole, pct);

            bool parentCompleted = (pct == 100);
            parentItem->setCheckState(0, parentCompleted ? Qt::Checked : Qt::Unchecked);

            QColor parentColor;
            if (parentCompleted) {
                parentColor = QColor(150, 150, 150);
            } else if (p->isExpired()) {
                parentColor = QColor(200, 50, 50);
            }

            parentItem->setForeground(0, parentColor);
            parentItem->setForeground(1, parentColor);

            if (item->treeWidget())
                item->treeWidget()->blockSignals(false);
        }
    }

    QColor color;
    if (checked) {
        color = QColor(150, 150, 150);
    } else if (t.isExpired()) {
        color = QColor(200, 50, 50);
    }
    item->setForeground(0, color);
    item->setForeground(1, color);
}

void ItemChangedVisitor::visit(project& p) {
    if (!item) return;

    bool checked = (item->checkState(0) == Qt::Checked);

    if (item->treeWidget())
        item->treeWidget()->blockSignals(true);

    p.setCompleted(checked);

    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* childItem = item->child(i);
        if (!childItem) continue;

        childItem->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);

        QVariant v = childItem->data(0, TaskWidgetRoles::ActivityPtrRole);
        if (v.isValid()) {
            task* sub = static_cast<task*>(v.value<void*>());
            if (sub) {
                sub->setCompleted(checked);

                QColor childColor;
                if (checked) {
                    childColor = QColor(150, 150, 150);
                } else if (sub->isExpired()) {
                    childColor = QColor(200, 50, 50);
                }
                childItem->setForeground(0, childColor);
                childItem->setForeground(1, childColor);
            }
        }
    }

    item->setData(0, ProgressBarDelegate::ProgressRole, checked ? 100 : 0);

    QColor color;
    if (checked) {
        color = QColor(150, 150, 150);
    } else if (p.isExpired()) {
        color = QColor(200, 50, 50);
    }
    item->setForeground(0, color);
    item->setForeground(1, color);

    if (item->treeWidget())
        item->treeWidget()->blockSignals(false);
}

void ItemChangedVisitor::visit(Event&) {}
void ItemChangedVisitor::visit(Reminder&) {}
void ItemChangedVisitor::visit(Routine&) {}