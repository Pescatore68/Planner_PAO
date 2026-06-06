#include "Headers/ActivityVisitor.h"
#include <Headers/project.h>

project::project(const string& name, const string& description, const tag* Tag, const date& end, const HourMinute& oEnd, const bool& b)
    : task(name, description, Tag, end, oEnd, b) {};


project::~project() {
    for (task* t : subtasks) {
        delete t;
    }
}

const std::vector<task*>& project::getSubtasks() const { return subtasks; }

const task* project::getSubtask(unsigned int idx) const {
    return subtasks[idx];
}

//metodo per barra di completamento
unsigned int project::nCompleted() const {
    unsigned int i=0;
    for(task* t : subtasks) {
        if (t->isCompleted())
            i++;
    }
    return i;
}

void project::add(const string& name, const string& description, const date& end, const HourMinute& oEnd, const bool& b) {
    subtasks.push_back(new task(name, description, this->getTag(), end, oEnd, b));
}

unsigned int project::size() const {
    return subtasks.size();
}

void project::remove(task* rm) {
    for(std::vector<task*>::iterator it=subtasks.begin(); it!=subtasks.end(); ++it) {
        if (*it==rm) {
            delete *it;
            subtasks.erase(it);
            return;
        }
    }
}

void project::remove(unsigned int idx) {
    delete subtasks[idx];
    subtasks.erase(subtasks.begin()+idx);
}

string project::summary() const {
    std::string status = isCompleted() ? "[✓]" : "[ ]";
    std::string result;

    result += status + " " + getName() + "\n";

    if (!getDescription().empty())
        result += "  " + getDescription() + "\n";

    result += "  Scadenza: " + getDeadline().toString()
              + " ore " + getODeadline().toString() + "\n";

    for (unsigned int i=0; i<size(); i++) {
        const task* t = getSubtask(i);
        std::string status = t->isCompleted() ? "[✓]" : "[ ]";
        result += status + " " + t->getName() + "\n";

    }
    return result;
}

float project::completionPercentage() const {
    if (size() == 0) return 0.0f;                        // evita divisione per zero
    return (float)nCompleted() / size() * 100.0f;        // cast a float prima della divisione
}

QJsonObject project::toJson() const{
    QJsonObject obj;
    obj["type"] = "project";
    obj["name"] = QString::fromStdString(getName());
    obj["description"] = QString::fromStdString(getDescription());
    obj["tag"] = QString::fromStdString(getTag()->getName());
    obj["tagColor"] = getTag()->getColor().name();
    obj["deadline"] = QString::fromStdString(getDeadline().toString());
    obj["oDeadline"] = QString::fromStdString(getODeadline().toString());
    obj["check"] = isCompleted();
    QJsonArray subtasks;
    for (unsigned int i = 0; i < size(); i++) {
        QJsonObject sub;
        const task* t = getSubtask(i);
        sub["name"] = QString::fromStdString(t->getName());
        sub["description"] = QString::fromStdString(t->getDescription());
        sub["deadline"] = QString::fromStdString(t->getDeadline().toString());
        sub["oDeadline"] = QString::fromStdString(t->getODeadline().toString());
        sub["check"] = t->isCompleted();
        subtasks.append(sub);
    }
    obj["subtasks"] = subtasks;
    return obj;
}

project* project::fromJson(const QJsonObject& obj, tagManager& tm) {
    auto* p = new project(obj["name"].toString().toStdString(),
                          obj["description"].toString().toStdString(),
                          tm.newTag(obj["tag"].toString().toStdString(), QColor(obj["tagColor"].toString())),
                          date::dateFromString(obj["deadline"].toString().toStdString()),
                          HourMinute::hmFromString(obj["oDeadline"].toString().toStdString()),
                          obj["check"].toBool());

    const QJsonArray subtasks = obj["subtasks"].toArray();
    for (const auto& val : subtasks) {
        QJsonObject sub = val.toObject();
        p->add(sub["name"].toString().toStdString(),
               sub["description"].toString().toStdString(),
               date::dateFromString(sub["deadline"].toString().toStdString()),
               HourMinute::hmFromString(sub["oDeadline"].toString().toStdString()),
               sub["check"].toBool());
    }
    return p;
}
void project::accept(ActivityVisitor& v) { v.visit(*this); }
