#ifndef PROJECTFORM_H
#define PROJECTFORM_H
#include "Headers/UI/Forms/ActivityForm.h"
#include "Headers/UI/Forms/TaskForm.h"
#include <QDateEdit>
#include <QTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFrame>

class ProjectForm : public ActivityForm {
    Q_OBJECT
public:
    explicit ProjectForm(tagManager& tm, QWidget* parent = nullptr);
    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;

private:
    // deadline progetto
    QDateEdit* deadlineEdit;
    QTimeEdit* oDeadlineEdit;

    // subtask
    QPushButton*        btnAddSubtask;
    QFrame*             subtaskPanel;   // pannello che appare/scompare
    QVBoxLayout*        subtaskLayout;  // lista dei TaskForm
    QList<TaskForm*>    subtaskForms;
    tagManager*         tmPtr;          // serve per creare nuovi TaskForm
    QScrollArea* scroll = nullptr;

    void setupSubtaskPanel();
    void addSubtaskForm();
};
#endif
