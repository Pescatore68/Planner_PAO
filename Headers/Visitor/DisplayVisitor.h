#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
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

    void processTag(const tag* t) {
        if (t && !t->getName().empty()) {
            tagText = " " + t->getName() + " ";
            tagColor = t->getColor();
        } else {
            tagColor = QColor();
        }
    }

    int getOffset(const date& start) const {
        QDate s(start.getYear(), start.getMonth(), start.getDay());
        QDate c(currentViewDate.getYear(), currentViewDate.getMonth(), currentViewDate.getDay());
        return s.daysTo(c);
    }

public:
    DisplayVisitor() : textSummary(""), tagText(""), isRoutineType(false), hasCheckableStatus(false), isChecked(false), isWriteMode(false), newValueToSet(false), currentViewDate(date::today()) {}

    void setViewDate(const date& d) { currentViewDate = d; }

    void setWriteMode(bool value) {
        isWriteMode = true;
        newValueToSet = value;
    }

    std::string getSummary() const { return textSummary; }
    bool isRoutine() const { return isRoutineType; }
    bool isCheckable() const { return hasCheckableStatus; }
    bool getCheckedState() const { return isChecked; }
    QColor getTagColor() const { return tagColor; }

    void applyToLayout(QVBoxLayout* targetLayout, QWidget* parent) const {
        if (!targetLayout) return;

        QLabel* summaryLabel = new QLabel(QString::fromStdString(textSummary), parent);
        summaryLabel->setWordWrap(true);
        summaryLabel->setStyleSheet(
            "QLabel {"
            "  color: #2c3e50;"
            "  background-color: #ffffff;"
            "  padding: 12px 14px;"
            "  border-radius: 8px;"
            "  border: 1px solid #e2e8f0;"
            "  font-size: 13px;"
            "  line-height: 1.4;"
            "}"
            );
        targetLayout->addWidget(summaryLabel);

        if (!tagText.empty() && tagColor.isValid()) {
            QWidget* tagContainer = new QWidget(parent);
            QHBoxLayout* tagLayout = new QHBoxLayout(tagContainer);
            tagLayout->setContentsMargins(2, 4, 2, 4);
            tagLayout->setSpacing(0);

            QLabel* tagLabel = new QLabel(QString::fromStdString(tagText), parent);
            QFont tagFont = tagLabel->font();
            tagFont.setPixelSize(11);
            tagFont.setBold(true);
            tagLabel->setFont(tagFont);

            QString baseColor = tagColor.name();
            QColor translucentBg = tagColor;
            translucentBg.setAlpha(30);
            QString bgColor = QString("rgba(%1, %2, %3, 0.12)")
                                  .arg(translucentBg.red())
                                  .arg(translucentBg.green())
                                  .arg(translucentBg.blue());

            tagLabel->setStyleSheet(QString(
                                        "QLabel {"
                                        "  color: %1;"
                                        "  background-color: %2;"
                                        "  padding: 4px 8px;"
                                        "  border-radius: 6px;"
                                        "  border: 1px solid rgba(%3, %4, %5, 0.25);"
                                        "}"
                                        ).arg(baseColor).arg(bgColor).arg(tagColor.red()).arg(tagColor.green()).arg(tagColor.blue()));

            tagLayout->addWidget(tagLabel);
            tagLayout->addStretch();
            targetLayout->addWidget(tagContainer);
        }
    }

    void visit(Event& e) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        textSummary = "[EVENT] " + e.getName();
        if (!e.getDescription().empty()) {
            textSummary += "\n" + e.getDescription();
        }

        if (e.hasTime()) {
            textSummary += "\nDate: " + e.getStartDate().toString() + "-" + e.getEndDate().toString() + " Time: " + e.getStartTime().toString() + " - " + e.getEndTime().toString();
        } else {
            textSummary += "\nDate: " + e.getStartDate().toString() + "-" + e.getEndDate().toString();
        }

        if (!e.getLocation().empty())
            textSummary += "\nLocation: " + e.getLocation();
        processTag(e.getTag());
    }

    void visit(Reminder& r) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        textSummary = "[REMINDER] " + r.getName();
        if (!r.getDescription().empty()) {
            textSummary += "\n" + r.getDescription();
        }
        textSummary += "\nDate: " + r.getDate().toString() + " Time: " + r.getTime().toString();
        if (!r.getLocation().empty())
            textSummary += "\nLocation: " + r.getLocation();
        processTag(r.getTag());
    }

    void visit(task& t) override {
        isRoutineType = false;
        hasCheckableStatus = true;

        if (isWriteMode) {
            t.setCompleted(newValueToSet);
        }

        isChecked = t.isCompleted();
        std::string status = isChecked ? "[X]" : "[ ]";

        textSummary = status + " " + t.getName();
        if (!t.getDescription().empty())
            textSummary += "\n" + t.getDescription();
        textSummary += "\nDate: " + t.getDeadline().toString() + " Time: " + t.getODeadline().toString();
        processTag(t.getTag());
    }

    void visit(project& p) override {
        isRoutineType = false;
        hasCheckableStatus = true;

        if (isWriteMode) {
            p.setCompleted(newValueToSet);
        }

        isChecked = p.isCompleted();
        std::string status = isChecked ? "[PROJECT COMPLETED]" : "[PROJECT IN PROGRESS]";

        textSummary = status + " " + p.getName();
        if (!p.getDescription().empty())
            textSummary += "\n" + p.getDescription();
        textSummary += "\nDate: " + p.getDeadline().toString() + " Time: " + p.getODeadline().toString();

        if (p.size() > 0) {
            textSummary += "\nSub-tasks:";
            for (unsigned int i = 0; i < p.size(); i++) {
                const task* t = p.getSubtask(i);
                std::string subStatus = t->isCompleted() ? "- [X] " : "- [ ] ";
                textSummary += "\n" + subStatus + t->getName();
            }
        }
        processTag(p.getTag());
    }

    void visit(Routine& r) override {
        isRoutineType = true;
        hasCheckableStatus = true;

        int offset = getOffset(r.getStartDate());
        std::vector<bool> history = r.getcheckHistory();

        if (isWriteMode) {
            if (offset >= 0) {
                if (offset >= (int)history.size()) history.resize(offset + 1, false);
                history[offset] = newValueToSet;
                r.setCheckHistory(history);
            }
        }

        isChecked = (offset >= 0 && offset < (int)history.size()) ? history[offset] : false;

        textSummary = r.getName() + " — " + r.FrequencyToString() + " - " + r.getDescription() + "\n";

        textSummary += r.getStartTime().toString() + "–" + r.getEndTime().toString() + "\n";

        textSummary += "Status: " + std::string(isChecked ? "✓" : "○");

        if (!history.empty()) {
            textSummary += "\nHistory: ";
            QDate startDate(r.getStartDate().getYear(), r.getStartDate().getMonth(), r.getStartDate().getDay());
            int count = 0;

            for (int i = 0; i < (int)history.size(); ++i) {
                QDate currentDate = startDate.addDays(i);
                date currentBackendDate(currentDate.day(), currentDate.month(), currentDate.year());

                if (r.isActive(currentBackendDate)) {
                    if (count > 0 && count % 7 == 0) textSummary += "\n";
                    textSummary += history[i] ? "●" : "○";
                    count++;
                }
            }
        }
        processTag(r.getTag());
    }
};

#endif // DISPLAYVISITOR_H