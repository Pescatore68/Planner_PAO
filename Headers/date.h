#ifndef DATE_H
#define DATE_H
#include <string>
using std::string;

class date {
private:
    unsigned int day;
    unsigned int month;
    unsigned int year;
public:
    date(unsigned int d, unsigned int m, unsigned int y);
    unsigned int getDay() const;
    unsigned int getMonth() const;
    unsigned int getYear() const;
    void setYear(const unsigned int& y);
    void setMonth(const unsigned int& m);
    void setDay(const unsigned int& d);
    void changeDate(const unsigned int& y, const unsigned int& m, const unsigned int& d);
    static date today();
    friend bool operator>(const date&, const date&);
    string toString() const;
};
#endif // DATE_H
