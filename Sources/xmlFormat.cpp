#include "Headers/xmlFormat.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

void xmlFormat::save(const std::string& filepath, const ActivityManager& am, const tagManager& tm) const {
    QDomDocument xmlDoc;

    QDomProcessingInstruction header = xmlDoc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    xmlDoc.appendChild(header);

    //root
    QDomElement root = xmlDoc.createElement("activityManager");
    xmlDoc.appendChild(root);

    //save tag, skip deafultTag(build with constructor
    QDomElement tagsObj = xmlDoc.createElement("tags");
    for (tag* t : tm.getTags()) {
        if (t != tm.getDefaultTag()) {
            QDomElement tagObj = xmlDoc.createElement("tag");
            t->toXml(tagObj, xmlDoc);
            tagsObj.appendChild(tagObj);
            tagsObj.appendChild(tagObj);
        }
    }
    root.appendChild(tagsObj);

    // save all activities

    QDomElement activitiesObj = xmlDoc.createElement("activities");
    for (unsigned int i = 0; i < am.size(); i++) {
        QDomElement actObj = xmlDoc.createElement("activity");
        am.get(i)->toXml(actObj, xmlDoc);
        activitiesObj.appendChild(actObj);
    }
    root.appendChild(activitiesObj);

    // write on file xml

    QFile file(QString::fromStdString(filepath));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << xmlDoc.toString(4);
        file.close();
    }
}

void xmlFormat::load(const std::string& filepath, ActivityManager& am, tagManager& tm) const {
    QFile file(QString::fromStdString(filepath));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QDomDocument xmlDoc;
    if (!xmlDoc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

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
        QString type = actObj.attribute("type");
        AbstractActivity* a = nullptr;

        // tag of activities
        QString tagName = actObj.firstChildElement("tag").text();
        tag* t = tm.findTag(tagName.toStdString());
        if (!t) t = tm.getDefaultTag();

        // use the right type of activities
        if (type == "Event")
            a = new Event("", "", t, date(1,1,2000), date(1,1,2000));
        else if (type == "Reminder")
            a = new Reminder("", "", t, date(1,1,2000), HourMinute(0,0), "");
        else if (type == "Routine")
            a = new Routine("", "", t, HourMinute(0,0), HourMinute(0,0), date(1,1,2000), date(1,1,2000), Routine::Frequency::Daily);
        else if (type == "Task")
            a = new task("", "", t,  date(1,1,2000), HourMinute(0,0));
        /*else if (type == "Project")
            a = new project("", "", t, date(1,1,2000), HourMinute(0,0));
        */
        if (a) {
            a->fromXml(actObj);
            am.add(a);
        }

        actObj = actObj.nextSiblingElement("activity");
    }
}



