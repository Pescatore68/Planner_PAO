#ifndef DAYWIDGETVISITOR_H
#define DAYWIDGETVISITOR_H

#include "ActivityVisitor.h"
#include "../date.h"

// Forward declarations
class Event;
class Reminder;
class Routine;
class task;
class project;

// ─── VISITOR 1: Identifica se un'attività occupa l'intera giornata ───────────
class AllDayCheckVisitor : public ActivityVisitor {
public:
    bool isAllDay = false;

    // 🔥 Ora usano i riferimenti (&) senza const per fare l'override corretto
    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

// ─── VISITOR 2: Estrae i minuti di inizio e fine per il posizionamento ───────
class MinutesExtractorVisitor : public ActivityVisitor {
private:
    const date& targetDate;

public:
    int startMinutes = 0;
    int endMinutes = 30;

    explicit MinutesExtractorVisitor(const date& d);

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

// ─── VISITOR 3: Filtra le attività da escludere (task e project) ─────────────
class TypeCheckVisitor : public ActivityVisitor {
public:
    bool valid = true;

    void visit(Event& e) override;
    void visit(Reminder& r) override;
    void visit(Routine& ro) override;
    void visit(task& t) override;
    void visit(project& p) override;
};

#endif // DAYWIDGETVISITOR_H
