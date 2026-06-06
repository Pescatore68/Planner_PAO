#include "Headers/event.h"

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
