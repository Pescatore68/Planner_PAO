#ifndef DATE_H
#define DATE_H

class date {
private:
    unsigned int day;
    unsigned int month;
    unsigned int year;
public:
    date(unsigned int d, unsigned int m, unsigned int y);
    unsigned int getDay();
    unsigned int getMonth();
    unsigned int getYear();
    void setYear(const unsigned int& y);
    void setMonth(const unsigned int& m);
    void setDay(const unsigned int& d);
    void changeDate(const unsigned int& y, const unsigned int& m, const unsigned int& d);
    static date today();

};
#endif // DATE_H
