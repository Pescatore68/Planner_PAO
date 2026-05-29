#ifndef ABSTRACTACTIVITY_H
#define ABSTRACTACTIVITY_H
#include "Headers/tag.h"
#include <string>

using std::string;

class AbstractActivity{
private:
    unsigned int id;
    string name;
    string description;
    tag tag;

public:
    AbstractActivity(int id, const string&, const string&);
    virtual ~AbstractActivity()=default;
    virtual string getName() const;



};

#endif
