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
        // ── Struttura Finestre Aziendali (Sfondo Platino Pulito) ─────────────────
        "QMainWindow, QDialog, QMessageBox, QStackedWidget, .MonthWidget, .calendar, .TaskWidget, .TagWidget, .AddDialog {"
        "  background-color: #e7ecef !important;" /* Sfondo istituzionale --platinum */
        "  color: #274c77;"                      /* Testo primario rigoroso --dusk-blue */
        "  font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;"
        "  font-size: 13px;"
        "}"
        "QWidget { color: #274c77; }"

        // ── Pulsanti Solidi e Tecnici (Angoli Squadrati 6px) ───────────────────
        "QPushButton {"
        "  background-color: #274c77;"           /* --dusk-blue */
        "  color: #e7ecef;"                      /* Testo --platinum */
        "  border: none;"
        "  border-radius: 6px;"                  /* Curve ridotte per un design serio */
        "  padding: 6px 16px;"
        "  font-weight: 600;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #6096ba;"           /* --steel-blue all'hover */
        "}"
        "QPushButton:pressed {"
        "  background-color: #a3cef1;"           /* --icy-blue alla pressione */
        "  color: #274c77;"
        "}"
        "QPushButton:checked {"
        "  background-color: #1e3a5f;"
        "  color: #FFFFFF;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #d1d5db;"
        "  color: #8b8c89;"
        "}"

        // ── Campi Di Input Quadrati e Puliti ──────────────────────────────────
        "QLineEdit, QDateEdit, QTimeEdit {"
        "  background-color: #FFFFFF;"
        "  color: #274c77;"
        "  border: 1px solid #8b8c89;"           /* Bordo --grey-olive nitido */
        "  border-radius: 4px;"
        "  padding: 5px 10px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
        "  border: 2px solid #274c77;"           /* Il focus marca l'input in blu scuro */
        "  padding: 4px 9px;"
        "}"
        "QLineEdit::placeholder {"
        "  color: #8b8c89;"
        "}"

        // ── ComboBox Gestionale ────────────────────────────────────────────────
        "QComboBox {"
        "  background-color: #FFFFFF;"
        "  color: #274c77;"
        "  border: 1px solid #8b8c89;"
        "  border-radius: 4px;"
        "  padding: 5px 26px 5px 10px;"
        "}"
        "QComboBox:hover {"
        "  border-color: #274c77;"
        "}"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 24px;"
        "  border-left: 1px solid #e7ecef;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #FFFFFF;"
        "  color: #274c77;"
        "  border: 1px solid #274c77;"
        "  border-radius: 4px;"
        "  selection-background-color: #a3cef1;" /* Selezione pulita in --icy-blue */
        "  selection-color: #274c77;"
        "}"

        // ── Calendario Strutturato ──────────────────────────────────────────────
        "QCalendarWidget {"
        "  border: 1px solid #8b8c89;"
        "  border-radius: 6px;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "  background-color: #FFFFFF;"
        "  color: #274c77;"
        "  border-radius: 0px;"
        "  selection-background-color: #274c77;" /* Giorno attivo: Blu Scuro */
        "  selection-color: #e7ecef;"
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "  color: #d1d5db;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "  background-color: #274c77;"
        "  border-top-left-radius: 6px;"
        "  border-top-right-radius: 6px;"
        "  border-bottom: 1px solid #6096ba;"
        "}"
        "QCalendarWidget QToolButton {"
        "  background-color: transparent;"
        "  color: #e7ecef;"
        "  font-weight: bold;"
        "  border-radius: 4px;"
        "  margin: 2px;"
        "}"
        "QCalendarWidget QToolButton:hover {"
        "  background-color: #6096ba;"
        "}"

        // ── CheckBox Rigidi ad Alto Contrasto ─────────────────────────────────
        "QCheckBox {"
        "  spacing: 8px;"
        "  color: #274c77;"
        "  font-weight: 500;"
        "}"
        "QCheckBox::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "  border: 1px solid #274c77;"           /* Linea esterna blu scuro fissa */
        "  border-radius: 3px;"                  /* Angolo quasi perfettamente dritto */
        "  background-color: #FFFFFF;"
        "}"
        "QCheckBox::indicator:hover {"
        "  border-color: #6096ba;"
        "  background-color: #e7ecef;"
        "}"
        "QCheckBox::indicator:checked {"
        "  background-color: #274c77;"           /* Riempimento blu solido */
        "  border-color: #274c77;"
        "  image: url(data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 24 24' fill='none' stroke='%23e7ecef' stroke-width='4' stroke-linecap='round' stroke-linejoin='round'><polyline points='20 6 9 17 4 12'></polyline></svg>);" /* Spunta interna in --platinum nitida */
        "}"

        // ── Data Views (Tabelle, Liste e Alberi) ───────────────────────────────
        "QListWidget, QTreeWidget {"
        "  background-color: #FFFFFF;"
        "  color: #274c77;"
        "  border: 1px solid #8b8c89;"
        "  border-radius: 6px;"
        "}"
        "QListWidget::item, QTreeWidget::item {"
        "  border-bottom: 1px solid #e7ecef;"     /* Separazione rigorosa ad elementi */
        "}"
        "QListWidget::item:hover, QTreeWidget::item:hover {"
        "  background-color: #e7ecef;"
        "  color: #274c77;"
        "}"
        "QListWidget::item:selected, QTreeWidget::item:selected {"
        "  background-color: #6096ba;"           /* Evidenziatore strutturato --steel-blue */
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