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
static const std::string SAVE_FORMAT = "xml"; // "json" oppure "xml"

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
        // ── Global & Window Structure ──────────────────────────────────────────
        // Forziamo lo sfondo nativo e impediamo la trasparenza sui container principali
        "QMainWindow, QDialog, QMessageBox, QStackedWidget, .MonthWidget, .calendar, .TaskWidget, .TagWidget, .AddDialog {"
        "  background-color: #F4EBE1 !important;" /* Ripristina il panna di sfondo */
        "  color: #4A3E4D;"
        "  font-family: 'Segoe UI', -apple-system, BlinkMacSystemFont, Roboto, sans-serif;"
        "  font-size: 14px;"
        "}"

        // Riconfiguriamo i QWidget generici per non rompere le finestre principali
        "QWidget {"
        "  color: #4A3E4D;"
        "}"

        // ── Elegant Buttons ─────────────────────────────────────────────────────
        "QPushButton {"
        "  background-color: #D5A5AA;"
        "  color: #FFFFFF;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 8px 20px;"
        "  font-weight: 600;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #83A6BF;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #61849D;"
        "}"
        "QPushButton:checked {"
        "  background-color: #7293A9;"
        "  color: #FFFFFF;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #E2D3CD;"
        "  color: #A6959B;"
        "}"

        // ── Inputs (LineEdits & SpinBoxes) ──────────────────────────────────────
        "QLineEdit, QDateEdit, QTimeEdit {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 6px;"
        "  padding: 6px 12px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus, QTimeEdit:focus {"
        "  border: 2px solid #83A6BF;"
        "  padding: 5px 11px;"
        "}"
        "QLineEdit::placeholder {"
        "  color: #A89A9F;"
        "  font-style: italic;"
        "}"

        // ── ComboBox ───────────────────────────────────────────────────────────
        "QComboBox {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 6px;"
        "  padding: 6px 30px 6px 12px;"
        "}"
        "QComboBox:hover {"
        "  border: 1px solid #83A6BF;"
        "}"
        "QComboBox::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 25px;"
        "  border-left: 1px solid #E6DBCF;"
        "}"
        "QComboBox QAbstractItemView {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 6px;"
        "  selection-background-color: #EAB2C2;"
        "  selection-color: #4A3E4D;"
        "  padding: 4px;"
        "}"

        // ── Advanced Custom Calendar Styles ─────────────────────────────────────
        "QCalendarWidget {"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 8px;"
        "}"
        "QCalendarWidget QAbstractItemView:enabled {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  selection-background-color: #83A6BF;"
        "  selection-color: #FFFFFF;"
        "  border-radius: 0px;"
        "}"
        "QCalendarWidget QAbstractItemView:disabled {"
        "  color: #C2B6A8;"
        "}"
        "QCalendarWidget QWidget#qt_calendar_navigationbar {"
        "  background-color: #D5A5AA;"
        "  border-top-left-radius: 8px;"
        "  border-top-right-radius: 8px;"
        "  border-bottom: 2px solid #C29399;"
        "}"
        "QCalendarWidget QToolButton {"
        "  background-color: transparent;"
        "  color: #FFFFFF;"
        "  font-weight: bold;"
        "  border-radius: 4px;"
        "  margin: 4px;"
        "  padding: 2px 8px;"
        "}"
        "QCalendarWidget QToolButton:hover {"
        "  background-color: rgba(255, 255, 255, 0.25);"
        "}"
        "QCalendarWidget QToolButton:pressed {"
        "  background-color: rgba(0, 0, 0, 0.1);"
        "}"
        "QCalendarWidget QWidget {"
        "  alternate-background-color: #F0E4D8;"
        "  color: #7D6B7F;"
        "}"

        // ── Data Views (Lists & Trees) ──────────────────────────────────────────
        "QListWidget, QTreeWidget {"
        "  background-color: #FFFFFF;"
        "  color: #4A3E4D;"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 8px;"
        "  padding: 4px;"
        "}"
        "QListWidget::item, QTreeWidget::item {"
        "  padding: 8px 10px;"
        "  border-radius: 4px;"
        "  margin-bottom: 2px;"
        "}"
        "QListWidget::item:hover, QTreeWidget::item:hover {"
        "  background-color: #F5E6E8;"
        "  color: #4A3E4D;"
        "}"
        "QListWidget::item:selected, QTreeWidget::item:selected {"
        "  background-color: #83A6BF;"
        "  color: #FFFFFF;"
        "}"

        // ── CheckBox Customization ──────────────────────────────────────────────
        "QCheckBox {"
        "  spacing: 8px;"
        "  color: #4A3E4D;"
        "}"
        "QCheckBox::indicator {"
        "  width: 18px;"
        "  height: 18px;"
        "  border: 1px solid #D1C4B4;"
        "  border-radius: 4px;"
        "  background-color: #FFFFFF;"
        "}"
        "QCheckBox::indicator:hover {"
        "  border: 1px solid #83A6BF;"
        "}"
        "QCheckBox::indicator:checked {"
        "  background-color: #83A6BF;"
        "  border: 1px solid #83A6BF;"
        "}"

        // ── ScrollBars ────────────────────────────────────
        "QScrollBar:vertical {"
        "  background: transparent;"
        "  width: 8px;"
        "  margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #C2B6A8;"
        "  border-radius: 4px;"
        "  min-height: 25px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "  background: #A69787;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical,"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "  background: none;"
        "  height: 0px;"
        "}"
        "QScrollBar:horizontal {"
        "  background: transparent;"
        "  height: 8px;"
        "}"
        "QScrollBar::handle:horizontal {"
        "  background: #C2B6A8;"
        "  border-radius: 4px;"
        "}"

        // ── Tables & Tree Headers ────────────────────────────────────────────────
        "QHeaderView::section {"
        "  background-color: #D5A5AA;"
        "  color: #FFFFFF;"
        "  border: none;"
        "  padding: 8px;"
        "  font-weight: bold;"
        "}"

        // ── Clean Separators & Tools ───────────────────────────────────────────
        "QFrame[frameShape='4'], QFrame[frameShape='5'] {"
        "  color: #E6DBCF;"
        "}"
        "QToolTip {"
        "  background-color: #4A3E4D;"
        "  color: #F4EBE1;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 5px 10px;"
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