#ifndef TAG_H
#define TAG_H
#include <string>
//using std::string;

class tag {
private:
    std::string category;
    unsigned int id;
    bool fix;
public:
    tag() {};
    ~tag() {};
    std::string getCat() const {return category;}
    unsigned int getId() const {return id;}
    bool getFix() const {return fix;}
};


#endif // TAG_H
