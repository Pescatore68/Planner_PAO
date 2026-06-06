#ifndef TAG_H
#define TAG_H
#include <string>
#include <QColor>
#include <QDomDocument>
#include <QDomElement>

class tag {
private:
    std::string name;
    QColor color;

public:
    tag(const std::string&, const QColor&);// default: temporary tag
    ~tag() = default;

    std::string getName() const;
    QColor getColor() const;


    void setName(const std::string& name);
    void setColor(const QColor& color);
    bool operator == (const tag& t) const; //use to filter by tag

    //data persistence
    void toXml(QDomElement& actOBj, QDomDocument& xmlDoc) const;
    void fromXml(const QDomElement& actObj);
};


#endif // TAG_H
