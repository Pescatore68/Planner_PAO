#ifndef TASKWIDGETVISITOR_H
#define TASKWIDGETVISITOR_H

#include "ActivityVisitor.h"
#include <QTreeWidgetItem>

//UserRole save pointer to the activity and to the parent
namespace TaskWidgetRoles {
constexpr int ActivityPtrRole = Qt::UserRole;
constexpr int ParentProjectRole = Qt::UserRole + 1;
}

class TreeBuilderVisitor : public ActivityVisitor {
private:
    QTreeWidgetItem* rootTasks;
    QTreeWidgetItem* rootProjects;

public:
    TreeBuilderVisitor(QTreeWidgetItem* tasksRoot, QTreeWidgetItem* projectsRoot);
    virtual ~TreeBuilderVisitor() = default;

    void visit(task& t) override;
    void visit(project& p) override;

    //not used
    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& r) override;
};



class ItemChangedVisitor : public ActivityVisitor { //for checked activity
private:
    QTreeWidgetItem* item;

public:
    explicit ItemChangedVisitor(QTreeWidgetItem* treeItem);
    virtual ~ItemChangedVisitor() = default;

    void visit(task& t) override;
    void visit(project& p) override;

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& r) override;
};


#endif // TASKWIDGETVISITOR_H
