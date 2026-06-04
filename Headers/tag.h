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
    tag(const std::string&, const QColor&, bool fix=false);// default: temporary tag
    ~tag() = default;

    std::string getName() const;
    QColor getColor() const;
    bool isFix() const;

    void setName(const std::string& name);
    void setColor(const QColor& color);
    void setFix(const bool& fix);
    bool operator == (const tag& t) const; //use to filter by tag

};


#endif // TAG_H
