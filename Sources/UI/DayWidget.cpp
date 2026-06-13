#include "Headers/UI/DayWidget.h"
#include "Headers/AbstractActivity.h"
#include "Headers/ActivityManager.h"
#include "Headers/event.h"
#include "Headers/reminder.h"
#include "Headers/routine.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include "Headers/Visitor/DisplayVisitor.h"
#include "Headers/HourMinute.h"
#include "Headers/Visitor/DayWidgetVisitor.h"

#include <QPainter>
#include <QPainterPath>
#include <QScrollBar>
#include <QEnterEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QFont>
#include <QFontMetrics>
#include <QTime>
#include <QDate>
#include <QTimer>
#include <cmath>
#include <algorithm>

// ═══════════════════════════════════════════════════════════════════════════════
//  ActivityBlock
// ═══════════════════════════════════════════════════════════════════════════════

ActivityBlock::ActivityBlock(AbstractActivity* a, QWidget* parent)
    : QWidget(parent), activity(a)
{
    setCursor(Qt::PointingHandCursor);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void ActivityBlock::paintEvent(QPaintEvent*)
{
    if (!activity) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Colore base dal tag
    QColor base = activity->getTag()
                      ? activity->getTag()->getColor()
                      : QColor(100, 149, 237); // cornflower blue fallback

    // Sfondo con leggera trasparenza – più scuro all'hover
    QColor bg = base;
    bg.setAlpha(hovered ? 230 : 200);

    // Bordo sinistro colorato (accent strip)
    const int STRIP = 3;
    QColor strip = base.darker(130);

    // Raggio angoli arrotondati
    const int R = 4;

    QRectF rect(0, 0, width(), height());

    // Disegna corpo arrotondato
    QPainterPath path;
    path.addRoundedRect(rect, R, R);
    p.fillPath(path, bg);

    // Strip sinistra colorata
    p.fillRect(QRectF(0, R, STRIP, height() - 2 * R), strip);
    QPainterPath stripPath;
    stripPath.addRoundedRect(QRectF(0, 0, STRIP, height()), R, R);
    p.fillPath(stripPath, strip);

    // Bordo sottile
    p.setPen(QPen(base.darker(115), 0.5));
    p.drawPath(path);

    // Testo: nome attività centrato
    QString name = QString::fromStdString(activity->getName());
    QFont font = p.font();

    // Adatta dimensione testo all'altezza disponibile
    int fontSize = (height() < 20) ? 7 : (height() < 32) ? 8 : 9;
    font.setPixelSize(fontSize);
    font.setBold(true);
    p.setFont(font);

    // Colore testo: bianco se sfondo scuro, scuro altrimenti
    double luminance = 0.299 * base.red() + 0.587 * base.green() + 0.114 * base.blue();
    p.setPen(luminance < 140 ? QColor(255,255,255,230) : QColor(30,30,30,220));

    QRectF textRect(STRIP + 4, 2, width() - STRIP - 6, height() - 4);

    if (height() < 16) {
        // troppo piccolo: solo un punto
        return;
    }

    p.drawText(textRect,
               Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap,
               name);
}

void ActivityBlock::enterEvent(QEnterEvent* e)
{
    hovered = true;
    update();
    QWidget::enterEvent(e);
}

void ActivityBlock::leaveEvent(QEvent* e)
{
    hovered = false;
    update();
    QWidget::leaveEvent(e);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  TimeGrid
// ═══════════════════════════════════════════════════════════════════════════════

TimeGrid::TimeGrid(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(TOTAL_HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

int TimeGrid::minutesToY(int totalMinutes)
{
    return (totalMinutes * HOUR_HEIGHT) / 60;
}

void TimeGrid::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);

    const int W = width();

    // Sfondo bianco/off-white
    p.fillRect(rect(), QColor(255, 255, 255));

    QFont labelFont;
    labelFont.setPixelSize(10);
    p.setFont(labelFont);

    for (int h = 0; h < TOTAL_HOURS; h++) {
        int y = h * HOUR_HEIGHT;

        // ── Linea intera ora ─────────────────────────────────────
        p.setPen(QPen(QColor(210, 210, 215), 0.8));          // grigio chiaro iOS
        p.drawLine(LABEL_WIDTH, y, W, y);

        // ── Etichetta ora ─────────────────────────────────────────
        QString label = (h == 0)
                            ? QString()                        // mezzanotte: no label
                            : QString("%1:%2")
                                  .arg(h, 2, 10, QChar(' '))
                                  .arg(0, 2, 10, QChar('0'));

        if (!label.isEmpty()) {
            p.setPen(QColor(140, 140, 148));                   // grigio testo iOS
            QRectF labelRect(2, y - 7, LABEL_WIDTH - 6, 14);
            p.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
        }

        // ── Linea mezzora (più sottile e più chiara) ──────────────
        int yHalf = y + HOUR_HEIGHT / 2;
        p.setPen(QPen(QColor(230, 230, 234), 0.5));           // quasi invisibile
        p.drawLine(LABEL_WIDTH, yHalf, W, yHalf);
    }

    // Linea verticale separatrice ore / contenuto
    p.setPen(QPen(QColor(210, 210, 215), 0.8));
    p.drawLine(LABEL_WIDTH, 0, LABEL_WIDTH, height());
}

// ═══════════════════════════════════════════════════════════════════════════════
//  DayWidget
// ═══════════════════════════════════════════════════════════════════════════════

DayWidget::DayWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am), currentDate(date::today())
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    buildHeader();
    buildAllDayArea();
    buildScrollArea();

    // Scroll all'ora corrente al primo avvio
    QTimer::singleShot(0, this, [this] {
        int currentHour = QTime::currentTime().hour();
        int scrollTo    = std::max(0, TimeGrid::minutesToY(currentHour * 60) - 120);
        scrollArea->verticalScrollBar()->setValue(scrollTo);
    });
}

