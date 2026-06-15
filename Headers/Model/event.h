#ifndef EVENT_H
#define EVENT_H

#include "AbstractActivity.h"
#include "date.h"
#include "HourMinute.h"
#include "tagManager.h"
#include <string>

class Event : public AbstractActivity{
private:
    date StartDate;
    date EndDate;
    HourMinute StartTime;
    HourMinute EndTime;
    std::string location;
public:
    //Constructors with time
    Event(const std::string&, const std::string&, const tag*, const date&, const date&, const HourMinute&, const HourMinute&, const std::string& Location = "" );
    //Constructors with no time
    Event(const std::string& name, const std::string& description, const tag* tag, const date& StartDate, const date& EndDate, const std::string& Location = "");
    //Constructors only
    //      one day
    Event(const std::string& name, const std::string& description,
          const tag* tag, const date& startDate,
          const HourMinute& startTime, const HourMinute& endTime,
          const std::string& location = "");

    date getStartDate() const;
    date getEndDate() const;
    HourMinute getStartTime() const;
    HourMinute getEndTime() const;
    bool hasTime() const; // non so se tenerlo, serve nella gui per mostrare l'HourMinute o allday
    std::string getLocation()const;

    void setStartDate(const date&);
    void setEndDate(const date&);
    void setStartTime(const HourMinute&);
    void setEndTime(const HourMinute&);
    void setLocation(const std::string&);

    //bool current() const; non credo serva
    bool isExpired() const override;
    bool isActive(const date&) const override;

    //data persistence
    QJsonObject toJson() const override;
    static Event* fromJson(const QJsonObject& , tagManager& );

    QDomElement toXml(QDomDocument& xmlDoc) const override;
    static Event* fromXml(const QDomElement& obj, tagManager& tm);

    void accept(ActivityVisitor& v) override;
};
#endif // EVENT_H
