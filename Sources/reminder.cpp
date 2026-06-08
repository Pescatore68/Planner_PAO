#include "Headers/reminder.h"
#include "Headers/Visitor/ActivityVisitor.h"

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

QDomElement Reminder::toXml(QDomDocument& xmlDoc) const {
    QDomElement reminderObj = xmlDoc.createElement("activity");
    reminderObj.setAttribute("type", "Reminder");

    QDomElement nameObj = xmlDoc.createElement("name");
    nameObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    reminderObj.appendChild(nameObj);

    QDomElement descObj = xmlDoc.createElement("description");
    descObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    reminderObj.appendChild(descObj);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getTag()->getName())));
    reminderObj.appendChild(tagObj);

    QDomElement tagColorObj = xmlDoc.createElement("tagColor");
    tagColorObj.appendChild(xmlDoc.createTextNode(getTag()->getColor().name()));
    reminderObj.appendChild(tagColorObj);

    QDomElement locObj = xmlDoc.createElement("location");
    locObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(location)));
    reminderObj.appendChild(locObj);

    QDomElement dateObj = xmlDoc.createElement("date");
    dateObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(day.toString())));
    reminderObj.appendChild(dateObj);

    QDomElement timeObj = xmlDoc.createElement("time");
    timeObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(time.toString())));
    reminderObj.appendChild(timeObj);

    return reminderObj;
}

Reminder* Reminder::fromXml(const QDomElement& obj, tagManager& tm) {
    std::string name = obj.firstChildElement("name").text().toStdString();
    std::string desc = obj.firstChildElement("description").text().toStdString();
    std::string loc  = obj.firstChildElement("location").text().toStdString();

    tag* t = tm.newTag(obj.firstChildElement("tag").text().toStdString(),QColor(obj.firstChildElement("tagColor").text()));

    date reminderDate    = date::dateFromString(obj.firstChildElement("date").text().toStdString());
    HourMinute reminderTime = HourMinute::hmFromString(obj.firstChildElement("time").text().toStdString());

    return new Reminder(name, desc, t, reminderDate, reminderTime, loc);
}

void Reminder::accept(ActivityVisitor& v) { v.visit(*this); }
