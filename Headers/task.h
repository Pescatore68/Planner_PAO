#ifndef TASK_H
#define TASK_H
#include <string>
#include "Headers/AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/orario.h"
#include "Headers/tag.h"

class task : virtual public AbstractActivity {
private:
    date starts;
    date ends;
    orario oStarts;
    orario oEnds;
    bool check;
public:
    task(std::string n, date s, date e, orario os, orario oe, tag t);
    task(std::string n, date s, date e, orario os, orario oe);
    ~task();
    std::string getName() const override ;
    date getStarts() const;
    date getEnds() const;
    orario getOStarts() const;
    orario getOEnds() const;
    bool isCompleted() const;
    void setCompleted (bool b);
    void setName(std::string n);
    void setStarts(date s);
    void setEnds(date e);
    void setOStarts(orario os);
    void setOEnds(orario oe);
};

#endif // TASK_H
