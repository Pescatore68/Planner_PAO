#ifndef DATE_H
#define DATE_H

class date {
private:
    unsigned int day;
    unsigned int month;
    unsigned int year;
public:
    date(unsigned int d, unsigned int m, unsigned int y) : day(d), month(m), year(y) {

    }
    unsigned int getDay() {return day;}
    unsigned int getMonth() {return month;}
    unsigned int getYear() {return year;}
};
#endif // DATE_H
