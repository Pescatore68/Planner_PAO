#include "Headers/ActivityManager.h"
#include "Headers/routine.h"
#include "Headers/tagManager.h"

ActivityManager::~ActivityManager() {
    for ( auto *a : activities ) {
        delete a;
    }
}

void ActivityManager::add(AbstractActivity* a) {
    activities.push_back(a); //ach! è this->activities.push_back(a)
}

void ActivityManager::remove(unsigned int idx) {
    if (idx >= activities.size()) return;
    delete activities[idx];
    activities.erase(activities.begin() + idx);
}

AbstractActivity* ActivityManager::get(unsigned int idx) const {
    if (idx >= activities.size()) return nullptr;
    return activities[idx];
}

unsigned int ActivityManager::size() const {
    return activities.size();
}

void ActivityManager::fit() {
    activities.shrink_to_fit();
}

std::vector<AbstractActivity*> ActivityManager::getOnDate(const date& d) const {
    std::vector<AbstractActivity*> result;
    for (auto a : activities) {
        for (auto a : activities) {
            if (a->isActive(d)) result.push_back(a);
        }
        return result;
    }
