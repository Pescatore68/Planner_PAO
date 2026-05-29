#ifndef TAGMANAGER_H
#define TAGMANAGER_H
#include "Headers/tag.h"
#include <vector>

class tagManager {
private:
    std::vector<tag*> tags;
    tag* defaultTag;

    //impedisco duplicati
    tagManager();
    tagManager(const tagManager&) = delete; //rendo non disponibile il costruttore di copia
    tagManager& operator=(const tagManager&) = delete; //"" l'operatore di assegnazione
public:

    ~tagManager();

    static tagManager& retPpt();

    tag* createTag(const )


};


#endif // TAGMANAGER_H
