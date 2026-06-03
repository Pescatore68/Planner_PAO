#include "Headers/reminder.h"

Reminder::Reminder(unsigned int id, const std::string& name, const std::string& description, const tag* t, const date& d, const orario& t, const std::string& location)
    : AbstractActivity(id, name, description, t)
      day(d), time(t), location(location){}

date Reminder::getDate() const {return day;}
orario Reminder::getTime() const {return time;}
std::string Reminder::getLocation() const {return location;}


bool Reminder::isExpired() const {
    date current = date::today();
    int currentDay = current.getYear() * 1000 + current.getMonth() * 100 + current.getDay();
    int reminderDate = day.getYear() * 1000 + day.getMonth() * 100 + day.getDay();
    if(currentDay > reminderDate) return true;

    if(currentDay == reminderDate){
        orario currentT = orario::now();
        unsigned int currentTime = current.getOre() * 60 + current.getMin();
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