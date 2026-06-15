#include "Headers/Model/Persistence/xmlFormat.h"
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include "Headers/Model/event.h"
#include "Headers/Model/reminder.h"
#include "Headers/Model/routine.h"
#include "Headers/Model/task.h"
#include "Headers/Model/project.h"

namespace xmlFormat {

AbstractActivity* fromXml(const QDomElement& obj, tagManager& tm) {
    std::string type = obj.attribute("type").toStdString();
    if (type == "Event")    return Event::fromXml(obj, tm);
    if (type == "Reminder") return Reminder::fromXml(obj, tm);
    if (type == "Routine")  return Routine::fromXml(obj, tm);
    if (type == "Task")     return task::fromXml(obj, tm);
    if (type == "Project")  return project::fromXml(obj, tm);
    return nullptr;
}

bool saveXml(const ActivityManager& am, const tagManager& tm, const std::string& path) {
    QDomDocument xmlDoc;

    QDomProcessingInstruction header = xmlDoc.createProcessingInstruction(
        "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    xmlDoc.appendChild(header);

    QDomElement root = xmlDoc.createElement("activityManager");
    xmlDoc.appendChild(root);

    //save tag, skip deafultTag(build with constructor
    QDomElement tagsObj = xmlDoc.createElement("tags");
    for (tag* t : tm.getTags()) {
        //if (t != tm.getDefaultTag()){
            QDomElement tagObj = xmlDoc.createElement("tag");
            tagObj.setAttribute("name",  QString::fromStdString(t->getName()));
            tagObj.setAttribute("color", t->getColor().name());
            tagsObj.appendChild(tagObj);
        //}
    }
    root.appendChild(tagsObj);

    // save all activities
    QDomElement activitiesObj = xmlDoc.createElement("activities");
    for (unsigned int i = 0; i < am.size(); i++) {
        activitiesObj.appendChild(am.get(i)->toXml(xmlDoc));
    }
    root.appendChild(activitiesObj);

    //write on file xml
    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return false;
    QTextStream stream(&f);
    stream << xmlDoc.toString(4);
    f.close();

    return true;
}

bool loadXml(ActivityManager& am, tagManager& tm, const std::string& path) {
    QFile f(QString::fromStdString(path));
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return false;

    QDomDocument xmlDoc;
    if (!xmlDoc.setContent(&f)) { f.close(); return false; }
    f.close();

    QDomElement root = xmlDoc.documentElement();

    //load tag and add in tagManager
    QDomElement tagsObj = root.firstChildElement("tags");
    QDomElement tagObj  = tagsObj.firstChildElement("tag");
    while (!tagObj.isNull()) {
        std::string name = tagObj.attribute("name").toStdString();
        QColor color = QColor(tagObj.attribute("color"));
        tm.newTag(name, color);
        tagObj = tagObj.nextSiblingElement("tag");
    }

    //load activities
    QDomElement activitiesObj = root.firstChildElement("activities");
    QDomElement actObj = activitiesObj.firstChildElement("activity");
    while (!actObj.isNull()) {
        AbstractActivity* a = fromXml(actObj, tm);
        if (a) am.add(a);
        actObj = actObj.nextSiblingElement("activity");
    }

    return true;
 }
}