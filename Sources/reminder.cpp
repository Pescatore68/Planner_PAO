#include "Headers/reminder.h"
#include "Headers/ActivityVisitor.h"

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

void Reminder::accept(ActivityVisitor& v) { v.visit(*this); }

//data persistence
void Reminder::toXml(QDomElement& reminderObj, QDomDocument& xmlDoc) const {
    reminderObj.setAttribute("type", "Reminder");

    QDomElement name = xmlDoc.createElement("name");
    name.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    reminderObj.appendChild(name);

    QDomElement description = xmlDoc.createElement("description");
    description.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    reminderObj.appendChild(description);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(
        getTag() ? QString::fromStdString(getTag()->getName()) : ""));
    reminderObj.appendChild(tagObj);

    QDomElement d = xmlDoc.createElement("date");
    d.setAttribute("day",   static_cast<int>(day.getDay()));
    d.setAttribute("month", static_cast<int>(day.getMonth()));
    d.setAttribute("year",  static_cast<int>(day.getYear()));
    reminderObj.appendChild(d);

    QDomElement t = xmlDoc.createElement("time");
    t.setAttribute("hour", static_cast<int>(time.getOre()));
    t.setAttribute("min",  static_cast<int>(time.getMin()));
    reminderObj.appendChild(t);

    QDomElement loc = xmlDoc.createElement("location");
    loc.appendChild(xmlDoc.createTextNode(QString::fromStdString(location)));
    reminderObj.appendChild(loc);
}

void Reminder::fromXml(const QDomElement& reminderObj) {
    setName(reminderObj.firstChildElement("name").text().toStdString());
    setDesc(reminderObj.firstChildElement("description").text().toStdString());

    QDomElement d = reminderObj.firstChildElement("date");
    day.changeDate(d.attribute("year").toUInt(),
                   d.attribute("month").toUInt(),
                   d.attribute("day").toUInt());

    QDomElement t = reminderObj.firstChildElement("time");
    time = HourMinute(t.attribute("hour").toUInt(),
                      t.attribute("min").toUInt());

    location = reminderObj.firstChildElement("location").text().toStdString();
}