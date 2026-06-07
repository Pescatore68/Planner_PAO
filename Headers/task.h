#ifndef TASK_H
#define TASK_H
#include "Headers/AbstractActivity.h"
#include "Headers/date.h"
#include "Headers/HourMinute.h"
#include "Headers/tag.h"

class task : public AbstractActivity {
private:
    date deadline;
    HourMinute oDeadline;
    bool check;
public:
    task(const string&, const string&, const tag*, const date&, const HourMinute&, const bool& = false);
    //task(const string&, const string&, const date&, const HourMinute&);
    date getDeadline() const;
    HourMinute getODeadline() const;
    bool isCompleted() const;
    void setCompleted (const bool&);
    void setDeadline (const date&);
    void setODeadline (const HourMinute&);
    bool isExpired() const override;
    string summary() const override;

    //data persistence
    QJsonObject toJson() const override;
    static task* fromJson(const QJsonObject&, tagManager&);

    QDomElement toXml(QDomDocument& xmlDoc) const override;
    static task* fromXml(const QDomElement& obj, tagManager& tm);

    void accept(ActivityVisitor& v) override;

};

#endif // TASK_H
