#include "Headers/ActivityManager.h"
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

AbstractActivity* ActivityManager::get(unsigned int idx) {
    if (idx >= activities.size()) return nullptr;
    return activities[idx];
}

unsigned int ActivityManager::size() {
    return activities.size();
}

void ActivityManager::fit() {
    activities.shrink_to_fit();
}

