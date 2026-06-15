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

    app.setStyleSheet(
        // ── Struttura Globale Finestre (Sfondo Panna Naturale) ───────────────────
        "QMainWindow, QDialog, QMessageBox, QStackedWidget, .MonthWidget, .calendar, .TaskWidget, .TagWidget, .AddDialog {"
        "  background-color: #F4EBE1 !important;" /* Panna di fondo */
        "  color: #4A3E4D;"                      /* Testo Vinaccia scuro per contrasto eccezionale */
        "  font-family: 'Segoe UI', -apple-system, sans-serif;"
        "  font-size: 14px;"
        "}"
        "QWidget { color: #4A3E4D; }"

        // ── Bottoni Arrotondati Morbidi ─────────────────────────────────────────
        "QPushButton {"
        "  background-color: #D5A5AA;"           /* --petal-frost alternativo */
        "  color: #FFFFFF;"
        "  border: none;"
        "  border-radius: 12px;"                 /* Curve morbide coerenti */
        "  padding: 8px 22px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #99c1de;"           /* --powder-blue all'hover */
        "}"
        "QPushButton:pressed {"
        "  background-color: #bcd4e6;"           /* --pale-sky alla pressione */
        "}"
        "QPushButton:checked {"
        "  background-color: #4A3E4D;"
        "  color: #FFFFFF;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #f0efeb;"           /* --parchment */
        "  color: #C2B6A8;"
        "}"

        // ── Campi di Input (Bordi Ben Definiti) ──────────────────────────────────
        "QLineEdit, QDateEdit, QTimeEdit {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 2px solid #eddcd2;"           /* Bordo --powder-petal visibile */
        "  border-radius: 10px;"                 /* Geometria arrotondata coordinata */
        "  padding: 6px 12px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
        "  border: 2px solid #99c1de;"           /* Focus illumina in --powder-blue */
        "}"
        "QLineEdit::placeholder {"
        "  color: #A6959B;"
        "  font-style: italic;"
        "}"

        // ── ComboBox Pastello ───────────────────────────────────────────────────
        "QComboBox {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 2px solid #eddcd2;"
        "  border-radius: 10px;"
        "  padding: 6px 30px 6px 12px;"
        "}"
        "QComboBox:hover {"
        "  border-color: #99c1de;"
        "}"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 28px;"
        "  border-left: 1px solid #f0efeb;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 2px solid #eddcd2;"
        "  border-radius: 10px;"
        "  selection-background-color: #fad2e1;" /* --petal-frost per la selezione */
        "  selection-color: #4A3E4D;"
        "}"

        // ── Calendario Avanzato Stondato ─────────────────────────────────────────
        "QCalendarWidget {"
        "  border: 2px solid #eddcd2;"
        "  border-radius: 14px;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border-radius: 10px;"
        "  selection-background-color: #99c1de;" /* Selezione giorno: --powder-blue */
        "  selection-color: #FFFFFF;"
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "  color: #C2B6A8;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "  background-color: #eddcd2;"
        "  border-top-left-radius: 12px;"
        "  border-top-right-radius: 12px;"
        "  border-bottom: 2px solid #fff1e6;"
        "}"
        "QCalendarWidget QToolButton {"
        "  background-color: transparent;"
        "  color: #4A3E4D;"
        "  font-weight: bold;"
        "  border-radius: 6px;"
        "  margin: 4px;"
        "}"
        "QCalendarWidget QToolButton:hover {"
        "  background-color: rgba(153, 193, 222, 0.4);"
        "}"

        // ── CheckBox Custom (Visibilità ad Alto Contrasto) ──────────────────────
        "QCheckBox {"
        "  spacing: 10px;"
        "  color: #4A3E4D;"
        "  font-weight: 600;"
        "}"
        "QCheckBox::indicator {"
        "  width: 20px;"
        "  height: 20px;"
        "  border: 2px solid #4A3E4D;"           /* Contorno scuro marcato e visibile */
        "  border-radius: 6px;"                  /* Angoli arrotondati a quadratino morbido */
        "  background-color: #FFFFFF;"           /* Sfondo bianco pulito */
        "}"
        "QCheckBox::indicator:hover {"
        "  background-color: #fff1e6;"           /* Illuminazione all'hover */
        "  border-color: #99c1de;"
        "}"
        "QCheckBox::indicator:checked {"
        "  background-color: #99c1de;"           /* Si riempie di azzurro quando attivo */
        "  border-color: #4A3E4D;"
        "  image: url(data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='none' stroke='%234A3E4D' stroke-width='4' stroke-linecap='round' stroke-linejoin='round'><polyline points='20 6 9 17 4 12'></polyline></svg>);" /* Segno di spunta scuro incorporato */
        "}"

        // ── Liste e Viste Dati ──────────────────────────────────────────────────
        "QListWidget, QTreeWidget {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 2px solid #eddcd2;"
        "  border-radius: 12px;"
        "}"
        "QListWidget::item:hover, QTreeWidget::item:hover {"
        "  background-color: #fff1e6;"
        "  color: #4A3E4D;"
        "}"
        "QListWidget::item:selected, QTreeWidget::item:selected {"
        "  background-color: #99c1de;"
        "  color: #FFFFFF;"
        "}"
        );

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