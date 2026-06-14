#include "Headers/Dialog/AddDialog.h"
#include "Headers/UI/Forms/EventForm.h"
#include "Headers/UI/Forms/ReminderForm.h"
#include "Headers/UI/Forms/RoutineForm.h"
#include "Headers/UI/Forms/TaskForm.h"
#include "Headers/UI/Forms/ProjectForm.h"

using namespace AddDialogIdx;

AddDialog::AddDialog(tagManager& tm, QWidget* parent)
    : QWidget(parent), tm(tm)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    setupToolbar();
    setupStack();
    setupActions();
}

void AddDialog::setupToolbar() {
    typeGroup = new QButtonGroup(this);
    typeGroup->setExclusive(true);

    btnEvent    = makeChip("Event");
    btnReminder = makeChip("Reminder");
    btnRoutine  = makeChip("Routine");
    btnTask     = makeChip("Task");
    btnProject  = makeChip("Project");

    typeGroup->addButton(btnEvent,    Event);
    typeGroup->addButton(btnReminder, Reminder);
    typeGroup->addButton(btnRoutine,  Routine);
    typeGroup->addButton(btnTask,     Task);
    typeGroup->addButton(btnProject,  Project);

    btnEvent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnReminder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnRoutine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnTask->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btnProject->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* row1Layout = new QHBoxLayout();
    row1Layout->setSpacing(6);
    row1Layout->addWidget(btnEvent);
    row1Layout->addWidget(btnReminder);
    row1Layout->addWidget(btnRoutine);

    QHBoxLayout* row2Layout = new QHBoxLayout();
    row2Layout->setSpacing(6);
    row2Layout->addWidget(btnTask);
    row2Layout->addWidget(btnProject);

    mainLayout->addLayout(row1Layout);
    mainLayout->addLayout(row2Layout);

    separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    btnEvent->setChecked(true);

    connect(typeGroup, &QButtonGroup::idClicked,
            this,      &AddDialog::onTypeSelected);
}

void AddDialog::setupStack() {
    eventForm    = new EventForm(tm, this);
    reminderForm = new ReminderForm(tm, this);
    routineForm  = new RoutineForm(tm, this);
    taskForm     = new TaskForm(tm, this);
    projectForm  = new ProjectForm(tm, this);

    stack = new QStackedWidget(this);
    stack->addWidget(eventForm);    // 0 — Event
    stack->addWidget(reminderForm); // 1 — Reminder
    stack->addWidget(routineForm);  // 2 — Routine
    stack->addWidget(taskForm);     // 3 — Task
    stack->addWidget(projectForm);  // 4 — Project

    stack->setCurrentIndex(Event);
    mainLayout->addWidget(stack);
}

void AddDialog::setupActions() {
    actionLayout = new QHBoxLayout();

    btnAdd    = new QPushButton("Add", this);
    btnCancel = new QPushButton("Cancel", this);

    actionLayout->addStretch();
    actionLayout->addWidget(btnCancel);
    actionLayout->addWidget(btnAdd);

    mainLayout->addLayout(actionLayout);

    connect(btnAdd, &QPushButton::clicked,
            this, &AddDialog::onAddClicked);

    connect(btnCancel, &QPushButton::clicked,
            this, &AddDialog::onCancelClicked);

}

QPushButton* AddDialog::makeChip(const QString& label) {
    auto* btn = new QPushButton(label, this);
    btn->setCheckable(true);
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

void AddDialog::onTypeSelected(int index) {
    stack->setCurrentIndex(index);
}

void AddDialog::onAddClicked() {
    ActivityForm* current =
        qobject_cast<ActivityForm*>(stack->currentWidget());

    if (!current || !current->validate())
        return;

    emit activityCreated(current->createActivity());
    static_cast<ActivityForm*>(stack->currentWidget())->reset();
}

void AddDialog::onCancelClicked()
{
    static_cast<ActivityForm*>(stack->currentWidget())->reset();
    emit activityCancelled();
}

AbstractActivity* AddDialog::createActivity() {
    ActivityForm* current = qobject_cast<ActivityForm*>(stack->currentWidget());
    if (!current) return nullptr;
    return current->createActivity();
}

void AddDialog::refreshTagCombo() {

    if (eventForm)    eventForm->refreshTags();
    if (reminderForm) reminderForm->refreshTags();
    if (taskForm)     taskForm->refreshTags();
    if (routineForm)  routineForm->refreshTags();
    if (projectForm)  projectForm->refreshTags();
}

