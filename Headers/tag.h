#ifndef TAG_H
#define TAG_H
#include <string>
#include <QColor>


class tag {
private:
    std::string name;
    bool fix; //fix or temporary tag
    QColor color;

public:
    tag() {const std:: string& name const Qcolor& color, bool fix=false}; // default: tempoary tag
    ~tag() = default;

    std::string getName() const;
    QColor getColor() const;
    bool getFix() const;

    void setName(const std::string& name);
    void setColor(const QColor& color);
    void setFix(const bool& fix);
    bool operator == (const tag& t) const; //use to filter by tag

};

//tag::*tag tag::defaultTag const {return }


#endif // TAG_H
