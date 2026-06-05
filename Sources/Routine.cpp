#include "Headers/routine.h"

Routine::Routine(const std::string& name, const std::string& description, const tag* t, const HourMinute& startTime, const HourMinute& endTime, const date& startDate, const date& endDate, Frequency freq)
    : AbstractActivity(name, description, t),
      startTime(startTime), endTime(endTime),
      startDate(startDate), endDate(endDate),
      freq(freq), check(false) {}

HourMinute Routine::getStartTime() const { return startTime; }
HourMinute Routine::getEndTime() const { return endTime; }
date Routine::getStartDate() const { return startDate; }
date Routine::getEndDate() const { return endDate; }
Routine::Frequency Routine::getFrequency() const { return freq; }
bool Routine::getCheck() const { return check; }
const std::vector<bool>& Routine::getcheckHistory() const { return check_history; }

void Routine::setStartTime(const HourMinute& o) { startTime = o; }
void Routine::setEndTime(const HourMinute& o) { endTime = o; }
void Routine::setStartDate(const date& d) { startDate = d; }
void Routine::setEndDate(const date& d) { endDate = d; }
void Routine::setFrequency(Frequency f) { freq = f; }

void Routine::setCheck(bool b) { check = b; }

void Routine::closeCheck() {
    check_history.push_back(check);
    check = false;
}

bool Routine::isExpired() const {
    return date::today() > endDate;
}

std::string Routine::summary() const {
    std::string s = getName() + " — " + FrequencyToString() + " - " + getDescription() + "\n";
    s += startTime.toString() + "–" + endTime.toString();
    s += " | oggi: ";
    s += check ? "✓" : "○";

    if (!check_history.empty()) {
        s += " | ";
        for (bool b : check_history) {
            s += b ? "●" : "○";
        }
    }
    return s;
}

std::string Routine::FrequencyToString() const {
    if (freq == Frequency::Daily) return "Daily";
    if (freq == Frequency::Weekly) return "Weekly";
    if (freq == Frequency::Monthly) return "Monthly";
    if (freq == Frequency::Yearly) return "Yearly";
	return "Not implemented";
}