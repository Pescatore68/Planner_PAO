#include "Headers/routine.h"
#include "Headers/Visitor/ActivityVisitor.h"
#include <QJsonArray>

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

void Routine::setCheckHistory(const std::vector<bool>& h) {
    check_history = h;
}

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

std::string Routine::FrequencyToString() const {
    if (freq == Frequency::Daily) return "Daily";
    if (freq == Frequency::Weekly) return "Weekly";
    if (freq == Frequency::Monthly) return "Monthly";
    if (freq == Frequency::Yearly) return "Yearly";
	return "Not implemented";
}

Routine::Frequency Routine::freqFromString(const std::string& s) {
    if (s == "Daily") return Routine::Frequency::Daily;
    if (s == "Weekly") return Routine::Frequency::Weekly;
    if (s == "Monthly") return Routine::Frequency::Monthly;
    if (s == "Yearly") return Routine::Frequency::Yearly;
    return Routine::Frequency::Daily;
}

QJsonObject Routine::toJson() const {
    QJsonObject obj;
    obj["type"] = "routine";
    obj["name"] = QString::fromStdString(getName());
    obj["description"] = QString::fromStdString(getDescription());
    obj["tag"] = QString::fromStdString(getTag()->getName());
    obj["tagColor"] = getTag()->getColor().name();
    obj["startDate"] = QString::fromStdString(getStartDate().toString());
    obj["endDate"] = QString::fromStdString(getEndDate().toString());
    obj["startTime"] = QString::fromStdString(getStartTime().toString());
    obj["endTime"] = QString::fromStdString(getEndTime().toString());
    obj["frequency"] = QString::fromStdString(FrequencyToString());
    obj["check"] = getCheck();

    QJsonArray historyArray;
    for (bool value : getcheckHistory()) {
        historyArray.append(value);
    }
    obj["history"] = historyArray;
    return obj;
}

Routine* Routine::fromJson(const QJsonObject& obj, tagManager& tm) {
    auto* r = new Routine(obj["name"].toString().toStdString(),
                          obj["description"].toString().toStdString(),
                          tm.newTag(obj["tag"].toString().toStdString(), QColor(obj["tagColor"].toString())),
                          HourMinute::hmFromString(obj["startTime"].toString().toStdString()),
                          HourMinute::hmFromString(obj["endTime"].toString().toStdString()),
                          date::dateFromString(obj["startDate"].toString().toStdString()),
                          date::dateFromString(obj["endDate"].toString().toStdString()),
                          freqFromString(obj["frequency"].toString().toStdString()));

    r->setCheck(obj["check"].toBool());
    QJsonArray historyArray = obj["history"].toArray();

    std::vector<bool> history;
    for (const auto& value : historyArray) {
        history.push_back(value.toBool());
    }

    r->setCheckHistory(history);
    return r;
}


QDomElement Routine::toXml(QDomDocument& xmlDoc) const {
    QDomElement routineObj = xmlDoc.createElement("activity");
    routineObj.setAttribute("type",      "Routine");
    routineObj.setAttribute("frequency", QString::fromStdString(FrequencyToString()));
    routineObj.setAttribute("check",     getCheck() ? "true" : "false");

    QDomElement nameObj = xmlDoc.createElement("name");
    nameObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    routineObj.appendChild(nameObj);

    QDomElement descObj = xmlDoc.createElement("description");
    descObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    routineObj.appendChild(descObj);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getTag()->getName())));
    routineObj.appendChild(tagObj);

    QDomElement tagColorObj = xmlDoc.createElement("tagColor");
    tagColorObj.appendChild(xmlDoc.createTextNode(getTag()->getColor().name()));
    routineObj.appendChild(tagColorObj);

    QDomElement startDateObj = xmlDoc.createElement("startDate");
    startDateObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getStartDate().toString())));
    routineObj.appendChild(startDateObj);

    QDomElement endDateObj = xmlDoc.createElement("endDate");
    endDateObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getEndDate().toString())));
    routineObj.appendChild(endDateObj);

    QDomElement startTimeObj = xmlDoc.createElement("startTime");
    startTimeObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getStartTime().toString())));
    routineObj.appendChild(startTimeObj);

    QDomElement endTimeObj = xmlDoc.createElement("endTime");
    endTimeObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getEndTime().toString())));
    routineObj.appendChild(endTimeObj);

    // history
    QDomElement historyObj = xmlDoc.createElement("history");
    for (bool b : check_history) {
        QDomElement dayObj = xmlDoc.createElement("day");
        dayObj.setAttribute("completed", b ? "true" : "false");
        historyObj.appendChild(dayObj);
    }
    routineObj.appendChild(historyObj);

    return routineObj;
}

Routine* Routine::fromXml(const QDomElement& obj, tagManager& tm) {
    auto* r = new Routine(obj.firstChildElement("name").text().toStdString(),obj.firstChildElement("description").text().toStdString(),
                        tm.newTag(obj.firstChildElement("tag").text().toStdString(), QColor(obj.firstChildElement("tagColor").text())),
                        HourMinute::hmFromString(obj.firstChildElement("startTime").text().toStdString()),
                        HourMinute::hmFromString(obj.firstChildElement("endTime").text().toStdString()),
                        date::dateFromString(obj.firstChildElement("startDate").text().toStdString()),
                        date::dateFromString(obj.firstChildElement("endDate").text().toStdString()),
                        freqFromString(obj.attribute("frequency").toStdString()));

    r->setCheck(obj.attribute("check") == "true");

    // carica history
    std::vector<bool> history;
    QDomElement historyObj = obj.firstChildElement("history");
    QDomElement dayObj     = historyObj.firstChildElement("day");
    while (!dayObj.isNull()) {
        history.push_back(dayObj.attribute("completed") == "true");
        dayObj = dayObj.nextSiblingElement("day");
    }
    r->setCheckHistory(history);

    return r;
}

void Routine::accept(ActivityVisitor& v) { v.visit(*this); }
