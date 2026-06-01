#include "Headers/event.h"

Event::Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const date& EndDate,
          const orario& StartTime, const orario& EndTime,
          const std::string& Location )
    : AbstractActivity(id, name, description, tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(new orario(StartTime)), EndTime(new orario(EndTime)),
      location(Location){}

Event::Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const date& EndDate, const std::string& Location)
    : AbstractActivity(id, name, description, tag),
      StartDate(StartDate), EndDate(EndDate),
      StartTime(nullptr), EndTime(nullptr), location(Location){}

Event::Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const orario& StartTime, const orario& EndTime, const std::string& Location)
    : AbstractActivity(id, name, description, tag),
      StartDate(StartDate), EndDate(StartDate), //end=start
      StartTime(new orario(StartTime)), EndTime(new orario(EndTime)),
      location(Location){}
Event::~Event() {
    delete StartTime;
    delete EndTime;
}

//getter
date Event::getStartDate() const { return StartDate; }
date Event::getEndDate() const { return EndDate; }
orario* Event::getStartTime() const { return StartTime; }
orario* Event::getEndTime() const { return EndTime; }
bool Event::Time() const { return StartTime != nullptr; }
std::string Event::getLocation() const { return Location; }

//set
void Event::setSDate(const date& d) { StartDate = d; }
void Event::setEDate(const date& d) { EndDate = d; }
void Event::setSTime(const orario& t) {
    delete StartTime;
    StartTime = new orario(t);
}
void Event::setETime(const orario& t) {
    delete EndTime;
    EndTime = new orario(t);
}
void Event::setLocation(const std::string& l) { location = l; }

bool Event::isExpired() const {
    date Day =  date :: today();
    int today_N = Day.getYear()*10000 + Day.getMonth()*100 + Day.getDay();
    int DueDay = EndDate.getYear()*10000 + EndDate.getMonth()*100 + EndDate.getDay();

    if(today_N > DueDay) return true;

    if(Time() && today_N == EndDay){
        orario current_time = orario :: now();
        unsigned int Min = current_time.getOre() * 60 + current_time.getMin();
        unsigned int EndMin = EndTime->getOre() * 60 + EndTime->getMin();
        return Min > EndMin;
    }
    return false;
}
