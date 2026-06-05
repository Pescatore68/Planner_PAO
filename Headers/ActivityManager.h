#ifndef ACTIVITYMANAGER_H
#define ACTIVITYMANAGER_H

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
    AbstractActivity* get(unsigned int);
    unsigned int size();
    void fit();

};

#endif // ACTIVITYMANAGER_H
