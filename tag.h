#ifndef TAG_H
#define TAG_H
#include <string>

using std::string;

class tag {
private:
    string category;
    unsigned int id;
    bool fix;
public:
    tag()=default;
    ~tag()=default;
    string getCat() {return category;}


};


#endif // TAG_H
