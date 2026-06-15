#ifndef ADDWIDGET_H
#define ADDWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QFrame>

#include "Headers/Model/tagManager.h"
#include "Headers/Model/AbstractActivity.h"

class ActivityForm;
class EventForm;
class ReminderForm;
class RoutineForm;
class TaskForm;
class ProjectForm;

class AddDialog : public QWidget {
    Q_OBJECT

public:
    explicit AddDialog(tagManager& tm, QWidget* parent = nullptr);

    AbstractActivity* createActivity();
    void refreshTagCombo();

signals:
    void activityCreated(AbstractActivity* a);
    void activityCancelled();

private slots:
    void onTypeSelected(int index);
    void onAddClicked();
    void onCancelClicked();

private:
    tagManager& tm;

    QVBoxLayout* mainLayout;

    // toolbar
    QHBoxLayout* toolbarLayout;
    QButtonGroup* typeGroup;
    QPushButton* btnEvent;
    QPushButton* btnReminder;
    QPushButton* btnRoutine;
    QPushButton* btnTask;
    QPushButton* btnProject;

    QFrame* separator;

    // stack forms
    QStackedWidget* stack;
    EventForm* eventForm;
    ReminderForm* reminderForm;
    RoutineForm* routineForm;
    TaskForm* taskForm;
    ProjectForm* projectForm;

    // actions
    QHBoxLayout* actionLayout;
    QPushButton* btnAdd;
    QPushButton* btnCancel;

    QPushButton* makeChip(const QString& label);
    void setupToolbar();
    void setupStack();
    void setupActions();
};

namespace AddDialogIdx {
const int Event    = 0;
const int Reminder = 1;
const int Routine  = 2;
const int Task     = 3;
const int Project  = 4;
}

#endif // ADDWIDGET_H
