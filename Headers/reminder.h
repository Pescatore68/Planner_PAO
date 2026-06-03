#ifndef REMINDER_H
#define REMINDER_H
#include "AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/orario.h"
#include <string>

class Reminder: public AbstractActivity {
private:
    date day;
    orario time;
    std::string location;
public:
    reminder(unsigned int id, const std::string& name, const std::string& description, const tag* t, const date& day, const orario& time, const std::string& location);
    ~Reminder();

    date getDate() const;
    orario getTime() const;
    std::string getLocation() const;

    void setDate(const date& d)
    void setTime(const orario& t)
    void setLocation(const std::string& l)

    bool isExpired() const override;
    std::string summary() const override;
}
#endif // REMAINDER_H
