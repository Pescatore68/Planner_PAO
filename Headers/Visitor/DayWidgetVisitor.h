#ifndef DAYWIDGETVISITOR_H
#define DAYWIDGETVISITOR_H

#include "ActivityVisitor.h"
#include "../date.h"

class Event;
class Reminder;
class Routine;
class task;
class project;

//check if activity is selected all day
class AllDayCheckVisitor : public ActivityVisitor {
public:
    bool isAllDay = false;

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

//return start and end time for pos on grid
class MinutesExtractorVisitor : public ActivityVisitor {
private:
    const date& targetDate;

public:
    int startMinutes = 0;
    int endMinutes = 30;

    explicit MinutesExtractorVisitor(const date& d);

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

//filter task and project
class TypeCheckVisitor : public ActivityVisitor {
public:
    bool valid = true;

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

#endif // DAYWIDGETVISITOR_H
