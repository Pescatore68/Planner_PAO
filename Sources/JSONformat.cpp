#include "Headers/JSONformat.h"
#include "Headers/JSONformat.h"
#include "Headers/tagManager.h"
#include "Headers/task.h"
#include "Headers/routine.h"
#include "Headers/reminder.h"
#include "Headers/event.h"
#include "Headers/project.h"



static date dateFromString(const std::string& s) {
    return date(std::stoi(s.substr(0,2)),
                std::stoi(s.substr(3,2)),
                std::stoi(s.substr(6,4)));
}

static HourMinute hmFromString(const std::string& s) {
    return HourMinute(std::stoi(s.substr(0,2)),
                      std::stoi(s.substr(3,2)));
}

static Routine::Frequency freqFromString(const std::string& s) {
    if (s == "Daily")   return Routine::Frequency::Daily;
    if (s == "Weekly")  return Routine::Frequency::Weekly;
    if (s == "Monthly") return Routine::Frequency::Monthly;
    if (s == "Yearly")  return Routine::Frequency::Yearly;
    return Routine::Frequency::Daily;
}

QJsonDocument JSONformat::toJson(const JSONformat& am) {
    QJsonArray arr;

    for (unsigned int i=0; i<am.size(); i++) {
        AbstractActivity* a = am.get(i);
        QJsonObject obj;
        obj["name"] = QString::fromStdString(a->getName());
        obj["description"] = QString::fromStdString(a->getDescription());
        obj["tag"] = QString::fromStdString(a->getTag()->getName()); //->getName per const tag*

        if (const auto* p = dynamic_cast<const project*>(a)) {
            obj["type"] = "project";
            obj["deadline"] = QString::fromStdString(p->getDeadline().toString());
            obj["oDeadline"] = QString::fromStdString(p->getODeadline().toString());
            obj["check"] = p->isCompleted();
            //std::vector<task*> subtasks;+
            QJsonArray subtasks;
            for (unsigned int i=0; i<p->size(); i++) {
                QJsonObject sub;
                const task* t = p->getSubtask(i);
                sub["taskNum"] = QString::fromStdString(std::to_string(i));
                sub["deadline"] = QString::fromStdString(t->getDeadline().toString());
                sub["oDeadline"] = QString::fromStdString(t->getODeadline().toString());
                sub["check"] = t->isCompleted();
            }
        }

        else if (const auto* t = dynamic_cast<const task*>(a)) {
            obj["type"] = "task";
            obj["deadline"] = QString::fromStdString(t->getDeadline().toString());
            obj["oDeadline"] = QString::fromStdString(t->getODeadline().toString());
            obj["check"] = t->isCompleted();
        }

        else if (const auto* r = dynamic_cast<const Routine*>(a)) {
            obj["type"] = "routine";
            obj["startDate"] = QString::fromStdString(r->getStartDate().toString());
            obj["endDate"] = QString::fromStdString(r->getEndDate().toString());
            obj["startTime"] = QString::fromStdString(r->getStartTime().toString());
            obj["endTime"] = QString::fromStdString(r->getEndTime().toString());
            obj["frequency"] = QString::fromStdString(r->FrequencyToString());
            obj["check"] = r->getCheck();
        }

        else if (const auto* rem = dynamic_cast<const Reminder*>(a)) {
            obj["type"]     = "reminder";
            obj["date"]     = QString::fromStdString(rem->getDate().toString());
            obj["time"]     = QString::fromStdString(rem->getTime().toString());
            obj["location"]  = QString::fromStdString(rem->getLocation());
        }
        else if (const auto* e = dynamic_cast<const Event*>(a)) {
            obj["type"]      = "event";
            obj["startDate"] = QString::fromStdString(e->getStartDate().toString());
            obj["endDate"]   = QString::fromStdString(e->getEndDate().toString());
            obj["startTime"] = QString::fromStdString(e->getStartTime().toString());
            obj["endTime"]   = QString::fromStdString(e->getEndTime().toString());
            obj["location"]  = QString::fromStdString(e->getLocation());
            obj["allDay"]    = !e->hasTime();
        }

        arr.append(obj);
    }
    return QJsonDocument(QJsonObject{ {"activities", arr} });
}


void JSONformat::fromJson(const QJsonDocument& doc, JSONformat& am, tagManager& tm) {
    QJsonArray arr = doc.object()["activities"].toArray();

    for (const auto& val : arr) {
        QJsonObject obj = val.toObject();
        std::string type = obj["type"].toString().toStdString();
        std::string name = obj["name"].toString().toStdString();
        std::string desc = obj["description"].toString().toStdString();
        tag* t           = tm.findTag(obj["tag"].toString().toStdString());

        if (type == "project") {
            auto* p = new project(name, desc, t,
                                  dateFromString(obj["deadline"].toString().toStdString()),
                                  hmFromString(obj["oDeadline"].toString().toStdString()),
                                  obj["check"].toBool()); //oggetto QT poi a string

            QJsonArray subtasks = obj["subtasks"].toArray();
            for (const auto& val : subtasks) {
                QJsonObject sub = val.toObject();
                p->add(sub["name"].toString().toStdString(),
                       sub["description"].toString().toStdString(),

                       dateFromString(sub["deadline"].toString().toStdString()),
                       hmFromString(sub["oDeadline"].toString().toStdString()),
                       sub["check"].toBool());
            }
            am.add(p);
        }

        else if (type == "task") {
            auto* tk = new task(name, desc, t,
                                dateFromString(obj["deadline"].toString().toStdString()),
                                hmFromString(obj["oDeadline"].toString().toStdString()));
            tk->setCompleted(obj["check"].toBool());
            am.add(tk);
        }
        else if (type == "routine") {
            auto* r = new Routine(name, desc, t,
                                  hmFromString(obj["startTime"].toString().toStdString()),
                                  hmFromString(obj["endTime"].toString().toStdString()),
                                  dateFromString(obj["startDate"].toString().toStdString()),
                                  dateFromString(obj["endDate"].toString().toStdString()),
                                  freqFromString(obj["frequency"].toString().toStdString()));
            am.add(r);
        }
        else if (type == "reminder") {
            auto* rem = new Reminder(name, desc, t,
                                     dateFromString(obj["date"].toString().toStdString()),
                                     hmFromString(obj["time"].toString().toStdString()),
                                     obj["message"].toString().toStdString());
            am.add(rem);
        }
        else if (type == "event") {
            auto* e = new Event(name, desc, t,
                                dateFromString(obj["startDate"].toString().toStdString()),
                                dateFromString(obj["endDate"].toString().toStdString()),
                                hmFromString(obj["startTime"].toString().toStdString()),
                                hmFromString(obj["endTime"].toString().toStdString()));
            e->setLocation(obj["location"].toString().toStdString());
            am.add(e);
        }
    }
}

bool JSONformat::saveJson(const QString& path, const ActivityManager& am) const {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(toJson(am).toJson(QJsonDocument::Indented));
    return true;
}

bool JSONformat::loadJson(const QString& path, tagManager& tm) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError) return false;
    fromJson(doc, tm);
    return true;
}