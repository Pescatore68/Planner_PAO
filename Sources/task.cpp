#include "Headers/task.h"

task::task(const string& n, const string& d, const tag* t, date e, orario oe) : AbstractActivity(n, d, t), deadline(e), oDeadline(oe) {};
task::task(const string& n, const string& d, date e, orario oe) : AbstractActivity(n, d), deadline(e), oDeadline(oe) {};


date task::getDeadline() const { return deadline; }
orario task::getODeadline() const { return oDeadline; }
void task::setDeadline (date e) { deadline = e; }
void task::setODeadline (orario oe) { oDeadline = oe; }
bool task::isCompleted() const { return check; }
void task::setCompleted (bool b) { check = b; }

