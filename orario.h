#ifndef ORARIO_H
#define ORARIO_H

class orario{ // time creava problemi di conflitto
private:
    unsigned int min;
public:
    orario (unsigned int m, unsigned int o);
    unsigned int getMin() const;
    unsigned int getOre() const;
};

#endif // ORARIO_H
