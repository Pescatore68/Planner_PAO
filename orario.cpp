#include "orario.h"
#include <stdexcept>

orario::orario (unsigned int m, unsigned int o) {
    if ( o>23 || m>59) {
        throw std::invalid_argument("invalid time");
    }
    else {
        min = o*60+m;
    }
}

unsigned int orario::getMin() const {
    return min%60;
}

unsigned int orario::getOre() const {
    return min/60;
}


