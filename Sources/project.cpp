#include "Headers/ActivityVisitor.h"
#include <Headers/project.h>

//metodo per barra di completamento
unsigned int project::nCompleted() const {
    unsigned int i=0;
    for(task* t : subtasks) {
        if (t->isCompleted())
            i++;
    }
    return i;
};

void project::add(const string& name, const string& description, const tag* Tag, const date& end, const HourMinute& oEnd) {
    subtasks.push_back(new task(name, description, Tag, end, oEnd));
}
void project::add(const string& name, const string& description, const date& end, const HourMinute& oEnd) {
    subtasks.push_back(new task(name, description, end, oEnd));
}

unsigned int project::size() const {
    return subtasks.size();
}

void project::remove(task* rm) {
    for(std::vector<task*>::iterator it=subtasks.begin(); it!=subtasks.end(); ++it) {
        if (*it==rm) {
            delete *it;
            subtasks.erase(it);
            return;
        }
    }
}

void project::remove(unsigned int idx) {
    delete subtasks[idx];
    subtasks.erase(subtasks.begin()+idx);
}

//string project::summary() const;

float project::completionPercentage() const {
    if (size() == 0) return 0.0f;                        // evita divisione per zero
    return (float)nCompleted() / size() * 100.0f;        // cast a float prima della divisione
}

void project::accept(ActivityVisitor& v) { v.visit(*this); }
