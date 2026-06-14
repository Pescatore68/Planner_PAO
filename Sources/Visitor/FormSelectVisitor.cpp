#include "Headers/Visitor/FormSelectVisitor.h"
#include "Headers/UI/Forms/EventForm.h"
#include "Headers/UI/Forms/TaskForm.h"
#include "Headers/UI/Forms/RoutineForm.h"
#include "Headers/UI/Forms/ReminderForm.h"
#include "Headers/UI/Forms/ProjectForm.h"
#include "Headers/event.h"
#include "Headers/task.h"
#include "Headers/routine.h"
#include "Headers/reminder.h"
#include "Headers/project.h"


FormSelectVisitor::FormSelectVisitor(tagManager& tm, QWidget* parent) : tm(tm), parent(parent) {}
void FormSelectVisitor::visit(Event&)    { chosenForm = new EventForm(tm, parent); }
void FormSelectVisitor::visit(task&)     { chosenForm = new TaskForm(tm, parent); }
void FormSelectVisitor::visit(Routine&)  { chosenForm = new RoutineForm(tm, parent); }
void FormSelectVisitor::visit(Reminder&) { chosenForm = new ReminderForm(tm, parent); }
void FormSelectVisitor::visit(project&)  { chosenForm = new ProjectForm(tm, parent); }
