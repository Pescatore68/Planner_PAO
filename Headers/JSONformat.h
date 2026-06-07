#ifndef JSONFORMAT_H
#define JSONFORMAT_H
#include <string>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <fstream>

class AbstractActivity;
class ActivityManager;
class tagManager;

namespace JSONformat {
static AbstractActivity* fromJson(const QJsonObject&, tagManager& );
bool loadJson(ActivityManager&, tagManager& , const std::string& );
bool saveJson(const ActivityManager&, const tagManager&, const std::string&);
}

#endif // JSONFORMAT_H
