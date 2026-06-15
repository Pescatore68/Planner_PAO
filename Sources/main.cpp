#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "Headers/mainwindow.h"
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include "Headers/Model/Persistence/JSONformat.h"
#include "Headers/Model/Persistence/xmlFormat.h"
#include <QApplication>
#include "Headers/UI/StyleHelper.h"

//file json and xml
static const std::string JSON_PATH = "activities.json";
static const std::string XML_PATH  = "activities.xml";

//save format
static const std::string SAVE_FORMAT = "xml"; //"json" or "xml"

//load data
static bool loadData(ActivityManager& am, tagManager& tm)
{
    //tries both format
    if (SAVE_FORMAT == "json") {
        if (QFileInfo::exists(QString::fromStdString(JSON_PATH))) {
            qDebug() << "[Load] Caricamento JSON da:" << QString::fromStdString(JSON_PATH);
            if (JSONformat::loadJson(am, tm, JSON_PATH)) {
                qDebug() << "[Load] JSON caricato con successo. Attività:" << am.size();
                return true;
            }
            qWarning() << "[Load] Errore nel caricamento JSON.";
        }
        // Fallback su XML
        if (QFileInfo::exists(QString::fromStdString(XML_PATH))) {
            qDebug() << "[Load] Fallback XML da:" << QString::fromStdString(XML_PATH);
            if (xmlFormat::loadXml(am, tm, XML_PATH)) {
                qDebug() << "[Load] XML caricato con successo. Attività:" << am.size();
                return true;
            }
            qWarning() << "[Load] Errore nel caricamento XML.";
        }
    } else {
        if (QFileInfo::exists(QString::fromStdString(XML_PATH))) {
            qDebug() << "[Load] Caricamento XML da:" << QString::fromStdString(XML_PATH);
            if (xmlFormat::loadXml(am, tm, XML_PATH)) {
                qDebug() << "[Load] XML caricato con successo. Attività:" << am.size();
                return true;
            }
            qWarning() << "[Load] Errore nel caricamento XML.";
        }
        // Fallback su JSON
        if (QFileInfo::exists(QString::fromStdString(JSON_PATH))) {
            qDebug() << "[Load] Fallback JSON da:" << QString::fromStdString(JSON_PATH);
            if (JSONformat::loadJson(am, tm, JSON_PATH)) {
                qDebug() << "[Load] JSON caricato con successo. Attività:" << am.size();
                return true;
            }
            qWarning() << "[Load] Errore nel caricamento JSON.";
        }
    }

    qDebug() << "[Load] Nessun file di salvataggio trovato. Avvio con dati vuoti.";
    return false;
}


static void saveData(const ActivityManager& am, const tagManager& tm)
{
    bool okJson = JSONformat::saveJson(am, tm, JSON_PATH);
    bool okXml  = xmlFormat::saveXml(am, tm, XML_PATH);

    if (okJson)
        qDebug() << "[Save] JSON salvato in:" << QString::fromStdString(JSON_PATH);
    else
        qWarning() << "[Save] Errore nel salvataggio JSON.";

    if (okXml)
        qDebug() << "[Save] XML salvato in:" << QString::fromStdString(XML_PATH);
    else
        qWarning() << "[Save] Errore nel salvataggio XML.";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet(StyleHelper::getApplicationStyle());

    //set working directory
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug() << "[Init] Working dir:" << QDir::currentPath();

    ActivityManager am;
    tagManager      tm;

    loadData(am, tm);

    MainWindow w(am, tm);
    w.show();

    int exitCode = app.exec();

    //save both xml and json format
    saveData(am, tm);

    return exitCode;
}