#include "Headers/task.h"
#include "Headers/tagManager.h"
#include "Headers/Visitor/ActivityVisitor.h"

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

bool task::isActive(const date& d) const{
    return d==getDeadline();
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
                        tm.findTag(obj["tag"].toString().toStdString()),
                        date::dateFromString(obj["deadline"].toString().toStdString()),
                        HourMinute::hmFromString(obj["oDeadline"].toString().toStdString()));
    tk->setCompleted(obj["check"].toBool());
    return tk;
}

QDomElement task::toXml(QDomDocument& xmlDoc) const {
    QDomElement taskObj = xmlDoc.createElement("activity");
    taskObj.setAttribute("type",  "Task");
    taskObj.setAttribute("check", check ? "true" : "false");

    QDomElement nameObj = xmlDoc.createElement("name");
    nameObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getName())));
    taskObj.appendChild(nameObj);

    QDomElement descObj = xmlDoc.createElement("description");
    descObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getDescription())));
    taskObj.appendChild(descObj);

    QDomElement tagObj = xmlDoc.createElement("tag");
    tagObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(getTag()->getName())));
    taskObj.appendChild(tagObj);

    QDomElement tagColorObj = xmlDoc.createElement("tagColor");
    tagColorObj.appendChild(xmlDoc.createTextNode(getTag()->getColor().name()));
    taskObj.appendChild(tagColorObj);

    QDomElement deadlineObj = xmlDoc.createElement("deadline");
    deadlineObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(deadline.toString())));
    taskObj.appendChild(deadlineObj);

    QDomElement oDeadlineObj = xmlDoc.createElement("oDeadline");
    oDeadlineObj.appendChild(xmlDoc.createTextNode(QString::fromStdString(oDeadline.toString())));
    taskObj.appendChild(oDeadlineObj);

    return taskObj;
}

task* task::fromXml(const QDomElement& obj, tagManager& tm) {
    std::string name = obj.firstChildElement("name").text().toStdString();
    std::string desc = obj.firstChildElement("description").text().toStdString();

    tag* t = tm.newTag(obj.firstChildElement("tag").text().toStdString(), QColor(obj.firstChildElement("tagColor").text()));

    date deadline = date::dateFromString(obj.firstChildElement("deadline").text().toStdString());
    HourMinute oDeadline = HourMinute::hmFromString(obj.firstChildElement("oDeadline").text().toStdString());

    task* tk = new task(name, desc, t, deadline, oDeadline);
    tk->setCompleted(obj.attribute("check") == "true");
    return tk;
}

void task::accept(ActivityVisitor& v) { v.visit(*this); }
