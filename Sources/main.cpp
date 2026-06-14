#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "Headers/mainwindow.h"
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"
#include "Headers/JSONformat.h"
#include "Headers/xmlFormat.h"

// ─── Percorsi dei file di salvataggio ────────────────────────────────────────
// Puoi cambiare i nomi / percorsi a piacere.
// I file vengono creati nella stessa cartella dell'eseguibile se non esiste
// un percorso assoluto.
static const std::string JSON_PATH = "activities.json";
static const std::string XML_PATH  = "activities.xml";

// ─── Formato preferito ───────────────────────────────────────────────────────
// Cambia a "xml" per usare XML come formato principale.
static const std::string SAVE_FORMAT = "json"; // "json" oppure "xml"

// ─── Caricamento dati ────────────────────────────────────────────────────────
static bool loadData(ActivityManager& am, tagManager& tm)
{
    // Proviamo prima il formato principale, poi l'altro come fallback.
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

// ─── Salvataggio dati ────────────────────────────────────────────────────────
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

// ─── main ────────────────────────────────────────────────────────────────────
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Imposta la directory di lavoro alla cartella dell'eseguibile
    // così i file JSON/XML vengono trovati / scritti sempre nello stesso posto.
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    qDebug() << "[Init] Working dir:" << QDir::currentPath();

    ActivityManager am;
    tagManager      tm;

    // ── Carica i dati salvati (se esistono) ──────────────────────────────────
    loadData(am, tm);

    // ── Finestra principale ───────────────────────────────────────────────────
    MainWindow w(am, tm);
    w.show();

    // ── Ciclo eventi Qt ──────────────────────────────────────────────────────
    int exitCode = app.exec();

    // ── Salva i dati all'uscita (sia JSON che XML per avere entrambi) ─────────
    saveData(am, tm);

    return exitCode;
}