#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

#include "Headers/Model/date.h"
class tagManager;

#include "AbstractActivity.h"

class ActivityManager {
private:
    std::vector<AbstractActivity*> activities;

public:
    ActivityManager() = default;
    ~ActivityManager();
    void add(AbstractActivity*);
    void remove(unsigned int);
    AbstractActivity* get(unsigned int) const;
    unsigned int size() const;
    void fit();
    //activity on a specific day
    std::vector<AbstractActivity*> getOnDate(const date& d) const;
};

#endif // ACTIVITYMANAGER_H
