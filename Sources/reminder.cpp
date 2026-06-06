#include "Headers/reminder.h"

Reminder::Reminder(const std::string& name, const std::string& description, const tag* Tag, const date& day,const HourMinute& time, const std::string& location)
    : AbstractActivity(name, description, Tag), day(day), time(time), location(location) {}

date Reminder::getDate() const {return day;}
HourMinute Reminder::getTime() const {return time;}
std::string Reminder::getLocation() const {return location;}

void Reminder::setDate(const date& d) { day = d; }    // corretto
void Reminder::setTime(const HourMinute& o) { time = o; }
void Reminder::setLocation(const std::string& l) { location = l; }


bool Reminder::isExpired() const {
    date current = date::today();
    if(current > day) return true;
    if(day > current) return false;
    if( HourMinute::now() > time) return true;
    return false;
}


std::string Reminder::summary() const {
    std::string s = getName() + " — " + getDescription() + "\n";
    s += day.toString() + " " + time.toString();
    if (!location.empty())
        s += " | " + location;
    return s;
}

QJsonObject Reminder::toJson() const {
    QJsonObject obj;
    obj["type"] = "reminder";
    obj["name"] = QString::fromStdString(getName());
    obj["description"] = QString::fromStdString(getDescription());
    obj["tag"] = QString::fromStdString(getTag()->getName());
    obj["tagColor"] = getTag()->getColor().name();
    obj["date"] = QString::fromStdString(getDate().toString());
    obj["time"] = QString::fromStdString(getTime().toString());
    obj["location"] = QString::fromStdString(getLocation());
    return obj;
}

Reminder* Reminder::fromJson(const QJsonObject& obj, tagManager& tm) {
    auto* rem = new Reminder(obj["name"].toString().toStdString(),
                             obj["description"].toString().toStdString(),
                             tm.newTag(obj["tag"].toString().toStdString(), QColor(obj["tagColor"].toString())),
                             date::dateFromString(obj["date"].toString().toStdString()),
                             HourMinute::hmFromString(obj["time"].toString().toStdString()),
                             obj["message"].toString().toStdString());
    return rem;
}