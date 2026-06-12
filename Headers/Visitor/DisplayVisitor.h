#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include <string>
#include <vector>

class DisplayVisitor : public ActivityVisitor {
private:
    std::string textSummary;
    bool isRoutineType;
    bool hasCheckableStatus;
    bool isChecked;
    std::vector<bool> routineHistory;

public:
    DisplayVisitor() : textSummary(""), isRoutineType(false), hasCheckableStatus(false), isChecked(false) {}

    std::string getSummary() const { return textSummary; }
    bool isRoutine() const { return isRoutineType; }
    bool isCheckable() const { return hasCheckableStatus; }
    bool getCheckedState() const { return isChecked; }
    std::vector<bool> getRoutineHistory() const { return routineHistory; }

    void visit(Event& e) override {
        textSummary = e.summary();
        isRoutineType = false;
        hasCheckableStatus = false;
    }
    void visit(Reminder& r) override {
        textSummary = r.summary();
        isRoutineType = false;
        hasCheckableStatus = false;
    }
    void visit(task& t) override {
        textSummary = t.summary();
        isRoutineType = false;
        hasCheckableStatus = true;
        isChecked = t.isCompleted();
    }
    void visit(project& p) override {
        textSummary = p.summary();
        isRoutineType = false;
        hasCheckableStatus = true;
        isChecked = p.isCompleted(); // Eredita da task
    }
    void visit(Routine& r) override {
        textSummary = r.summary();
        isRoutineType = true;
        hasCheckableStatus = true;
        isChecked = r.getCheck();
        routineHistory = r.getcheckHistory();
    }
};

#endif // DISPLAYVISITOR_H