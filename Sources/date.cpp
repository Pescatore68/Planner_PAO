#include "Headers/date.h"
#include <ctime>

date::date(unsigned int d, unsigned int m, unsigned int y) : day(d), month(m), year(y) {}
unsigned int date::getDay() {return day;}
unsigned int date::getMonth() {return month;}
unsigned int date::getYear() {return year;}
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

