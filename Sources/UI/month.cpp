#include "Headers/UI/month.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "Headers/date.h"
#include "Headers/AbstractActivity.h"
#include "qtextformat.h"

MonthWidget::MonthWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am) // Inizializza il riferimento al backend
{
    setup();
}

void MonthWidget::setup()
{
    // Layout principale ORIZZONTALE (divide la finestra in sinistra e destra)
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(15);

    // 1. SINISTRA: Il calendario standard di Qt
    calendar = new QCalendarWidget(this);
    mainLayout->addWidget(calendar, 2); // Il numero '2' lo fa apparire più largo

    // 2. DESTRA: Pannello con il titolo e la lista dei task
    QWidget* sidePanel = new QWidget(this);
    QVBoxLayout* sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* titleLabel = new QLabel("Attività del giorno:", this);
    // Opzionale: rendiamo il titolo un po' più carino ed evidente
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    activityList = new QListWidget(this); // La lista visiva

    sideLayout->addWidget(titleLabel);
    sideLayout->addWidget(activityList);

    mainLayout->addWidget(sidePanel, 1); // Il numero '1' lo fa apparire più stretto

    // 3. CONNESSIONI
    // Quando l'utente clicca una data, aggiorna la lista interna
    connect(calendar, &QCalendarWidget::clicked, this, &MonthWidget::onDateChanged);

    // Mantiene la tua connessione originale verso il widget padre (calendar)
    connect(calendar, &QCalendarWidget::clicked, this, &MonthWidget::dateClicked);

    // Mostra subito le attività del giorno selezionato all'avvio dell'app
    onDateChanged(calendar->selectedDate());
}

// Questo slot viene chiamato ogni volta che l'utente clicca sul calendario
void MonthWidget::onDateChanged(const QDate& qd)
{
    activityList->clear(); // Svuota la lista precedente

    date backendDate(qd.day(), qd.month(), qd.year());
    auto activities = am.getOnDate(backendDate);

    // Se non ci sono attività, lasciamo la lista pulita e vuota (oppure un messaggio discreto)
    if (activities.empty()) {
        return;
    }

    // Popoliamo la lista con i nomi reali delle attività
    for (AbstractActivity* act : activities) {
        if (act) {
            QString name = QString::fromStdString(act->getName());

            // Usiamo un piccolo pallino o un prefisso elegante per ogni attività
            // Questo assicura che Qt disegni correttamente la riga nel widget!
            activityList->addItem("• " + name);
        }
    }
}
void MonthWidget::updateCalendarView()
{
    // 1. Resetta i vecchi formati accumulati
    calendar->setDateTextFormat(QDate(), QTextCharFormat());

    QTextCharFormat impegnatoFormat;
    impegnatoFormat.setBackground(QColor(230, 242, 255));
    impegnatoFormat.setForeground(Qt::blue);
    impegnatoFormat.setFontWeight(QFont::Bold);

    int year = calendar->yearShown();
    int month = calendar->monthShown();

    QDate firstDay(year, month, 1);
    int daysInMonth = firstDay.daysInMonth();

    // 2. Ricoloriamo tutti i giorni del mese corrente
    for (int day = 1; day <= daysInMonth; ++day) {
        QDate qd(year, month, day);
        date backendDate(qd.day(), qd.month(), qd.year()); // Ordine (Giorno, Mese, Anno) del tuo backend

        auto activities = am.getOnDate(backendDate);

        if (!activities.empty()) {
            calendar->setDateTextFormat(qd, impegnatoFormat);
        } else {
            calendar->setDateTextFormat(qd, QTextCharFormat());
        }
    }

    // ─── IL TRUCCO RISOLUTIVO È QUI ──────────────────────────────────────────
    // Prendiamo la data che l'utente sta visualizzando selezionata nel calendario
    QDate dataSelezionata = calendar->selectedDate();

    // Forziamo manualmente la chiamata a onDateChanged passandogli la data corrente.
    // Questo costringe la QListWidget a svuotarsi, interrogare il backend e
    // ripopolarsi immediatamente con i nuovi dati appena creati!
    this->onDateChanged(dataSelezionata);
}