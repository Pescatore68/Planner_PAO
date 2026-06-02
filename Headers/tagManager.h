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

    tag* newTag(const std::string& name, const QColor& color, bool fix=false);
    tag* findTag(const std::string& name) const;
    void removeTag( const std::string& name );
    const std::vector<tag*>& getTags() const;
    std::vector<tag*> getFixedTags() const;
    std::vector<tag*> getTemporaryTags() const;
    tag* getDefaultTag() const;
	void rmvTemporaryTag(const std::vector<std::string>& activeTags);
};


#endif // TAGMANAGER_H
