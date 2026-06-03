#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"

class project : public task {
private:
    std::vector<task*> p;

public:
    //metodo per barra di completamento
    unsigned int nCompleted();
    //add
    void add(string n, date e, orario oe, tag t);
    void add(string n, date e, orario oe);
    //size
    unsigned int size();
    //remove
    void remove(const tag& t);
    void remove(unsigned int i);
    bool isExpired() const override;
    string summary() const override;
    float completionPercentage() const; // nCompleted() / size() * 100 → per la barra

};

#endif // PROJECT_H
