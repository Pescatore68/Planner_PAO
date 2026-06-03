#ifndef ABSTRACTACTIVITY_H
#define ABSTRACTACTIVITY_H
#include "tag.h"
#include <string>

using std::string;

class AbstractActivity{
private:
    string name;
    string description;
    const tag* t;

public:
    AbstractActivity(const string&, const string&);
    AbstractActivity(const string& n, const string& d, const tag* t);
    AbstractActivity(const string&, const string&, std::vector<AbstractActivity*>& );
    AbstractActivity(const string& n, const string& d, const tag* t, std::vector<AbstractActivity*>&);
    virtual ~AbstractActivity()=default;
    string getName() const;
    string getDesc() const;
    const tag* getTag() const;
    void setName(const string&);
    void setDesc(const string&);
    void setTag(const tag*);
    virtual string summary() const = 0;
    virtual bool isExpired() const = 0;
    //virtual void display() const = 0; non so se serve

};

#endif
