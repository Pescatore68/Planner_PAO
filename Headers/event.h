#ifndef EVENT_H
#define EVENT_H

#include "AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/HourMinute.h"
#include <string>

class Event : public AbstractActivity{
private:
    date StartDate;
    date EndDate;
    orario* StartTime;
    orario* EndTime;
    std::string Location;
public:
    //Constructors with time
    Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const date& EndDate,
          const orario& StartTime, const orario& EndTime,
          const std::string& Location = "" );
    //Constructors with no time
    Event(unsigned int id, const std::string& name, const std:: string& description, const Tag& tag, const date& StartDate, const date& EndDate,
          const std::string& Location = "");
    //Constructors only one day
    Event(unsigned int id, const std::string& name, const std::string& description,
          const Tag& tag, const date& startDate,
          const orario& startTime, const orario& endTime,
          const std::string& location = "");

    ~Event() override;

    date getStartDate() const;
    date getEndDate() const;
    orario* getStartTime() const;
    orario* getEndTime() const;
    bool Time() const; // non so se tenerlo, serve nella gui per mostrare l'orario o allday
    std::string getLocation()const;

    void setSDate(const date& d);
    void setEDate(const date& d);
    void setSTime(const orario& t);
    void setETime(const orario& t);
    void setLocation(const std::string& l);

    //bool current() const; non credo serva
    bool isExpired() const override;

#endif // EVENT_H
