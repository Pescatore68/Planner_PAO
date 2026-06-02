#ifndef TASK_H
#define TASK_H
#include <string>
#include "Headers/AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/orario.h"
#include "Headers/tag.h"

class task : public AbstractActivity {
private:
    date deadline;
    orario oDeadline;
    bool check;
public:
    task(std::string n, date e, orario oe, tag t);
    task(std::string n, date e, orario oe);
    ~task();
    date getDeadline() const;
    orario getODeadline() const;
    bool isCompleted() const;
    void setCompleted (bool b);
    void setDeadline (date e);
    void setODeadline (orario oe);
};

#endif // TASK_H
