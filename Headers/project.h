#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"

class project : public task {
private:
    std::vector<task*> subtasks;

public:
    //metodo per barra di completamento
    unsigned int nCompleted();
    //add
    void add(const string&, const string&, const tag*, const date&, const HourMinute&);
    void add(const string&, const string&, const date&, const HourMinute&);
    //size
    unsigned int size();
    //remove
    void remove(task*);
    void remove(unsigned int i);

    bool isExpired() const override;
    string summary() const override;
    float completionPercentage() const; // nCompleted() / size() * 100 → per la barra

};

#endif // PROJECT_H
