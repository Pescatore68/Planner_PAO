#include "Headers/Model/Persistence/JSONformat.h"
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include "Headers/Model/task.h"
#include "Headers/Model/routine.h"
#include "Headers/Model/reminder.h"
#include "Headers/Model/event.h"
#include "Headers/Model/project.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>

AbstractActivity* JSONformat::fromJson(const QJsonObject& obj, tagManager& tm) {
    std::string type = obj["type"].toString().toStdString();

    if (type == "task")     return task::fromJson(obj, tm);
    if (type == "routine")  return Routine::fromJson(obj, tm);
    if (type == "reminder") return Reminder::fromJson(obj, tm);
    if (type == "event")    return Event::fromJson(obj, tm);
    if (type == "project")  return project::fromJson(obj, tm);
    return nullptr;
}

bool JSONformat::saveJson(const ActivityManager& am, const tagManager& tm, const std::string& path) {

    QFileInfo fi(QString::fromStdString(path));
    QDir dir = fi.absoluteDir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;

    QJsonObject root;
    QJsonArray tagsArray;
    for (tag* t : tm.getTags()) {
        if (t) {
            QJsonObject tagObj;
            tagObj["name"]  = QString::fromStdString(t->getName());
            tagObj["color"] = t->getColor().name();
            tagsArray.append(tagObj);
        }
    }
    root["tags"] = tagsArray;

    QJsonArray arr;
    for (unsigned int i = 0; i < am.size(); i++) {
        if (am.get(i)) {
            arr.append(am.get(i)->toJson());
        }
    }
    root["activities"] = arr;

    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    f.close();
    return true;
}

bool JSONformat::loadJson(ActivityManager& am, tagManager& tm, const std::string& path) {
    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = f.readAll();
    f.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) return false;

    QJsonObject root = doc.object();

    if (root.contains("tags") && root["tags"].isArray()) {
        QJsonArray tagsArray = root["tags"].toArray();
        for (const auto& val : tagsArray) {
            QJsonObject tagObj = val.toObject();
            std::string name = tagObj["name"].toString().toStdString();
            QColor color(tagObj["color"].toString());
            tm.newTag(name, color);
        }

    }

    if (root.contains("activities") && root["activities"].isArray()) {
        QJsonArray arr = root["activities"].toArray();
        for (const auto& val : arr) {
            AbstractActivity* a = fromJson(val.toObject(), tm);
            if (a) {
                am.add(a);
            }
        }
    }


    return true;
}