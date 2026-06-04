#include "Headers/task.h"

task::task(const string& name, const string& description, const tag* Tag, const date& end, const HourMinute& oEnd)
    : AbstractActivity(name, description, Tag), deadline(end), oDeadline(oEnd) {};

task::task(const string& name, const string& description,  const date& end, const HourMinute& oEnd)
    : AbstractActivity(name, description), deadline(end), oDeadline(oEnd) {};


date task::getDeadline() const { return deadline; }
HourMinute task::getODeadline() const { return oDeadline; }

void task::setDeadline (const date& end) { deadline = end; }
void task::setODeadline (const HourMinute& oEnd) { oDeadline = oEnd; }
bool task::isCompleted() const { return check; }
void task::setCompleted (const bool& b) { check = b; }

bool task::isExpired() const {
    return (deadline > date::today() && oDeadline > HourMinute::now() ) ? true : false;
}

string task::summary() const {

}

