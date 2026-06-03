#include "Headers/reminder.h"

Reminder::Reminder(const std::string& name, const std::string& description, const tag* Tag, const date& d, const orario& t, const std::string& location)
    : AbstractActivity(name, description, Tag)
      day(d), time(t), location(location){}

date Reminder::getDate() const {return day;}
orario Reminder::getTime() const {return time;}
std::string Reminder::getLocation() const {return location;}

void setReminderDate(const date& d);
void setReminderOrario(const orario& o);
void setLocation(const std::string& loc);


bool Reminder::isExpired() const {
    date current = date::today();
    int currentDay = current.getYear() * 10000 + current.getMonth() * 100 + current.getDay();
    int reminderDate = day.getYear() * 10000 + day.getMonth() * 100 + day.getDay();
    if(currentDay > reminderDate) return true;

    if(currentDay == reminderDate){
        orario currentT = orario::now();
        unsigned int currentTime = currentT.getOre() * 60 + currentT.getMin();
        unsigned int reminderTime = time.getOre() * 60 + time.getMin();
        return (currentTime > reminderTime);
    }
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