#include "Headers/Model/HourMinute.h"
#include <stdexcept>
#include <ctime>

HourMinute::HourMinute (unsigned int h, unsigned int m) {
    if ( h>23 || m>59) {
        throw std::invalid_argument("invalid time");
    }
    else {
        min = h * 60 + m;
    }
}

unsigned int HourMinute::getMin() const {
    return min%60;
}

unsigned int HourMinute::getOre() const {
    return min/60;
}

HourMinute HourMinute::now() {
    time_t t = time(nullptr);
    tm local = *localtime(&t);
    return HourMinute(local.tm_hour, local.tm_min);
}

bool operator>(const HourMinute& n, const HourMinute& m) {
    return n.min > m.min ? true : false;
}

bool operator==(const HourMinute& n, const HourMinute& m) {
    return n.min == m.min ? true : false;
}

std::string HourMinute::toString() const {
    std::string h = (getOre() < 10 ? "0" : "") + std::to_string(getOre());
    std::string m = (getMin() < 10 ? "0" : "") + std::to_string(getMin());
    return h + ":" + m;
}

HourMinute HourMinute::hmFromString(const std::string& s) {
    return HourMinute(std::stoi(s.substr(0,2)),
                      std::stoi(s.substr(3,2)));
}

