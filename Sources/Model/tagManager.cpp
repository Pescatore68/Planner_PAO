#include "Headers/Model/tagManager.h"
#include "Headers/Model/tag.h"
#include "Headers/Model/ActivityManager.h"
tagManager::tagManager(){
    defaultTag=new tag("no tag", QColor(128,128,128));
    tags.push_back(defaultTag);
}

tagManager::~tagManager() {
    for (tag* t : tags) {
        delete t;
    }
    tags.clear();
}

tag* tagManager::newTag(const std::string& name, const QColor& color) {
    if(findTag(name))
        return findTag(name);
    tag* t = new tag(name, color);
    tags.push_back(t);
    return t;
}

tag* tagManager::findTag(const std::string& name) const {
    for (tag* t : tags) {
        if(t->getName() == name) {return t;}
    }
    return nullptr;
}
void tagManager::removeTag(const std::string& name, ActivityManager& am) {
    for (auto it = tags.begin(); it != tags.end(); ++it) {
        tag* t = *it;
        if (t->getName() == name) {
            if (t != defaultTag) {
                // Update activity with this tag
                for (unsigned int i = 0; i < am.size(); i++) {
                    AbstractActivity* a = am.get(i);
                    if (a->getTag() && a->getTag()->getName() == name) {
                        a->setTag(defaultTag);
                    }
                }
                tags.erase(it);
                delete t;
            }
            return; // trovato, esci in ogni caso
        }
    }
}
const std::vector<tag*>& tagManager::getTags() const {
    return tags;
}

tag* tagManager::getDefaultTag() const {
    return defaultTag;
}