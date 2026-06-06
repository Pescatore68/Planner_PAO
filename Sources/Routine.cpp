#include "Headers/routine.h"
#include "Headers/ActivityVisitor.h"

Routine::Routine(const std::string& name, const std::string& description, const tag* t, const HourMinute& startTime, const HourMinute& endTime, const date& startDate, const date& endDate, Frequency freq)
    : AbstractActivity(name, description, t),
      startTime(startTime), endTime(endTime),
      startDate(startDate), endDate(endDate),
      freq(freq), check(false) {
    if (startDate > endDate)
        throw std::invalid_argument("StartDate must be before or equal to EndDate");
    }

HourMinute Routine::getStartTime() const { return startTime; }
HourMinute Routine::getEndTime() const { return endTime; }
date Routine::getStartDate() const { return startDate; }
date Routine::getEndDate() const { return endDate; }
Routine::Frequency Routine::getFrequency() const { return freq; }
bool Routine::getCheck() const { return check; }
const std::vector<bool>& Routine::getcheckHistory() const { return check_history; }

void Routine::setStartTime(const HourMinute& o) { startTime = o; }
void Routine::setEndTime(const HourMinute& o) { endTime = o; }
void Routine::setStartDate(const date& d) {
    if (d > endDate)
        throw std::invalid_argument("StartDate must be before or equal to EndDate");
    startDate = d; }
void Routine::setEndDate(const date& d) {
    if (startDate > d)
        throw std::invalid_argument("EndDate must be after or equal to StartDate");
    endDate = d;
    }
void Routine::setFrequency(Frequency f) { freq = f; }

void Routine::setCheck(bool b) { check = b; }

void Routine::closeCheck() {
    check_history.push_back(check);
    check = false;
}

bool Routine::isActive(const date& d) const {
    if (d > endDate || startDate > d) return false;

    if (freq == Frequency::Daily)   return true;
    if (freq == Frequency::Weekly)  return d.dayOfWeek() == startDate.dayOfWeek();
    if (freq == Frequency::Monthly) return d.getDay() == startDate.getDay();
    if (freq == Frequency::Yearly) return d.getDay() == startDate.getDay() && d.getMonth() == startDate.getMonth();
    return false;
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

void Routine::accept(ActivityVisitor& v) { v.visit(*this); }

// data persistence
void Routine::toXml(QDomElement& routineObj, QDomDocument& xmlDoc) const {
    routineObj.setAttribute("type",      "Routine");
    routineObj.setAttribute("frequency", QString::fromStdString(FrequencyToString()));
    routineObj.setAttribute("check",     check ? "true" : "false");

    QDomElement name = xmlDoc.createElement("name");
    name.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    routineObj.appendChild(name);

    QDomElement desc = xmlDoc.createElement("description");
    desc.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    routineObj.appendChild(desc);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(
        getTag() ? QString::fromStdString(getTag()->getName()) : ""));
    routineObj.appendChild(tagObj);

    QDomElement st = xmlDoc.createElement("startTime");
    st.setAttribute("hour", static_cast<int>(startTime.getOre()));
    st.setAttribute("min",  static_cast<int>(startTime.getMin()));
    routineObj.appendChild(st);

    QDomElement et = xmlDoc.createElement("endTime");
    et.setAttribute("hour", static_cast<int>(endTime.getOre()));
    et.setAttribute("min",  static_cast<int>(endTime.getMin()));
    routineObj.appendChild(et);

    QDomElement sd = xmlDoc.createElement("startDate");
    sd.setAttribute("day",   static_cast<int>(startDate.getDay()));
    sd.setAttribute("month", static_cast<int>(startDate.getMonth()));
    sd.setAttribute("year",  static_cast<int>(startDate.getYear()));
    routineObj.appendChild(st);

    QDomElement ed = xmlDoc.createElement("endDate");
    ed.setAttribute("day",   static_cast<int>(endDate.getDay()));
    ed.setAttribute("month", static_cast<int>(endDate.getMonth()));
    ed.setAttribute("year",  static_cast<int>(endDate.getYear()));
    routineObj.appendChild(ed);

    // history
    QDomElement history = xmlDoc.createElement("history");
    for (bool b : check_history) {
        QDomElement day = xmlDoc.createElement("day");
        day.setAttribute("completed", b ? "true" : "false");
        history.appendChild(day);
    }
    routineObj.appendChild(history);
}

void Routine::fromXml(const QDomElement& routineObj) {
    setName(routineObj.firstChildElement("name").text().toStdString());
    setDesc(routineObj.firstChildElement("description").text().toStdString());
    std::string freqStr = routineObj.attribute("frequency").toStdString();
    if (freqStr == "Daily")        freq = Frequency::Daily;
    else if (freqStr == "Weekly")  freq = Frequency::Weekly;
    else if (freqStr == "Monthly") freq = Frequency::Monthly;
    else if (freqStr == "Yearly")  freq = Frequency::Yearly;


    check = routineObj.attribute("check") == "true";

    QDomElement st = routineObj.firstChildElement("startTime");
    startTime = HourMinute(st.attribute("hour").toUInt(), st.attribute("min").toUInt());

    QDomElement et = routineObj.firstChildElement("endTime");
    endTime = HourMinute(et.attribute("hour").toUInt(), et.attribute("min").toUInt());

    QDomElement sd = routineObj.firstChildElement("startDate");
    startDate.changeDate(sd.attribute("year").toUInt(), sd.attribute("month").toUInt(), sd.attribute("day").toUInt());

    QDomElement ed = routineObj.firstChildElement("endDate");
    endDate.changeDate(ed.attribute("year").toUInt(), ed.attribute("month").toUInt(), ed.attribute("day").toUInt());

    check_history.clear();
    QDomElement history = routineObj.firstChildElement("history");
    QDomElement day  = history.firstChildElement("day");
    while (!day.isNull()) {
        check_history.push_back(day.attribute("completed") == "true");
        day = day.nextSiblingElement("day");
    }

    //QDomElement frequency = routineObj.firstChildElement("frequency");

}