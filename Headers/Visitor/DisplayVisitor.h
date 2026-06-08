#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H


#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include <iostream>

class DisplayVisitor : public ActivityVisitor {
public:
    void visit(Event& e)    override { std::cout << e.summary()    << "\n"; }
    void visit(Reminder& r) override { std::cout << r.summary()    << "\n"; }
    void visit(Routine& r)  override { std::cout << r.summary()    << "\n"; }
    void visit(task& t)     override { std::cout << t.summary()    << "\n"; }
    void visit(project& p)  override { std::cout << p.summary()    << "\n"; }
};

#endif // DISPLAYVISITOR_H
