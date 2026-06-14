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

class ProjectForm : public TaskForm {
    Q_OBJECT
public:
    explicit ProjectForm(tagManager& tm, QWidget* parent = nullptr);
    AbstractActivity* createActivity() override;
    bool validate() override;
    void reset() override;
    const QList<TaskForm*>& getSubtaskForms() const { return subtaskForms; }
    void triggerAddSubtaskForm();

    void loadFromActivity(AbstractActivity* act) override;
    void saveToActivity(AbstractActivity* act) override;

private:
    // subtask
    QPushButton*        btnAddSubtask;
    QFrame*             subtaskPanel;
    QVBoxLayout*        subtaskLayout;
    QList<TaskForm*>    subtaskForms;
    tagManager*         tmPtr;
    QScrollArea* scroll = nullptr;

    void setupSubtaskPanel();
    void addSubtaskForm();
};
#endif
