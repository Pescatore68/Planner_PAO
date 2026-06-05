#ifndef ROUTINE_H
#define ROUTINE_H

#include "AbstractActivity.h"
#include "date.h"
#include "HourMinute.h"
#include <vector>
#include <string>

class Routine : public AbstractActivity{
public:
    enum class Frequency{ Daily, Weekly, Monthly, Yearly};
    Routine(const std::string& name, const std::string& description, const tag* t, const HourMinute& start, const HourMinute& end, const date& startDate, const date& endDate, Frequency freq);

    HourMinute getStartTime() const;
    HourMinute getEndTime() const;
    date getStartDate() const;
    date getEndDate() const;
    Frequency getFrequency() const;
    bool getCheck() const;
    const std::vector<bool>& getcheckHistory() const;

    void setStartTime(const HourMinute& startTime);
    void setEndTime(const HourMinute& endTime);
    void setStartDate(const date& startDate);
    void setEndDate(const date& endDate);
    void setFrequency(Frequency freq);
    void setCheck(bool check);

    void closeCheck();
    bool isActive(const date& d) const;


    bool isExpired() const override;
    std::string summary() const override;

    std::string FrequencyToString() const;


private:
    HourMinute startTime;
    HourMinute endTime;
    date startDate;
    date endDate;
    Frequency freq;
    bool check;
    std::vector<bool>check_history;
};

#endif // ROUTINE_H