// ─── Header ──────────────────────────────────────────────────────────────────
void DayWidget::buildHeader()
{
    headerLabel = new QLabel(this);
    headerLabel->setAlignment(Qt::AlignCenter);

    QFont f = headerLabel->font();
    f.setPixelSize(15);
    f.setBold(false);
    headerLabel->setFont(f);
    headerLabel->setFixedHeight(44);
    headerLabel->setStyleSheet(
        "QLabel {"
        "  color: #1c1c1e;"
        "  background-color: #f2f2f7;"
        "  border-bottom: 1px solid #d1d1d6;"
        "  padding: 0 12px;"
        "}"
        );

    mainLayout->addWidget(headerLabel);
}

// ─── ScrollArea + TimeGrid ────────────────────────────────────────────────────
void DayWidget::buildScrollArea()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(false);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollBar:vertical {"
        "  width: 6px;"
        "  background: transparent;"
        "}"
        "QScrollBar::handle:vertical {"
        "  background: #c7c7cc;"
        "  border-radius: 3px;"
        "  min-height: 30px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "  height: 0;"
        "}"
        );

    // scrollContent: contiene TimeGrid + overlay blocchi attività
    scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: white;");

    timeGrid = new TimeGrid(scrollContent);
    timeGrid->move(0, 0);

    mainLayout->addWidget(scrollArea, 1);
    scrollArea->setWidget(scrollContent);
}


