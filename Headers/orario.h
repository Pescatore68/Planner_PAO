#ifndef ORARIO_H
#define ORARIO_H

class orario{ // time creava problemi di conflitto
private:
    unsigned int min;
public:
    orario (unsigned int h, unsigned int m);
    unsigned int getMin() const;
    unsigned int getOre() const;
    static orario now();
};

#endif // ORARIO_H
