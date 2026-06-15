#ifndef TASK_H
#define TASK_H
#include "Headers/Model/AbstractActivity.h"
#include "Headers/Model/date.h"
#include "Headers/Model/HourMinute.h"
#include "Headers/Model/tag.h"

class task : public AbstractActivity {
private:
    date deadline;
    HourMinute oDeadline;
    bool check;
public:
    task(const string&, const string&, const tag*, const date&, const HourMinute&, const bool& = false);
    date getDeadline() const;
    HourMinute getODeadline() const;
    bool isCompleted() const;
    void setCompleted (const bool&);
    void setDeadline (const date&);
    void setODeadline (const HourMinute&);
    bool isExpired() const override;
    bool isActive(const date& d) const override;

    //data persistence
    QJsonObject toJson() const override;
    static task* fromJson(const QJsonObject&, tagManager&);

    QDomElement toXml(QDomDocument& xmlDoc) const override;
    static task* fromXml(const QDomElement& obj, tagManager& tm);

    void accept(ActivityVisitor& v) override;

};

#endif // TASK_H
