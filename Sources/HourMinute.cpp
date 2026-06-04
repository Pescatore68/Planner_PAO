#include "Headers/HourMinute.h"
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


