#include "Headers/Model/Persistence/PersistenceManager.h"
#include "Headers/Model/Persistence/JSONformat.h"
#include "Headers/Model/Persistence/xmlFormat.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

bool PersistenceManager::loadDataWithDialog(ActivityManager& am, tagManager& tm, QWidget* parent) {
    QString filePath = QFileDialog::getOpenFileName(
        parent,
        "Open Activity File",
        "",
        "Data Files (*.json *.xml);;JSON (*.json);;XML (*.xml)"
        );

    if (filePath.isEmpty()) {
        qDebug() << "[Load] Loading canceled by the user.";
        return false;
    }

    return loadFromFile(am, tm, filePath.toStdString());
}

void PersistenceManager::saveDataWithDialog(const ActivityManager& am, const tagManager& tm, QWidget* parent) {
    QString filePath = QFileDialog::getSaveFileName(
        parent,
        "Save Activity File",
        "activities.xml",
        "XML (*.xml);;JSON (*.json)"
        );

    if (filePath.isEmpty()) {
        qDebug() << "[Save] Saving canceled by the user.";
        return;
    }

    saveToFile(am, tm, filePath.toStdString());
}

bool PersistenceManager::loadFromFile(ActivityManager& am, tagManager& tm, const std::string& filePath) {
    QString qPath = QString::fromStdString(filePath);

    if (qPath.endsWith(".json", Qt::CaseInsensitive)) {
        qDebug() << "[Load] Reading JSON format from:" << qPath;
        return JSONformat::loadJson(am, tm, filePath);
    }
    else if (qPath.endsWith(".xml", Qt::CaseInsensitive)) {
        qDebug() << "[Load] Reading XML format from:" << qPath;
        return xmlFormat::loadXml(am, tm, filePath);
    }

    qWarning() << "[Load] Unsupported file format:" << qPath;
    return false;
}

void PersistenceManager::saveToFile(const ActivityManager& am, const tagManager& tm, const std::string& filePath) {
    QString qPath = QString::fromStdString(filePath);

    if (qPath.endsWith(".json", Qt::CaseInsensitive)) {
        if (JSONformat::saveJson(am, tm, filePath)) {
            qDebug() << "[Save] JSON successfully saved to:" << qPath;
        }
    }
    else if (qPath.endsWith(".xml", Qt::CaseInsensitive)) {
        if (xmlFormat::saveXml(am, tm, filePath)) {
            qDebug() << "[Save] XML successfully saved to:" << qPath;
        }
    } else {
        qWarning() << "[Save] Unable to save. Unrecognized file format.";
    }
}