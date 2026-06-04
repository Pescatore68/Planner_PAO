#ifndef HOURMINUTE_H
#define HOURMINUTE_H

class HourMinute{ // time creava problemi di conflitto
private:
    unsigned int min;
public:
    HourMinute (unsigned int h, unsigned int m);
    unsigned int getMin() const;
    unsigned int getOre() const;
    static HourMinute now();
    //ridefinizione dell'operatore >
    friend bool operator>(const HourMinute&, const HourMinute&);
};

#endif // HOURMINUTE_H
