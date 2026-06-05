#include "Headers/tag.h"

//constructor
tag::tag(const std::string& name, const QColor& color): name(name), color(color){}

std::string tag::getName() const {return name;}
QColor tag::getColor() const {return color;}

void tag::setName(const std::string& n) {name=n;}
void tag::setColor(const QColor& c) { color=c;}

bool tag::operator==(const tag& t) const {
    return name == t.getName();
}