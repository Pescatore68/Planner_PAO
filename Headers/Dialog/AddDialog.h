#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QFrame>

#include "Headers/tagManager.h"
#include "Headers/AbstractActivity.h"

class ActivityForm;
class EventForm;
class ReminderForm;
class RoutineForm;
class TaskForm;
class ProjectForm;

class AddDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddDialog(tagManager& tm, QWidget* parent = nullptr);

    // Chiamare solo dopo exec() == QDialog::Accepted.
    // Restituisce l'oggetto allocato; la ownership passa al chiamante (ActivityManager).
    AbstractActivity* createActivity();

private slots:
    void onTypeSelected(int index);
    void onAddClicked();

private:
    tagManager& tm;

    QVBoxLayout*    mainLayout;

    // ── Toolbar chip ──────────────────────────────────────────────────────
    QHBoxLayout*    toolbarLayout;
    QButtonGroup*   typeGroup;
    QPushButton*    btnEvent;
    QPushButton*    btnReminder;
    QPushButton*    btnRoutine;
    QPushButton*    btnTask;
    QPushButton*    btnProject;

    // ── Separatore ────────────────────────────────────────────────────────
    QFrame*         separator;

    // ── Form stack ────────────────────────────────────────────────────────
    QStackedWidget* stack;
    EventForm*      eventForm;
    ReminderForm*   reminderForm;
    RoutineForm*    routineForm;
    TaskForm*       taskForm;
    ProjectForm*    projectForm;

    // ── Bottoni azione ────────────────────────────────────────────────────
    QHBoxLayout*    actionLayout;
    QPushButton*    btnAdd;
    QPushButton*    btnCancel;

    // helpers
    QPushButton* makeChip(const QString& label);
    void setupToolbar();
    void setupStack();
    void setupActions();
};

// Indici stack — usati sia nell'header che nell'implementazione
namespace AddDialogIdx {
constexpr int Event    = 0;
constexpr int Reminder = 1;
constexpr int Routine  = 2;
constexpr int Task     = 3;
constexpr int Project  = 4;
}

#endif // ADDDIALOG_H
