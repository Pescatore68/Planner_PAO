#ifndef ABSTRACTACTIVITY_H
#define ABSTRACTACTIVITY_H
#include "tag.h"
#include <string>

using std::string;

class AbstractActivity{
private:
    string name;
    unsigned int priority;
    tag t;
public:
    AbstractActivity()=default;
    virtual ~AbstractActivity()=default;
    virtual string getName() {return name;}
    virtual unsigned int getPriority() {return priority;}
    virtual tag getTag() {return t;}

};

#endif
