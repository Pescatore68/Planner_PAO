#ifndef ACTIVITYSEARCH_H
#define ACTIVITYSEARCH_H

#include <vector>
#include <string>
#include "Headers/ActivityManager.h"
#include "Headers/AbstractActivity.h"

class ActivitySearch {
public:
    static std::vector<AbstractActivity*> findByName(const ActivityManager& am, const std::string& query);
};
#endif // ACTIVITYSEARCH_H