void DayWidget::buildAllDayArea()
{
    allDayContainer = new QWidget(this);
    allDayLayout = new QVBoxLayout(allDayContainer);

    // Margini compatti: allineati con la griglia oraria
    // Lasciamo a sinistra uno spazio vuoto pari a TimeGrid::LABEL_WIDTH per allineare i blocchi
    allDayLayout->setContentsMargins(TimeGrid::LABEL_WIDTH + 4, 4, 4, 4);
    allDayLayout->setSpacing(2);

    allDayContainer->setStyleSheet(
        "QWidget {"
        "  background-color: #f2f2f7;" // Stesso grigio chiaro dell'header
        "  border-bottom: 1px solid #d1d1d6;"
        "}"
        );

    // Di base lo nascondiamo, lo mostreremo solo se ci sono attività all-day
    allDayContainer->setVisible(false);

    mainLayout->addWidget(allDayContainer);
}

static std::pair<int,int> extractMinutes(AbstractActivity* a, const date& d)
{
    if (!a) return {0, 30};
    MinutesExtractorVisitor visitor(d);
    a->accept(visitor);
    return {visitor.startMinutes, visitor.endMinutes};
}

DayWidget::BlockGeometry
DayWidget::computeGeometry(AbstractActivity* a, int colIdx, int nCols) const
{
    auto [startMin, endMin] = extractMinutes(a, currentDate);

    const int gridLeft  = TimeGrid::LABEL_WIDTH + 2;
    const int gridRight = scrollContent->width() - 4;
    const int colW      = (gridRight - gridLeft) / std::max(nCols, 1);

    BlockGeometry g;
    g.top    = TimeGrid::minutesToY(startMin) + 1;
    g.height = std::max(TimeGrid::minutesToY(endMin) - g.top, 18); // minimo 18px
    g.left   = gridLeft + colIdx * colW + 2;
    g.width  = colW - 4;
    return g;
}

bool DayWidget::shouldShow(AbstractActivity* a) const
{
    if (!a) return false;
    TypeCheckVisitor visitor;
    a->accept(visitor);
    return visitor.valid;
}

static bool isAllDayActivity(AbstractActivity* a)
{
    if (!a) return false;
    AllDayCheckVisitor visitor;
    a->accept(visitor);
    return visitor.isAllDay;
}



// ─── Pulizia e ricostruzione blocchi ─────────────────────────────────────────

void DayWidget::clearBlocks()
{
    for (ActivityBlock* b : blocks) {
        b->removeEventFilter(this); // Previene chiamate zombie a eventi distrutti
        b->deleteLater();
    }
    blocks.clear();
}

// Algoritmo di rilevamento sovrapposizioni per assegnare colonne multiple
// agli eventi che si sovrappongono nello stesso intervallo orario.
static QList<QList<AbstractActivity*>>
groupOverlapping(const QList<QPair<AbstractActivity*, std::pair<int,int>>>& items)
{
    // Ogni gruppo = colonne parallele
    QList<QList<AbstractActivity*>> columns;

    for (const auto& [act, range] : items) {
        int placed = -1;

        // Cerca una colonna dove l'ultimo elemento non si sovrappone
        for (int c = 0; c < columns.size(); c++) {
            AbstractActivity* last = columns[c].last();
            // Recupera range del last
            // (riusato dal ciclo esterno tramite ricerca lineare, accettabile per n piccoli)
            bool overlaps = false;
            for (const auto& [a2, r2] : items) {
                if (a2 == last) {
                    overlaps = (range.first < r2.second && r2.first < range.second);
                    break;
                }
            }
            if (!overlaps) { placed = c; break; }
        }

        if (placed == -1) {
            columns.append({act});
        } else {
            columns[placed].append(act);
        }
    }
    return columns;
}

