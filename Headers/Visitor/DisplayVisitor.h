#ifndef DISPLAYVISITOR_H
#define DISPLAYVISITOR_H

#include "Headers/Visitor/ActivityVisitor.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include <string>
#include <vector>

class DisplayVisitor : public ActivityVisitor {
private:
    std::string textSummary;
    bool isRoutineType;
    bool hasCheckableStatus;
    bool isChecked;

    // Variabili interne per gestire la modalità "Scrittura"
    bool isWriteMode;
    bool newValueToSet;

public:
    // Il costruttore di base lavora in modalità LETTURA (passiva)
    DisplayVisitor()
        : textSummary(""), isRoutineType(false), hasCheckableStatus(false),
        isChecked(false), isWriteMode(false), newValueToSet(false) {}

    // Funzione speciale da chiamare prima di fare .accept() se vogliamo SCRIVERE (cambiare il check)
    void setWriteMode(bool value) {
        isWriteMode = true;
        newValueToSet = value;
    }

    // Getter per la GUI
    std::string getSummary() const { return textSummary; }
    bool isRoutine() const { return isRoutineType; }
    bool isCheckable() const { return hasCheckableStatus; }
    bool getCheckedState() const { return isChecked; }

    // ─── IMPLEMENTAZIONE DEI VISIT CON DOPPIA LOGICA ─────────────────────

    void visit(Event& e) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        // Gli eventi non si spuntano, quindi ignoriamo la modalità scrittura

        textSummary = e.getName() + " — " + e.getDescription() + "\n";
        textSummary += e.getStartDate().toString();
        if (e.hasTime())
            textSummary += " " + e.getStartTime().toString() + "–" + e.getEndTime().toString();
        else
            textSummary += " → " + e.getEndDate().toString();
        if (!e.getLocation().empty())
            textSummary += " | " + e.getLocation();
    }

    void visit(Reminder& r) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        // I reminder non si spuntano

        textSummary = r.getName() + " — " + r.getDescription() + "\n";
        textSummary += r.getDate().toString() + " " + r.getTime().toString();
        if (!r.getLocation().empty())
            textSummary += " | " + r.getLocation();
    }

    void visit(task& t) override {
        isRoutineType = false;
        hasCheckableStatus = true;

        // Se siamo in modalità scrittura, modifichiamo l'oggetto reale!
        if (isWriteMode) {
            t.setCompleted(newValueToSet);
        }

        isChecked = t.isCompleted();

        std::string status = isChecked ? "[✓]" : "[ ]";
        textSummary = status + " " + t.getName() + "\n";
        if (!t.getDescription().empty())
            textSummary += "  " + t.getDescription() + "\n";
        textSummary += "  Scadenza: " + t.getDeadline().toString() + " ore " + t.getODeadline().toString() + "\n";
    }

    void visit(project& p) override {
        isRoutineType = false;
        hasCheckableStatus = true;

        // Se siamo in modalità scrittura, modifichiamo il progetto
        if (isWriteMode) {
            p.setCompleted(newValueToSet);
        }

        isChecked = p.isCompleted();

        std::string status = isChecked ? "[✓]" : "[ ]";
        textSummary = status + " " + p.getName() + "\n";
        if (!p.getDescription().empty())
            textSummary += "  " + p.getDescription() + "\n";
        textSummary += "  Scadenza: " + p.getDeadline().toString() + " ore " + p.getODeadline().toString() + "\n";

        for (unsigned int i = 0; i < p.size(); i++) {
            const task* t = p.getSubtask(i);
            std::string subStatus = t->isCompleted() ? "[✓]" : "[ ]";
            textSummary += "    " + subStatus + " " + t->getName() + "\n";
        }
    }

    void visit(Routine& r) override {
        isRoutineType = true;
        hasCheckableStatus = true;

        // Se siamo in modalità scrittura, modifichiamo la routine
        if (isWriteMode) {
            r.setCheck(newValueToSet);
        }

        isChecked = r.getCheck();

        textSummary = r.getName() + " — " + r.FrequencyToString() + " - " + r.getDescription() + "\n";
        textSummary += r.getStartTime().toString() + "–" + r.getEndTime().toString();
        textSummary += " | oggi: ";
        textSummary += isChecked ? "✓" : "○";

        const auto& history = r.getcheckHistory();
        if (!history.empty()) {
            textSummary += " | ";
            for (bool b : history) {
                textSummary += b ? "●" : "○";
            }
        }
    }
};

#endif // DISPLAYVISITOR_H