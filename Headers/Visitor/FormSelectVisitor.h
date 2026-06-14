#ifndef FORMSELECTVISITOR_H
#define FORMSELECTVISITOR_H

#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/UI/Forms/ActivityForm.h"
#include "Headers/ActivityManager.h"

class FormSelectVisitor : public ActivityVisitor {
public:
    ActivityForm* chosenForm = nullptr;
    tagManager& tm;
    QWidget* parent;

    FormSelectVisitor(tagManager& tm, QWidget* parent);
    void visit(Event&) override;
    void visit(task&) override;
    void visit(Routine&) override;
    void visit(Reminder&) override;
    void visit(project&) override;
};

#endif // FORMSELECTVISITOR_H
