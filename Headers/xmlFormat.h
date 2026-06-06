#ifndef XMLFORMAT_H
#define XMLFORMAT_H


#include <string>
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"


class xmlFormat{
public:
    // save activity in XML
    void save(const std::string& filepath, const ActivityManager& am, const tagManager& tm) const;

    // load activity from XML
    void load(const std::string& filepath, ActivityManager& am, tagManager& tm) const;
};

#endif // XMLFORMAT_H
