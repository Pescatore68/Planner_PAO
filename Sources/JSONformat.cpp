#include "Headers/JSONformat.h"
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"
#include "Headers/task.h"
#include "Headers/routine.h"
#include "Headers/reminder.h"
#include "Headers/event.h"
#include "Headers/project.h"

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
    QJsonObject root;
    QJsonArray tagsArray;
    for (tag* t : tm.getTags()) {
            QJsonObject tagObj;
            tagObj["name"]  = QString::fromStdString(t->getName());
            tagObj["color"] = t->getColor().name();
            tagsArray.append(tagObj);
    }
    root["tags"] = tagsArray;

    QJsonArray arr;
    for (unsigned int i = 0; i < am.size(); i++)
        arr.append(am.get(i)->toJson());


    root["activities"] = arr;
    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool JSONformat::loadJson(ActivityManager& am, tagManager& tm, const std::string& path) {
    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::ReadOnly)) return false;
    const QJsonObject root = QJsonDocument::fromJson(f.readAll()).object();
    const QJsonArray arr   = root["activities"].toArray();
    for (const auto& val : arr) {
        AbstractActivity* a = fromJson(val.toObject(), tm);
        if (a) am.add(a);
    }
    return true;
}