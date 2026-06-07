#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "ActivityVisitor.h"

class DisplayVisitor : public ActivityVisitor{
public:
    virtual void visit(const task&) = 0;
    virtual void visit(const project&) = 0;
    virtual void visit(const Event&) = 0;
    virtual void visit(const Reminder&) = 0;
    virtual void visit(const Routine&) = 0;
    virtual ~DisplayVisitor() = default;

};

#endif // DISPLAYVISITOR_H
