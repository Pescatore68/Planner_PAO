#include "Headers/event.h"
#include "Headers/ActivityVisitor.h"


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
bool Event::hasTime() const { return !(StartTime==HourMinute(0,0) && EndTime==HourMinute(23,59));}
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

void Event::accept(ActivityVisitor& v) { v.visit(*this); }

void Event::toXml(QDomElement& evtObj, QDomDocument& xmlDoc) const {
    evtObj.setAttribute("type", "Event");

    QDomElement name = xmlDoc.createElement("name");
    name.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    evtObj.appendChild(name);

    QDomElement description = xmlDoc.createElement("description");
    description.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    evtObj.appendChild(description);


    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(
        getTag() ? QString::fromStdString(getTag()->getName()) : ""));
    evtObj.appendChild(tagObj);


    QDomElement startDate = xmlDoc.createElement("startDate");
    startDate.setAttribute("day",   static_cast<int>(StartDate.getDay()));
    startDate.setAttribute("month", static_cast<int>(StartDate.getMonth()));
    startDate.setAttribute("year",  static_cast<int>(StartDate.getYear()));
    evtObj.appendChild(startDate);

    QDomElement endDate = xmlDoc.createElement("endDate");
    endDate.setAttribute("day",   static_cast<int>(EndDate.getDay()));
    endDate.setAttribute("month", static_cast<int>(EndDate.getMonth()));
    endDate.setAttribute("year",  static_cast<int>(EndDate.getYear()));
    evtObj.appendChild(endDate);

    if (hasTime()) {
        QDomElement startTime = xmlDoc.createElement("startTime");
        startTime.setAttribute("hour", static_cast<int>(StartTime.getOre()));
        startTime.setAttribute("min",  static_cast<int>(StartTime.getMin()));
        evtObj.appendChild(startTime);

        QDomElement endTime = xmlDoc.createElement("endTime");
        endTime.setAttribute("hour", static_cast<int>(EndTime.getOre()));
        endTime.setAttribute("min",  static_cast<int>(EndTime.getMin()));
        evtObj.appendChild(endTime);
    }

    QDomElement loc = xmlDoc.createElement("location");
    loc.appendChild(xmlDoc.createTextNode(QString::fromStdString(location)));
    evtObj.appendChild(loc);
}

void Event::fromXml(const QDomElement& evtObj) {
    setName(evtObj.firstChildElement("name").text().toStdString());
    setDesc(evtObj.firstChildElement("description").text().toStdString());

    QDomElement startDate = evtObj.firstChildElement("startDate");
    StartDate.changeDate(startDate.attribute("year").toUInt(),startDate.attribute("month").toUInt(),startDate.attribute("day").toUInt());

    QDomElement endDate = evtObj.firstChildElement("endDate");
    EndDate.changeDate(endDate.attribute("year").toUInt(),
                       endDate.attribute("month").toUInt(),
                       endDate.attribute("day").toUInt());
    if(hasTime())
    {
        QDomElement startTime = evtObj.firstChildElement("StartTime");
        StartTime = HourMinute(startTime.attribute("hour").toUInt(), startTime.attribute("min").toUInt());

        QDomElement endTime = evtObj.firstChildElement("StartTime");
        EndTime = HourMinute(startTime.attribute("hour").toUInt(), endTime.attribute("min").toUInt());

    }

    location = evtObj.firstChildElement("location").text().toStdString();

}