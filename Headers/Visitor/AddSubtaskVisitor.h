#ifndef ADDSUBTASKVISITOR_H
#define ADDSUBTASKVISITOR_H


class AddSubtaskVisitor : public ActivityVisitor
{
private:
    std::string title;
    std::string description;
    date deadline;
    HourMinute time;

public:
    AddSubtaskVisitor(
        const std::string& t,
        const std::string& d,
        const date& dl,
        const HourMinute& hm
        )
        : title(t),
        description(d),
        deadline(dl),
        time(hm)
    {}

    void visit(task&) override
    {
        // una task normale non può contenere subtasks
    }

    void visit(project& p) override
    {
        p.add(
            title,
            description,
            deadline,
            time,
            false
            );
    }
};

#endif // ADDSUBTASKVISITOR_H
