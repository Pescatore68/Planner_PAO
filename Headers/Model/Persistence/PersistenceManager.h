#ifndef PERSISTENCEMANAGER_H
#define PERSISTENCEMANAGER_H

#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include <QWidget>
#include <string>

class PersistenceManager {
public:
    // Caricamento e salvataggio espliciti tramite Finestra di Dialogo
    static bool loadDataWithDialog(ActivityManager& am, tagManager& tm, QWidget* parent = nullptr);
    static void saveDataWithDialog(const ActivityManager& am, const tagManager& tm, QWidget* parent = nullptr);

    // Metodi di utilità interna per processare il file scelto
    static bool loadFromFile(ActivityManager& am, tagManager& tm, const std::string& filePath);
    static void saveToFile(const ActivityManager& am, const tagManager& tm, const std::string& filePath);
};

#endif // PERSISTENCEMANAGER_H
