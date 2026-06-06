#include "Headers/task.h"
#include "Headers/tagManager.h"
#include "Headers/ActivityVisitor.h"

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

QJsonObject task::toJson() const {
    QJsonObject obj;
    obj["type"] = "task";
    obj["name"] = QString::fromStdString(getName());
    obj["description"] = QString::fromStdString(getDescription());
    obj["tag"] = QString::fromStdString(getTag()->getName());
    obj["tagColor"] = getTag()->getColor().name();
    obj["deadline"] = QString::fromStdString(getDeadline().toString());
    obj["oDeadline"] = QString::fromStdString(getODeadline().toString());
    obj["check"] = isCompleted();
    return obj;
}

task* task::fromJson(const QJsonObject& obj, tagManager& tm) {
    auto* tk = new task(obj["name"].toString().toStdString(),
                        obj["description"].toString().toStdString(),
                        tm.newTag(obj["tag"].toString().toStdString(), QColor(obj["tagColor"].toString())),
                        date::dateFromString(obj["deadline"].toString().toStdString()),
                        HourMinute::hmFromString(obj["oDeadline"].toString().toStdString()));
    tk->setCompleted(obj["check"].toBool());
    return tk;
}



void task::accept(ActivityVisitor& v) { v.visit(*this); }
