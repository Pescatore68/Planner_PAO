#include "Headers/date.h"
#include <ctime>

date::date(unsigned int d, unsigned int m, unsigned int y) : day(d), month(m), year(y) {}
unsigned int date::getDay() const {return day;}
unsigned int date::getMonth() const {return month;}
unsigned int date::getYear() const {return year;}
void date::setYear(const unsigned int& y) {year=y;}
void date::setMonth(const unsigned int& m) {month=m;}
void date::setDay(const unsigned int& d) {day=d;}
void date::changeDate(const unsigned int& y, const unsigned int& m, const unsigned int& d) {
    setYear(y);
    setMonth(m);
    setDay(d);
}

date date::today() {
    time_t t = time(nullptr);
    tm local = *localtime(&t);
    return date(local.tm_mday, local.tm_mon + 1, local.tm_year + 1900);
}

unsigned int date::dayOfWeek() const {
    unsigned int d = day;
    unsigned int m = month;
    unsigned int y = year;
    if (m < 3) { m += 12; y--; }
    return (d + (13*(m+1))/5 + y + y/4 - y/100 + y/400) % 7;
}

bool operator==(const date& n, const date& m) {
    return n.getDay() == m.getDay() &&
           n.getMonth() == m.getMonth() &&
           n.getYear() == m.getYear();
}

bool operator>(const date& n, const date& m) {
    if (n.getYear() != m.getYear())
        return n.getYear() > m.getYear();
    if (n.getMonth() != m.getMonth())
        return n.getMonth() > m.getMonth();
    return n.getDay() > m.getDay();
}

std::string date::toString() const {
    std::string d = (day < 10 ? "0" : "") + std::to_string(day);
    std::string m = (month < 10 ? "0" : "") + std::to_string(month);
    return d + "/" + m + "/" + std::to_string(year);
}
