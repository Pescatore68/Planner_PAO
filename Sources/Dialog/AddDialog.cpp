#include "Headers/Dialog/AddDialog.h"
#include "Headers/UI/Forms/EventForm.h"
#include "Headers/UI/Forms/ReminderForm.h"
#include "Headers/UI/Forms/RoutineForm.h"
#include "Headers/UI/Forms/TaskForm.h"
#include "Headers/UI/Forms/ProjectForm.h"

using namespace AddDialogIdx;

// ─────────────────────────────────────────────────────────────────────────────
AddDialog::AddDialog(tagManager& tm, QWidget* parent)
    : QDialog(parent), tm(tm)
{
    setWindowTitle("Nuova attività");
    setMinimumWidth(440);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(8);

    setupToolbar();
    setupStack();
    setupActions();

    setLayout(mainLayout);
}

// ─── Toolbar con chip ─────────────────────────────────────────────────────────
void AddDialog::setupToolbar() {
    toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(6);

    typeGroup = new QButtonGroup(this);
    typeGroup->setExclusive(true);

    btnEvent    = makeChip("Event");
    btnReminder = makeChip("Reminder");
    btnRoutine  = makeChip("Routine");
    btnTask     = makeChip("Task");
    btnProject  = makeChip("Project");

    // id = indice stack
    typeGroup->addButton(btnEvent,    Event);
    typeGroup->addButton(btnReminder, Reminder);
    typeGroup->addButton(btnRoutine,  Routine);
    typeGroup->addButton(btnTask,     Task);
    typeGroup->addButton(btnProject,  Project);

    toolbarLayout->addWidget(btnEvent);
    toolbarLayout->addWidget(btnReminder);
    toolbarLayout->addWidget(btnRoutine);
    toolbarLayout->addWidget(btnTask);
    toolbarLayout->addWidget(btnProject);
    toolbarLayout->addStretch();

    mainLayout->addLayout(toolbarLayout);

    separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(separator);

    // seleziona Event di default
    btnEvent->setChecked(true);

    connect(typeGroup, &QButtonGroup::idClicked,
            this,      &AddDialog::onTypeSelected);
}

// ─── Stack dei form ───────────────────────────────────────────────────────────
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

// ─── Bottoni Aggiungi / Annulla ───────────────────────────────────────────────
void AddDialog::setupActions() {
    actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(8);

    btnAdd    = new QPushButton("Aggiungi", this);
    btnCancel = new QPushButton("Annulla",  this);

    actionLayout->addStretch();
    actionLayout->addWidget(btnCancel);
    actionLayout->addWidget(btnAdd);

    mainLayout->addLayout(actionLayout);

    connect(btnAdd,    &QPushButton::clicked, this, &AddDialog::onAddClicked);
    connect(btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

// ─── Helper chip ─────────────────────────────────────────────────────────────
QPushButton* AddDialog::makeChip(const QString& label) {
    auto* btn = new QPushButton(label, this);
    btn->setCheckable(true);
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

// ─── Slot: cambio tipo ────────────────────────────────────────────────────────
void AddDialog::onTypeSelected(int index) {
    stack->setCurrentIndex(index);
}

// ─── Slot: Aggiungi premuto — valida prima di chiudere ────────────────────────
void AddDialog::onAddClicked() {
    ActivityForm* current = qobject_cast<ActivityForm*>(stack->currentWidget());
    if (current && current->validate())
        accept();
    // se validate() fallisce il dialog rimane aperto (il warning lo mostra il form)
}

// ─── Restituisce l'attività creata dal form attivo ────────────────────────────
AbstractActivity* AddDialog::createActivity() {
    ActivityForm* current = qobject_cast<ActivityForm*>(stack->currentWidget());
    if (!current) return nullptr;
    return current->createActivity(tm);
}