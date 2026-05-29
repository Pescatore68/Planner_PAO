#include "Headers/tag.h"

std::string tag::getName() const {return name;}
QColor tag::getColor() const {return color;}
bool tag::getFix() const {return fix;}
void tag::setName(const std::string& n) {name=n;}
void setFix(const bool& b) { fix=b; }
void setColor(const QColor& c) { color=c;}
