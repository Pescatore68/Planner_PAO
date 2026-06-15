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
    // Variabili interne per gestire la modalità "Scrittura"
    bool isWriteMode;
    bool newValueToSet;
    date currentViewDate;

    void processTag(const tag* t) {
        if (t && !t->getName().empty()) {
            tagText = "@" + t->getName();
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

        // 1. Label per il testo dell'attività (Testo scuro standard)
        QLabel* summaryLabel = new QLabel(QString::fromStdString(textSummary), parent);
        summaryLabel->setWordWrap(true);
        summaryLabel->setStyleSheet("QLabel { color: #555; background-color: #fcfcfc; padding: 5px; border-radius: 4px; }");
        targetLayout->addWidget(summaryLabel);

        // 2. Se c'è un tag, il Visitor crea una riga dedicata sotto con il colore specifico del tag
        if (!tagText.empty() && tagColor.isValid()) {
            QWidget* tagContainer = new QWidget(parent);
            QHBoxLayout* tagLayout = new QHBoxLayout(tagContainer);
            tagLayout->setContentsMargins(5, 2, 5, 2);
            tagLayout->setSpacing(0);

            QLabel* sepLabel = new QLabel(" | ", parent);
            sepLabel->setStyleSheet("QLabel { color: #888; }");
            tagLayout->addWidget(sepLabel);

            QLabel* tagLabel = new QLabel(QString::fromStdString(tagText), parent);

            QFont tagFont = tagLabel->font();
            tagFont.setBold(true);
            tagLabel->setFont(tagFont);

            // Applichiamo il colore in modo forzato con Style Sheet usando il colore estratto
            QString hexColor = tagColor.name();
            tagLabel->setStyleSheet(QString("QLabel { color: %1; }").arg(hexColor));

            tagLayout->addWidget(tagLabel);
            tagLayout->addStretch();

            targetLayout->addWidget(tagContainer);
        }
    }

    void visit(Event& e) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        textSummary = e.getName() + " — " + e.getDescription() + "\n";
        textSummary += e.getStartDate().toString();
        if (e.hasTime())
            textSummary += " " + e.getStartTime().toString() + "–" + e.getEndTime().toString();
        else
            textSummary += " → " + e.getEndDate().toString();
        if (!e.getLocation().empty())
            textSummary += " | " + e.getLocation();
        processTag(e.getTag());
    }

    void visit(Reminder& r) override {
        isRoutineType = false;
        hasCheckableStatus = false;
        textSummary = r.getName() + " — " + r.getDescription() + "\n";
        textSummary += r.getDate().toString() + " " + r.getTime().toString();
        if (!r.getLocation().empty())
            textSummary += " | " + r.getLocation();
        processTag(r.getTag());
    }

    void visit(task& t) override {
        isRoutineType = false;
        hasCheckableStatus = true;

        if (isWriteMode) {
            t.setCompleted(newValueToSet);
        }

        isChecked = t.isCompleted();

        std::string status = isChecked ? "[✓]" : "[ ]";
        textSummary = status + " " + t.getName() + "\n";
        if (!t.getDescription().empty())
            textSummary += "  " + t.getDescription() + "\n";
        textSummary += "  Scadenza: " + t.getDeadline().toString() + " ore " + t.getODeadline().toString() + "\n";
        processTag(t.getTag());
    }

    void visit(project& p) override {
        isRoutineType = false;
        hasCheckableStatus = true;

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
        processTag(p.getTag());
    }

    void visit(Routine& r) override {
        isRoutineType = true;
        hasCheckableStatus = true;

        int offset = getOffset(r.getStartDate());
        std::vector<bool> history = r.getcheckHistory(); // Copia per lavorarci

        // 1. Se siamo in modalità scrittura, aggiorniamo il vettore
        if (isWriteMode) {
            if (offset >= 0) {
                if (offset >= (int)history.size()) history.resize(offset + 1, false);
                history[offset] = newValueToSet;
                r.setCheckHistory(history); // Salva nel modello
            }
        }

        // 2. Leggiamo lo stato aggiornato (o quello originale se non siamo in scrittura)
        if (offset >= 0 && offset < (int)history.size()) {
            isChecked = history[offset];
        } else {
            isChecked = false;
        }

        // 3. Ricostruzione completa della stringa summary (fondamentale!)
        textSummary = r.getName() + " — " + r.FrequencyToString() + " - " + r.getDescription() + "\n";
        textSummary += r.getStartTime().toString() + "–" + r.getEndTime().toString();
        textSummary += " | oggi: ";
        textSummary += isChecked ? "✓" : "○";

        if (!history.empty()) {
            textSummary += "\nPast: ";

            // Calcoliamo la data di inizio come QDate per gestire i giorni della settimana
            QDate startDate(r.getStartDate().getYear(), r.getStartDate().getMonth(), r.getStartDate().getDay());

            int count = 0;
            for (int i = 0; i < (int)history.size(); ++i) {
                QDate currentDate = startDate.addDays(i);
                date currentBackendDate(currentDate.day(), currentDate.month(), currentDate.year());
                // FILTRO FREQUENZA:
                // Esempio: se è una Routine settimanale (es. frequenza 1 = Weekly)
                // dovresti controllare se il giorno è coerente con la Routine
                bool isDayActive = true;

                // Se la tua Routine ha un metodo tipo getActiveDays(), usalo qui:
                if (!r.isActive(currentBackendDate)) isDayActive = false;

                if (isDayActive) {
                    // A capo ogni 7 giorni attivi
                    if (count > 0 && count % 7 == 0) {
                        textSummary += "\n                  ";
                    }
                    textSummary += history[i] ? "●" : "○";
                    count++;
                }
            }
        }
        processTag(r.getTag());
    }
};

#endif // DISPLAYVISITOR_H