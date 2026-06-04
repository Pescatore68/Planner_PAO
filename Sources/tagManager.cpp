#include "Headers/tagManager.h"
#include "Headers/tag.h"
#include "Headers/ActivityManager.h"
tagManager::tagManager(){
    defaultTag=new tag("no tag", QColor(128,128,128),true);
    tags.push_back(defaultTag);
}

tagManager::~tagManager() {
    for (tag* t : tags) {
        delete t;
    }
    tags.clear();
}

tag* tagManager::newTag(const std::string& name, const QColor& color, bool fix) {
    if(findTag(name))
        return findTag(name);
    tag* t = new tag(name, color, fix);
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
std::vector<tag*> tagManager::getFixedTags() const {
    std::vector<tag*> fixed;
    for (auto t : tags) {
        // dosen't show defaultag
        if (t->isFix() && t != defaultTag) {
            fixed.push_back(t);
        }
    }
    return fixed;
}
std::vector<tag*> tagManager::getTemporaryTags() const {
    std::vector<tag*> temporary;
    for (auto t : tags) {
        if (!t->isFix()) {
            temporary.push_back(t);
        }
    }
    return temporary;
}
void tagManager::rmvTemporaryTag(ActivityManager& am) {
    // tag in use
    vector<string> usedTags;
    for (unsigned int i = 0; i < am.size(); i++) {
        const tag* t = am.get(i)->getTag();
        if (t) {
            auto f = std::find(usedTags.begin(), usedTags.end(), t->getName());
            if (f == usedTags.end()) {
                usedTags.push_back(t->getName());
            }
        }
    }

    // remove temporary tag not in use
    auto it = tags.begin();
    while (it != tags.end()) {
        tag* t = *it;
        if (!t->isFix() && std::find(usedTags.begin(), usedTags.end(), t->getName()) == usedTags.end())
        {
            removeTag(t->getName(), am);
            it = tags.begin();
        }
        else {
            ++it;
        }
    }
}