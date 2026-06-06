#include "Headers/task.h"
#include "Headers/ActivityVisitor.h"

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

void task::accept(ActivityVisitor& v) { v.visit(*this); }

// in task.cpp
void task::toXml(QDomElement& taskObj, QDomDocument& xmlDoc) const {
    taskObj.setAttribute("type", "Task");
    taskObj.setAttribute("check", check ? "true" : "false");

    QDomElement name = xmlDoc.createElement("name");
    name.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    taskObj.appendChild(name);

    QDomElement desc = xmlDoc.createElement("description");
    desc.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    taskObj.appendChild(desc);

    QDomElement tagElem = xmlDoc.createElement("tag");
    tagElem.appendChild(xmlDoc.createTextNode(
        getTag() ? QString::fromStdString(getTag()->getName()) : ""));
    taskObj.appendChild(tagElem);

    QDomElement dl = xmlDoc.createElement("deadline");
    dl.setAttribute("day",   static_cast<int>(deadline.getDay()));
    dl.setAttribute("month", static_cast<int>(deadline.getMonth()));
    dl.setAttribute("year",  static_cast<int>(deadline.getYear()));
    taskObj.appendChild(dl);

    QDomElement odl = xmlDoc.createElement("oDeadline");
    odl.setAttribute("hour", static_cast<int>(oDeadline.getOre()));
    odl.setAttribute("min",  static_cast<int>(oDeadline.getMin()));
    taskObj.appendChild(odl);
}

void task::fromXml(const QDomElement& taskObj) {
    setName(taskObj.firstChildElement("name").text().toStdString());
    setDesc(taskObj.firstChildElement("description").text().toStdString());
    check = taskObj.attribute("check") == "true";

    QDomElement dl = taskObj.firstChildElement("deadline");
    deadline.changeDate(dl.attribute("year").toUInt(), dl.attribute("month").toUInt(), dl.attribute("day").toUInt());

    QDomElement odl = taskObj.firstChildElement("oDeadline"); oDeadline = HourMinute(odl.attribute("hour").toUInt(), odl.attribute("min").toUInt());
}
