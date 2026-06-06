#include "Headers/task.h"

task::task(const string& name, const string& description, const tag* Tag, const date& end = date::today(), const HourMinute& oEnd = HourMinute(23,59), const bool& b)
    : AbstractActivity(name, description, Tag), deadline(end), oDeadline(oEnd), check(b) {};

/*task::task(const string& name, const string& description, const date& end, const HourMinute& oEnd)
    : AbstractActivity(name, description), deadline(end), oDeadline(oEnd) {};*/


date task::getDeadline() const { return deadline; }
HourMinute task::getODeadline() const { return oDeadline; }

void task::setDeadline (const date& end) { deadline = end; }
void task::setODeadline (const HourMinute& oEnd) { oDeadline = oEnd; }
bool task::isCompleted() const { return check; }
void task::setCompleted (const bool& b) { check = b; }

bool task::isExpired() const {
    return (deadline > date::today() && oDeadline > HourMinute::now() ) ? true : false;
}

std::string task::summary() const {
    std::string status = check ? "[✓]" : "[ ]";
    std::string result;

    result += status + " " + getName() + "\n";

    if (!getDescription().empty())
        result += "  " + getDescription() + "\n";

    result += "  Scadenza: " + deadline.toString()
              + " ore " + oDeadline.toString() + "\n";

    return result;
}

