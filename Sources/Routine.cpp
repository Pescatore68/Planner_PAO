#include "Headers/routine.h"

Routine::Routine(unsigned int id, const std::string& name, const std::string& description,
                 const tag* t, const orario& startTime, const orario& endTime,
                 const date& startDate, const date& endDate,
                 Frequency freq)
    : AbstractActivity(id, name, description, t),
      startTime(startTime), endTime(endTime),
      startDate(startDate), endDate(endDate),
      freq(freq), check(false) {}

orario Routine::getStartTime() const { return startTime; }
orario Routine::getEndTime() const { return endTime; }
date Routine::getStartDate() const { return startDate; }
date Routine::getEndDate() const { return endDate; }
Routine::Frequency Routine::getFrequency() const { return freq; }
bool Routine::getCheck() const { return check; }
const std::vector<bool>& Routine::getcheckHistory() const { return check_history; }

void Routine::setStartTime(const orario& o) { startTime = o; }
void Routine::setEndTime(const orario& o) { endTime = o; }
void Routine::setStartDate(const date& d) { startDate = d; }
void Routine::setEndDate(const date& d) { endDate = d; }
void Routine::setFrequency(Frequency f) { freq = f; }

void Routine::setCheck(bool b) { check = b; }

void Routine::closeCheck() {
    history.push_back(check);
    check = false;
}

bool Routine::isExpired() const {
    date current = date::today();
    int current_day = current.getYear() * 10000 + current.getMonth() * 100 + current.getDay();
    int endRoutine = endDate.getYear() * 10000 + endDate.getMonth() * 100 + endDate.getDay();
    return current_day > endRoutine;
}

/*std::string Routine::summary() const {
    return getName() + " — " + frequencyToString(frequency)
           + " " + std::to_string(startTime.getOre()) + ":"
           + std::to_string(startTime.getMin()) + "–"
           + std::to_string(endTime.getOre()) + ":"
           + std::to_string(endTime.getMin());
}*/

std::string Routine::FrequencyToString(Frequency freq) {
    if (freq == Frequency::Daily) return "Daily";
    if (freq == Frequency::Weekly) return "Weekly";
    if (freq == Frequency::Monthly) return "Monthly";
    if (freq == Frequency::Yearly) return "Yearly";
}