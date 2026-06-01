#include "Headers/AbstractActivity.h"

AbstractActivity::AbstractActivity(const string& n, const string& d) : name(n), description(d), t() {};
AbstractActivity::AbstractActivity(const string& n, const string& d, const tag& t) : name(n), description(d), t(t) {};

string AbstractActivity::getName() const { return name; }
string AbstractActivity::getDesc() const { return description; }
tag AbstractActivity::getTag() const { return t; }

void AbstractActivity::setName(const string& n) { name = n; }
void AbstractActivity::setDesc(const string& d) { description = d; }
void AbstractActivity::setTag(const tag& T) { t = T; }




