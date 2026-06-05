#include "Headers/event.h"

Event::Event(const std::string& name, const std::string& description, const tag* Tag, const date& StartDate, const date& EndDate, const HourMinute& StartTime, const HourMinute& EndTime, const std::string& location)
    : AbstractActivity(name, description, Tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(StartTime), EndTime(EndTime),
      location(location) {}

Event::Event(const std::string& name, const std::string& description, const tag* tag, const date& StartDate, const date& EndDate, const std::string& location)
    : AbstractActivity(name, description, tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(HourMinute(0,0)), EndTime(HourMinute(23,59)),
      location(location) {}

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
void Event::setStartDate(const date& d) { StartDate = d; }
void Event::setEndDate(const date& d) { EndDate = d; }
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
