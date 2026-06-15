#include "Headers/Model/AbstractActivity.h"
#include "Headers/Model/ActivityManager.h"


AbstractActivity::AbstractActivity(const string& n, const string& d, const tag* t) : name(n), description(d), t(t) {};
AbstractActivity::AbstractActivity(const string& n, const string& d, ActivityManager& mgr) : name(n), description(d), t() {
    mgr.add(this);
};
AbstractActivity::AbstractActivity(const string& n, const string& d, const tag* t, ActivityManager& mgr) : name(n), description(d), t(t) {
    mgr.add(this);
};

string AbstractActivity::getName() const { return name; }
string AbstractActivity::getDescription() const { return description; }
const tag* AbstractActivity::getTag() const { return t; }

void AbstractActivity::setName(const string& n) { name = n; }
void AbstractActivity::setDesc(const string& d) { description = d; }
void AbstractActivity::setTag(const tag* T) { t = T; }




