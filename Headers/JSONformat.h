#ifndef JSONFORMAT_H
#define JSONFORMAT_H
#include <string>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <fstream>

class ActivityManager;
class tagManager;

namespace JSONformat {
QJsonDocument toJson(const ActivityManager&);
void fromJson(const QJsonDocument& doc, ActivityManager&, tagManager& tm);
bool save(const ActivityManager&, const std::string& path);
bool load(ActivityManager&, tagManager&, const std::string& path);
}

#endif // JSONFORMAT_H
