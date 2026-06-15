#ifndef TAGMANAGER_H
#define TAGMANAGER_H

class ActivityManager;

#include "Headers/Model/tag.h"
#include <vector>
#include <string>

class tagManager {
private:
    std::vector<tag*> tags;
    tag* defaultTag;

    tagManager(const tagManager&) = delete;
    tagManager& operator=(const tagManager&) = delete;

public:

    tagManager();
    ~tagManager();

    tag* newTag(const std::string& name, const QColor& color);
    tag* findTag(const std::string& name) const;
    void removeTag( const std::string& name, ActivityManager& am);
    const std::vector<tag*>& getTags() const;
    tag* getDefaultTag() const;
};


#endif // TAGMANAGER_H
