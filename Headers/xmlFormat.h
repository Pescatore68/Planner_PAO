#ifndef XMLFORMAT_H
#define XMLFORMAT_H

#include <string>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QTextStream>

class AbstractActivity;
class ActivityManager;
class tagManager;

namespace xmlFormat {
    static AbstractActivity* fromXml(const QDomElement& obj, tagManager& tm);
    bool loadXml(ActivityManager& am, tagManager& tm, const std::string& path);
    bool saveXml(const ActivityManager& am, const tagManager& tm, const std::string& path);
}

#endif // XMLFORMAT_H