#ifndef ROUTINE_H
#define ROUTINE_H

#include "AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/orario.h"
#include <vector>
#include <string>

class Routine : public AbstractActivity{
private:
    orario startTime;
    orario endTime;
    date startDate;
    date endDate;
    Frequency freq;
    bool check;
    std::vector<bool>check_history;

public:
    enum class Frequency{ Daily, Weekly, Monthly, Annual}
    Routine(unsigned int id, const std::string& name, std::string& description, const tag* t, const orario& start, const orario& end, const date& startDate, const date& endDate, Frequency freq);
    ~Routine();

    orario getStartTime() const;
    orario getEndTime() const;
    date getStartDate() const;
    date getEndDate() const;
    Frequency getFrequency() const;
    bool getCheck() const;
    const std::vector<bool>& getcheckHistory() const;

    void setStartTime(const orari& startTime);
    void setEndTime(const orario& endTime);
    void setStartDate(const date& startDate);
    void setEndDate(const date& endDate);
    void setFrequency(Frequecy freq);
    void setCheck(bool check);

    void closeCheck();


    bool isExpired() const override;
    std::string summary() const override;

    static std:: string FrequencyToString(Frequency f);


#endif // ROUTINE_H
