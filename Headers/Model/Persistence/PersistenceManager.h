#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include <QWidget>
#include <string>

class PersistenceManager {
public:
    // load and save in dialog window
    static bool loadDataWithDialog(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    static void saveDataWithDialog(const ActivityManager& am, const tagManager& tm, QWidget* parent = nullptr);

    // json and xml method
    static bool loadFromFile(ActivityManager& am, tagManager& tm, const std::string& filePath);
    static void saveToFile(const ActivityManager& am, const tagManager& tm, const std::string& filePath);
};

#endif // PERSISTENCEMANAGER_H
