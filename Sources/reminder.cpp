#include "Headers/reminder.h"

Reminder::Reminder(const std::string& name, const std::string& description, const tag* Tag, const date& day,const HourMinute& time, const std::string& location)
    : AbstractActivity(name, description, Tag),
      day(day), time(time), location(location){}

date Reminder::getDate() const {return day;}
HourMinute Reminder::getTime() const {return time;}
std::string Reminder::getLocation() const {return location;}

void Reminder::setReminderDate(const date& d) { day = d; }    // corretto
void Reminder::setReminderTime(const HourMinute& o) { time = o; }
void Reminder::setLocation(const std::string& l) { location = l; }


bool Reminder::isExpired() const {
    date current = date::today();
    if(current > day) return true;
    if(day > current) return false;
    if( HourMinute::now() > time) return true;
    return false;
}

/*std::string Reminder::summary() const {
    return getName() + " — " +
           std::to_string(reminderDate.getDay()) + "/" +
           std::to_string(reminderDate.getMonth()) + "/" +
           std::to_string(reminderDate.getYear()) + " " +
           std::to_string(reminderOrario.getOre()) + ":" +
           std::to_string(reminderOrario.getMin());
}*/