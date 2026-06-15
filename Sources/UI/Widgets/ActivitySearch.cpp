#include "Headers/UI/Widgets/ActivitySearch.h"

std::vector<AbstractActivity*> ActivitySearch::findByName(const ActivityManager& am, const std::string& query) {
    std::vector<AbstractActivity*> results;

    for (unsigned int i = 0; i < am.size(); ++i) {
        AbstractActivity* act = am.get(i);
        if (!act) continue;

        // Ricerca della sottostringa nel nome
        if (act->getName().find(query) != std::string::npos) {
            results.push_back(act);
        }
    }
    return results;
}

std::vector<AbstractActivity*> ActivitySearch::findByTag(const ActivityManager& am, const std::string& tagName) {
    std::vector<AbstractActivity*> results;
    if (tagName == "Tutti i tag" || tagName.empty()) {
        for(unsigned int i=0; i<am.size(); ++i) results.push_back(am.get(i));
        return results;
    }
    for(unsigned int i=0; i<am.size(); ++i) {
        AbstractActivity* act = am.get(i);
        if (act && act->getTag() && act->getTag()->getName() == tagName) {
            results.push_back(act);
        }
    }
    return results;
}