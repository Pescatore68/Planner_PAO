#ifndef REMINDER_H
#define REMINDER_H

#include "AbstractActivity.h"
#include "date.h"
#include "HourMinute.h"
#include <string>

class Reminder : public AbstractActivity {
private:
    date day;
    HourMinute time;
    std::string location;

public:
    Reminder(const std::string& name, const std::string& description, const tag* t, const date& day, const HourMinute& time, const std::string& location);

    date getDate() const;
    HourMinute getTime() const;
    std::string getLocation() const;

    void setDate(const date&);
    void setTime(const HourMinute&);
    void setLocation(const std::string&);

    bool isExpired() const override;
    std::string summary() const override;
};
#endif // REMAINDER_H
