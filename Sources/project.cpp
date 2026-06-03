#include <Headers/project.h>

//metodo per barra di completamento
unsigned int project::nCompleted() {
    unsigned int i=0;
    for(task* t : subtasks) {
        if (t->isCompleted())
            i++;
    }
    return i;
};

void project::add(const string& name, const string& description, const tag* Tag, const date& end, const HourMinute& oEnd) {
    subtasks.push_back(new task(name, description, Tag, end, oEnd));
}
void project::add(const string& name, const string& description, const date& end, const HourMinute& oEnd) {
    subtasks.push_back(new task(name, description, end, oEnd));
}

unsigned int project::size() {
    return subtasks.size();
}

void project::remove(task* rm) {
    for(std::vector<task*>::iterator it=subtasks.begin(); it!=subtasks.end(); ++it) {
        if (*it==rm) {
            delete *it;
            subtasks.erase(it);
        }
    }
}

void project::remove(unsigned int i) {

}
bool project::isExpired() const override {

}
string project::summary() const override;
float project::completionPercentage() const; // nCompleted() / size() * 100 → per la barra
