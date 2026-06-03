#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"

class project : public task {
private:
    std::vector<task*> subtasks;

public:
    unsigned int nCompleted() const;
    void add(const string&, const string&, const tag*, const date&, const HourMinute&);
    void add(const string&, const string&, const date&, const HourMinute&);
    unsigned int size() const;
    void remove(task*);
    void remove(unsigned int i);
    string summary() const override;
    float completionPercentage() const; // nCompleted() / size() * 100 → per la barra

};

#endif // PROJECT_H
