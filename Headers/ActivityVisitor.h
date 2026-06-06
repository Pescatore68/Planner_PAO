#ifndef ACTIVITYVISITOR_H
#define ACTIVITYVISITOR_H

class Event;
class Reminder;
class Routine;
class task;
class project;

class ActivityVisitor {
public:
    virtual ~ActivityVisitor() = default;
    virtual void visit(Event& e) = 0;
    virtual void visit(Reminder& r) = 0;
    virtual void visit(Routine& r) = 0;
    virtual void visit(task& t) = 0;
    virtual void visit(project& p) = 0;

};

#endif