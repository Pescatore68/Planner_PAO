#include "Headers/UI/ActivitySearch.h"

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

