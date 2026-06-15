#ifndef ACTIVITYSEARCH_H
#define ACTIVITYSEARCH_H

#include <vector>
#include <string>
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/AbstractActivity.h"

class ActivitySearch {
public:
    static std::vector<AbstractActivity*> findByName(const ActivityManager& am, const std::string& query);
    static std::vector<AbstractActivity*> findByTag(const ActivityManager& am, const std::string& tagName);

};
#endif // ACTIVITYSEARCH_H
