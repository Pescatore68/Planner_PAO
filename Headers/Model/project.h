#ifndef PROJECT_H
#define PROJECT_H

#include "task.h"
#include <QJsonObject>
#include <QJsonArray>
#include "tagManager.h"

class project : public task {
private:
    std::vector<task*> subtasks;

public:
    project(const string&, const string&, const tag*, const date&, const HourMinute&, const bool& = false);
    ~project();
    const std::vector<task*>& getSubtasks() const;
    const task* getSubtask(unsigned int) const;
    unsigned int nCompleted() const;
    void add(const string&, const string&, const date&, const HourMinute&, const bool& = false);
    unsigned int size() const;
    void remove(task*);
    void remove(unsigned int i);
    float completionPercentage() const;

    //data persistence
    QJsonObject toJson() const override;
    static project* fromJson(const QJsonObject& obj, tagManager& tm);

    QDomElement toXml(QDomDocument& xmlDoc) const override;
    static project* fromXml(const QDomElement& obj, tagManager& tm);

    void accept(ActivityVisitor& v) override;


};

#endif // PROJECT_H
