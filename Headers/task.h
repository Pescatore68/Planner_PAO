#ifndef TASK_H
#define TASK_H
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
    task(const string&, const string&, const tag*, date, orario);
    task(const string&, const string&, date, orario);
    ~task();
    date getDeadline() const;
    orario getODeadline() const;
    bool isCompleted() const;
    void setCompleted (bool b);
    void setDeadline (date e);
    void setODeadline (orario oe);
    bool isExpired() const override;
    string summary() const override;
};

#endif // TASK_H
