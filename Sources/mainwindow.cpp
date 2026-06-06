#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/ActivityManager.h"
#include "Headers/task.h"
#include "Headers/date.h"
#include "Headers/HourMinute.h"
#include "Headers/routine.h"
#include "Headers/project.h"
#include "Headers/reminder.h"
#include "Headers/event.h"
#include "Headers/tagManager.h"
#include "Headers/JSONformat.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ── tag ──────────────────────────────────────
    tagManager tm;
    tag* uni   = tm.newTag("uni",   QColor(128, 128, 128));
    tag* sport = tm.newTag("sport", QColor(255, 100, 100));
    tag* def   = tm.getDefaultTag();

    // ── activity ─────────────────────────────────
    ActivityManager am;

    // task con tag
    am.add(new task("Studiare analisi", "Capitolo 3",
                    uni, date(31, 12, 2025), HourMinute(9, 0)));

    // task con tag default
    am.add(new task("Comprare latte", "Al supermercato",
                    def, date(5, 6, 2026), HourMinute(10, 0)));

    // routine
    am.add(new Routine("Palestra", "Allenamento",
                       sport,
                       HourMinute(16, 0), HourMinute(18, 0),
                       date(1, 1, 2025), date(31, 12, 2026),
                       Routine::Frequency::Monthly));

    // reminder — usa location come terzo campo stringa
    am.add(new Reminder("Dentista", "Controllo annuale",
                        def,
                        date(15, 7, 2026), HourMinute(11, 30),
                        "Via Roma 10"));

    // event — più giorni con orario
    am.add(new Event("Conferenza Qt", "Qt World Summit",
                     uni,
                     date(1, 10, 2026), date(3, 10, 2026),
                     HourMinute(9, 0), HourMinute(18, 0),
                     "Berlino"));

    // event — un giorno senza orario (all day)
    am.add(new Event("Compleanno Marco", "",
                     def,
                     date(20, 8, 2026), date(20, 8, 2026),
                     ""));

    // project con subtask
    project* p = new project("Tesi", "Laurea triennale",
                             uni,
                             date(30, 6, 2027), HourMinute(23, 59));
    p->add("Introduzione",  "Scrivere intro",    date(1,  3, 2027), HourMinute(12, 0), true);
    p->add("Capitolo 1",    "Stato dell'arte",   date(1,  4, 2027), HourMinute(12, 0), false);
    p->add("Capitolo 2",    "Implementazione",   date(1,  5, 2027), HourMinute(12, 0), false);
    p->add("Conclusioni",   "Scrivere conclusioni", date(1, 6, 2027), HourMinute(12, 0), false);
    am.add(p);

    // ── save / load JSON ─────────────────────────
    const std::string path = "/tmp/planner_test.json";
    bool saved  = JSONformat::saveJson(am, path);

    ActivityManager am2;
    bool loaded = JSONformat::loadJson(am2, tm, path);

    // ── UI ───────────────────────────────────────
    QWidget*     container = new QWidget();
    QVBoxLayout* layout    = new QVBoxLayout(container);
    layout->setSpacing(8);

    auto addLabel = [&](const std::string& text, const QString& color = "") {
        QLabel* l = new QLabel(QString::fromStdString(text), container);
        l->setWordWrap(true);
        l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        if (!color.isEmpty())
            l->setStyleSheet("color: " + color + "; font-weight: bold;");
        layout->addWidget(l);
    };

    auto addSeparator = [&](const std::string& title) {
        addLabel("── " + title + " ──────────────────", "#333333");
    };

    addSeparator("ORIGINALI");
    for (unsigned int i = 0; i < am.size(); i++)
        addLabel(am.get(i)->summary());

    addLabel(saved  ? "✓ JSON salvato in " + path
                   : "✗ Errore salvataggio JSON",
             saved  ? "green" : "red");

    addLabel(loaded ? "✓ JSON ricaricato correttamente"
                    : "✗ Errore caricamento JSON",
             loaded ? "green" : "red");

    addSeparator("RICARICATE DA JSON");
    for (unsigned int i = 0; i < am2.size(); i++)
        addLabel(am2.get(i)->summary());

    // ── scroll area ──────────────────────────────
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidget(container);
    scroll->setWidgetResizable(true);
    setCentralWidget(scroll);
    resize(650, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}