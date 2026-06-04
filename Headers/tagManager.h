#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include "Headers/tag.h"
#include <vector>
#include <string>

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

    tag* newTag(const std::string& name, const QColor& color);
    tag* findTag(const std::string& name) const;
    void removeTag( const std::string& name, ActivityManager& am);
    const std::vector<tag*>& getTags() const;
    tag* getDefaultTag() const;
};


#endif // TAGMANAGER_H