void DayWidget::populateBlocks()
{
    clearBlocks();

    // Pulizia del layout All-Day superiore
    QLayoutItem* item;
    while ((item = allDayLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->removeEventFilter(this);
            item->widget()->deleteLater();
        }
        delete item;
    }

    auto activitiesOnDate = am.getOnDate(currentDate);

    QList<AbstractActivity*> allDayItems;
    QList<QPair<AbstractActivity*, std::pair<int,int>>> timedItems;

    // Smistamento tramite i metodi basati su Visitor
    for (AbstractActivity* a : activitiesOnDate) {
        if (!shouldShow(a)) continue;

        if (isAllDayActivity(a)) {
            allDayItems.append(a);
        } else {
            timedItems.append({a, extractMinutes(a, currentDate)});
        }
    }

    // ─── Rendering All-Day Area (In Alto) ───────────────────────────────────
    if (!allDayItems.isEmpty()) {
        allDayContainer->setVisible(true);
        for (AbstractActivity* act : allDayItems) {
            auto* block = new ActivityBlock(act, allDayContainer);
            block->setFixedHeight(24); // Dimensione standard compatta per liste in alto

            block->installEventFilter(this);
            block->setProperty("activityPtr", QVariant::fromValue(static_cast<void*>(act)));

            allDayLayout->addWidget(block);
            blocks.append(block);
        }
    } else {
        allDayContainer->setVisible(false);
    }

    // ─── Rendering Griglia Oraria Sottostante ───────────────────────────────
    if (timedItems.isEmpty()) {
        timeGrid->lower();
        timeGrid->update();
        return;
    }

    auto columns = groupOverlapping(timedItems);
    int nCols = columns.size();

    QMap<AbstractActivity*, int> colMap;
    for (int c = 0; c < nCols; c++) {
        for (AbstractActivity* a : columns[c]) {
            colMap[a] = c;
        }
    }

    for (const auto& [act, range] : timedItems) {
        int colIdx = colMap.value(act, 0);
        auto geo = computeGeometry(act, colIdx, nCols);

        auto* block = new ActivityBlock(act, scrollContent);
        block->setGeometry(geo.left, geo.top, geo.width, geo.height);

        block->installEventFilter(this);
        block->setProperty("activityPtr", QVariant::fromValue(static_cast<void*>(act)));

        block->raise();
        block->show();

        blocks.append(block);
    }

    // La griglia deve rimanere sotto i blocchi attività
    timeGrid->lower();
    timeGrid->update();
}

// eventFilter per catturare i click sui blocchi
bool DayWidget::eventFilter(QObject* obj, QEvent* event)  // NOLINT
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto* block = qobject_cast<ActivityBlock*>(obj);
        if (block) {
            void* ptr = block->property("activityPtr").value<void*>();
            if (ptr) emit activityClicked(static_cast<AbstractActivity*>(ptr));
        }
    }
    return QWidget::eventFilter(obj, event);
}

// ─── setDate / refresh ───────────────────────────────────────────────────────

void DayWidget::setDate(const date& d)
{
    currentDate = d;

    // Aggiorna header con data in italiano
    static const char* giorni[]  = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    static const char* mesi[]    = {"","January","February","March","April","May",
                                 "June","July","August","September","October",
                                 "November","Dicember"};
    QDate qd(d.getYear(), d.getMonth(), d.getDay());
    QString dayName = giorni[qd.dayOfWeek() % 7];   // 1=Lun ... 0=Dom
    QString dateStr = QString("%1 %2 %3 %4")
                          .arg(dayName)
                          .arg(d.getDay())
                          .arg(mesi[d.getMonth()])
                          .arg(d.getYear());
    headerLabel->setText(dateStr);

    refresh();
}

void DayWidget::refresh()
{
    // Ridimensiona scrollContent alla larghezza del viewport
    int contentW = scrollArea->viewport()->width();
    if (contentW < 200) contentW = 500; // fallback prima del primo resize
    scrollContent->setFixedSize(contentW, TimeGrid::TOTAL_HEIGHT);
    timeGrid->setFixedSize(contentW, TimeGrid::TOTAL_HEIGHT);

    populateBlocks();
}

void DayWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // Riadatta i blocchi quando la finestra cambia dimensione
    int contentW = scrollArea->viewport()->width();
    if (contentW > 0) {
        scrollContent->setFixedWidth(contentW);
        timeGrid->setFixedWidth(contentW);
        populateBlocks();
    }
}