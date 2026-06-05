#ifndef HOURMINUTE_H
#define HOURMINUTE_H
#include <string>
using std::string;

class HourMinute{ // time creava problemi di conflitto
private:
    unsigned int min;
public:
    HourMinute (unsigned int h=0, unsigned int m=0);
    unsigned int getMin() const;
    unsigned int getOre() const;
    static HourMinute now();
    //ridefinizione dell'operatore >
    friend bool operator>(const HourMinute&, const HourMinute&);
    friend bool operator==(const HourMinute&, const HourMinute&);
    string toString() const;
};

#endif // HOURMINUTE_H
