#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/Model/event.h"
#include "Headers/Model/reminder.h"
#include "Headers/Model/routine.h"
#include "Headers/Model/task.h"
#include "Headers/Model/project.h"
#include "qboxlayout.h"
#include "qlabel.h"
#include <string>
#include <vector>

class DisplayVisitor : public ActivityVisitor {
private:
    std::string textSummary;
    std::string tagText;
    bool isRoutineType;
    bool hasCheckableStatus;
    bool isChecked;
    QColor tagColor;
    bool isWriteMode;
    bool newValueToSet;
    date currentViewDate;

    void processTag(const tag* t);
    int getOffset(const date& start) const;

public:
    DisplayVisitor();

    void setViewDate(const date& d);
    void setWriteMode(bool value);

    std::string getSummary() const;
    bool isRoutine() const;
    bool isCheckable() const;
    bool getCheckedState() const;
    QColor getTagColor() const;

    void applyToLayout(QVBoxLayout* targetLayout, QWidget* parent) const;

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(task& t) override;
    void visit(project& p) override;
    void visit(Routine& r) override;
};

#endif // DISPLAYVISITOR_H