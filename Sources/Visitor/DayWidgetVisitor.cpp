#include "Headers/Visitor/DayWidgetVisitor.h"
#include "Headers/Model/event.h"
#include "Headers/Model/reminder.h"
#include "Headers/Model/routine.h"
#include "Headers/Model/task.h"
#include "Headers/Model/project.h"

void AllDayCheckVisitor::visit(Event& e) {
    isAllDay = !e.hasTime();
}

void AllDayCheckVisitor::visit(Reminder&) { isAllDay = false; }
void AllDayCheckVisitor::visit(Routine&)  { isAllDay = false; }
void AllDayCheckVisitor::visit(task&)     { isAllDay = false; }
void AllDayCheckVisitor::visit(project&)  { isAllDay = false; }

MinutesExtractorVisitor::MinutesExtractorVisitor(const date& d)
    : targetDate(d), startMinutes(0), endMinutes(30) {}

void MinutesExtractorVisitor::visit(Event& e) {
    startMinutes = (int)e.getStartTime().getOre() * 60 + (int)e.getStartTime().getMin();
    endMinutes   = (int)e.getEndTime().getOre()   * 60 + (int)e.getEndTime().getMin();

    if (!e.hasTime()) {
        startMinutes = 0;
        endMinutes = 24 * 60;
        return;
    }

    if (!(e.getStartDate() == targetDate) && !(e.getEndDate() == targetDate)) {
        startMinutes = 0;
        endMinutes = 24 * 60;
    } else if (!(e.getStartDate() == targetDate)) {
        startMinutes = 0;
        endMinutes = (int)e.getEndTime().getOre() * 60 + (int)e.getEndTime().getMin();
    } else if (!(e.getEndDate() == targetDate)) {
        startMinutes = (int)e.getStartTime().getOre() * 60 + (int)e.getStartTime().getMin();
        endMinutes = 24 * 60;
    }

    if (endMinutes <= startMinutes) endMinutes = startMinutes + 30;
}

void MinutesExtractorVisitor::visit(Reminder& r) {
    startMinutes = (int)r.getTime().getOre() * 60 + (int)r.getTime().getMin();
    endMinutes   = startMinutes + 30;
}

void MinutesExtractorVisitor::visit(Routine& ro) {
    startMinutes = (int)ro.getStartTime().getOre() * 60 + (int)ro.getStartTime().getMin();
    endMinutes   = (int)ro.getEndTime().getOre()   * 60 + (int)ro.getEndTime().getMin();
    if (endMinutes <= startMinutes) endMinutes = startMinutes + 30;
}

void MinutesExtractorVisitor::visit(task&) {}
void MinutesExtractorVisitor::visit(project&) {}

void TypeCheckVisitor::visit(Event&)    { valid = true; }
void TypeCheckVisitor::visit(Reminder&) { valid = true; }
void TypeCheckVisitor::visit(Routine&)  { valid = true; }
void TypeCheckVisitor::visit(task&)     { valid = false; }
void TypeCheckVisitor::visit(project&)  { valid = false; }
