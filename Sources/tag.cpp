#include "Headers/tag.h"

//constructor
tag::tag(const std::string& name, const QColor& color, bool fix): name(name), color(color), fix(fix) {}

std::string tag::getName() const {return name;}
QColor tag::getColor() const {return color;}
bool tag::isFix() const {return fix;}

void tag::setName(const std::string& n) {name=n;}
void tag::setColor(const QColor& c) { color=c;}
void tag::setFix(const bool& b) { fix=b; }

bool tag::operator==(const tag& t) const {
    return name == t.getName();
}