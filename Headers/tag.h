#ifndef TAG_H
#define TAG_H
#include <string>
#include <QColor>


class tag {
private:
    unsigned int id;
    std::string name;
    bool fix;
    QColor color;

public:
    tag() {}; // da ridefinire!!
    ~tag() {};
    std::string getName() const;
    QColor getColor() const;
    bool getFix() const;
    void setName(const std::string&);
    void setFix(const bool&);
    void setColor(const QColor&);

};

//tag::*tag tag::defaultTag const {return }


#endif // TAG_H
