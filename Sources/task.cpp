#include "Headers/task.h"

task::task(std::string n, date s, date e, orario os, orario oe, tag* t) :
    name(n), starts(s), ends(e), oStarts(os), oEnds(oe), tag(t), check(false) {
}

task::task(std::string n, date s, date e, orario os, orario oe) :
    name(n), starts(s), ends(e), oStarts(os), oEnds(oe), tag*(tag::defaultTag()), check(false) {}

std::string task::getName() const {
    return name;
};

date task::getStarts() const {
    return starts;
};

date task::getEnds() const {
    return ends;
};

orario task::getOStarts() const {
    return oStarts;
};

orario task::getOEnds() const {
    return oEnds;
};

bool task::isCompleted() const {
    return check;
};

void task::setCompleted (bool b) {
    check=b;
};

void task::setName(std::string n) {
    name=n;
};

void task::setStarts(date s) {
    starts=s;
};

void task::setEnds(date e) {
    ends=e;
};

void task::setOStarts(orario os) {
    oStarts=os;
};

void task::setOEnds(orario oe) {
    oEnds=oe;
};
