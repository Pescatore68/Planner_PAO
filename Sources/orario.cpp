#include "Headers/orario.h"
#include <stdexcept>
#include <ctime>

orario::orario (unsigned int h, unsigned int m) {
    if ( h>23 || m>59) {
        throw std::invalid_argument("invalid time");
    }
    else {
        min = h * 60 + m;
    }
}

unsigned int orario::getMin() const {
    return min%60;
}

unsigned int orario::getOre() const {
    return min/60;
}

orario orario::now() {
    time_t t = time(nullptr);
    tm local = *localtime(&t);
    return orario(local.tm_hour, local.tm_min);
}

bool operator>(const orario& n, const orario& m) {
    return n.min > m.min ? true : false;
}


