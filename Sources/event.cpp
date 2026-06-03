#include "Headers/event.h"

Event::Event(const std::string& name, const std:: string& description, const tag* Tag, const date& StartDate, const date& EndDate,
          const HourMinute& StartTime, const & EndTime,
          const std::string& Location )
    : AbstractActivity(name, description, Tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(new (StartTime)), EndTime(new (EndTime)),
      location(Location){}

Event::Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const date& EndDate, const std::string& Location)
    : AbstractActivity(id, name, description, tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(nullptr), EndTime(nullptr), location(Location){}

Event::Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const & StartTime, const & EndTime, const std::string& Location)
    : AbstractActivity(id, name, description, tag),
      StartDate(StartDate), EndDate(StartDate), //end=start
      StartTime(new (StartTime)), EndTime(new (EndTime)),
      location(Location){}
Event::~Event() {
    delete StartTime;
    delete EndTime;
}

//getter
date Event::getStartDate() const { return StartDate; }
date Event::getEndDate() const { return EndDate; }
* Event::getStartTime() const { return StartTime; }
* Event::getEndTime() const { return EndTime; }
bool Event::Time() const { return StartTime != nullptr; }
std::string Event::getLocation() const { return location; }

//set
void Event::setSDate(const date& d) { StartDate = d; }
void Event::setEDate(const date& d) { EndDate = d; }
void Event::setSTime(const & t) {
    delete StartTime;
    StartTime = new (t);
}
void Event::setETime(const & t) {
    delete EndTime;
    EndTime = new (t);
}
void Event::setLocation(const std::string& l) { location = l; }

bool Event::isExpired() const {
    date current = date::today();
    if (current > EndDate) return true;
    if (EndDate > current) return false;
    if (hasTime()) {
        return HourMinute::now() > *EndTime;
    }
    return false;
}
