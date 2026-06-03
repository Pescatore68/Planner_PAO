#include <Headers/project.h>

//metodo per barra di completamento
unsigned int project::nCompleted() {
    unsigned int i=0;
    for(task* t : p) {
        if (t->isCompleted())
            i++;
    }
    return i;
};

//add
void project::add(string n, date e, orario oe, tag t) {
    task(n, )
}
void project::add(string n, date e, orario oe);
//size
unsigned int project::size();
//remove
void project::remove(const tag& t);
void project::remove(unsigned int i);
bool project::isExpired() const override {

}
string project::summary() const override;
float project::completionPercentage() const; // nCompleted() / size() * 100 → per la barra
