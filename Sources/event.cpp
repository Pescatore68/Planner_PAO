#include "Headers/event.h"
#include "Headers/Visitor/ActivityVisitor.h"


Event::Event(const std::string& name, const std::string& description, const tag* Tag, const date& StartDate, const date& EndDate, const HourMinute& StartTime, const HourMinute& EndTime, const std::string& location)
    : AbstractActivity(name, description, Tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(StartTime), EndTime(EndTime),
      location(location) {
    if (StartDate > EndDate)
        throw std::invalid_argument("StartDate must be before or equal to EndDate");
}

Event::Event(const std::string& name, const std::string& description, const tag* tag, const date& StartDate, const date& EndDate, const std::string& location)
    : AbstractActivity(name, description, tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(HourMinute(0,0)), EndTime(HourMinute(23,59)),
      location(location) {
    if (StartDate > EndDate)
        throw std::invalid_argument("StartDate must be before or equal to EndDate");

}

Event::Event(const std::string& name, const std:: string& description, const tag* tag, const date& StartDate, const HourMinute& StartTime, const HourMinute& EndTime, const std::string& location)
    : AbstractActivity(name, description, tag),
      StartDate(StartDate), EndDate(StartDate), //end=start
      StartTime(StartTime), EndTime(EndTime),
      location(location){}

//getter
date Event::getStartDate() const { return StartDate; }
date Event::getEndDate() const { return EndDate; }
HourMinute Event::getStartTime() const { return StartTime; }
HourMinute Event::getEndTime() const { return EndTime; }
bool Event::hasTime() const {
    return !((StartTime.getOre() == 0 && StartTime.getMin() == 0 &&
              EndTime.getOre() == 23 && EndTime.getMin() == 59));}
std::string Event::getLocation() const { return location; }

//set
void Event::setStartDate(const date& d) {
    if (d > EndDate)
        throw std::invalid_argument("StartDate must be before or equal to EndDate");
    StartDate = d; }
void Event::setEndDate(const date& d) {
    if (StartDate > d)
        throw std::invalid_argument("EndDate must be after or equal to StartDate");
    EndDate = d;}
void Event::setStartTime(const HourMinute& t) { StartTime = t; }
void Event::setEndTime(const HourMinute& t) { EndTime = t; }
void Event::setLocation(const std::string& l) { location = l; }

bool Event::isExpired() const {
    date current = date::today();
    if (current > EndDate) return true;
    if (EndDate > current) return false;
    if (hasTime()) {
        return HourMinute::now() > EndTime;
    }
    return false;
}

std::string Event::summary() const {
    std::string s = getName() + " — " + getDescription() + "\n";
    s += StartDate.toString();
    if (hasTime())
        s += " " + StartTime.toString() + "–" + EndTime.toString();
    else
        s += " → " + EndDate.toString();
    if (!location.empty())
        s += " | " + location;
    return s;
}

bool Event::isActive(const date& d) const{
    return !(StartDate > d || d > EndDate);
}

QJsonObject Event::toJson() const {
    QJsonObject obj;
    obj["type"] = "event";
    obj["name"] = QString::fromStdString(getName());
    obj["description"] = QString::fromStdString(getDescription());
    obj["tag"] = QString::fromStdString(getTag()->getName());
    obj["tagColor"] = getTag()->getColor().name();
    obj["startDate"] = QString::fromStdString(getStartDate().toString());
    obj["endDate"] = QString::fromStdString(getEndDate().toString());
    obj["startTime"] = QString::fromStdString(getStartTime().toString());
    obj["endTime"] = QString::fromStdString(getEndTime().toString());
    obj["location"] = QString::fromStdString(getLocation());
    obj["allDay"] = !hasTime();
    return obj;

}

Event* Event::fromJson(const QJsonObject& obj, tagManager& tm) {
    auto* e = new Event(obj["name"].toString().toStdString(),
                        obj["description"].toString().toStdString(),
                        tm.newTag(obj["tag"].toString().toStdString(), QColor(obj["tagColor"].toString())),
                        date::dateFromString(obj["startDate"].toString().toStdString()),
                        date::dateFromString(obj["endDate"].toString().toStdString()),
                        HourMinute::hmFromString(obj["startTime"].toString().toStdString()),
                        HourMinute::hmFromString(obj["endTime"].toString().toStdString()));
    e->setLocation(obj["location"].toString().toStdString());
    return e;
}

QDomElement Event::toXml(QDomDocument& xmlDoc) const {
    QDomElement eventObj = xmlDoc.createElement("activity");
    eventObj.setAttribute("type",   "Event");
    eventObj.setAttribute("allDay", !hasTime() ? "true" : "false");

    QDomElement nameObj = xmlDoc.createElement("name");
    nameObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    eventObj.appendChild(nameObj);

    QDomElement descObj = xmlDoc.createElement("description");
    descObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    eventObj.appendChild(descObj);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getTag()->getName())));
    eventObj.appendChild(tagObj);

    QDomElement tagColorObj = xmlDoc.createElement("tagColor");
    tagColorObj.appendChild(xmlDoc.createTextNode(getTag()->getColor().name()));
    eventObj.appendChild(tagColorObj);

    QDomElement locObj = xmlDoc.createElement("location");
    locObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getLocation())));
    eventObj.appendChild(locObj);

    QDomElement startDateObj = xmlDoc.createElement("startDate");
    startDateObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getStartDate().toString())));
    eventObj.appendChild(startDateObj);

    QDomElement endDateObj = xmlDoc.createElement("endDate");
    endDateObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getEndDate().toString())));
    eventObj.appendChild(endDateObj);

    if (hasTime()) {
        QDomElement startTimeObj = xmlDoc.createElement("startTime");
        startTimeObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getStartTime().toString())));
        eventObj.appendChild(startTimeObj);

        QDomElement endTimeObj = xmlDoc.createElement("endTime");
        endTimeObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getEndTime().toString())));
        eventObj.appendChild(endTimeObj);
    }

    return eventObj;
}

Event* Event::fromXml(const QDomElement& obj, tagManager& tm) {
    std::string name = obj.firstChildElement("name").text().toStdString();
    std::string desc = obj.firstChildElement("description").text().toStdString();
    std::string loc  = obj.firstChildElement("location").text().toStdString();

    tag* t = tm.newTag(obj.firstChildElement("tag").text().toStdString(), QColor(obj.firstChildElement("tagColor").text()));

    date startDate = date::dateFromString(obj.firstChildElement("startDate").text().toStdString());
    date endDate   = date::dateFromString(obj.firstChildElement("endDate").text().toStdString());

    bool allDay = obj.attribute("allDay") == "true";
    if (!allDay) {
        HourMinute startTime = HourMinute::hmFromString(obj.firstChildElement("startTime").text().toStdString());
        HourMinute endTime   = HourMinute::hmFromString(obj.firstChildElement("endTime").text().toStdString());
        return new Event(name, desc, t, startDate, endDate, startTime, endTime, loc);
    }

    return new Event(name, desc, t, startDate, endDate, loc);
}

void Event::accept(ActivityVisitor& v) { v.visit(*this); }
