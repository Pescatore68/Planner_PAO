#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QTimer>
#include <QMap>

#include "Headers/ActivityManager.h"
#include "Headers/date.h"

class AbstractActivity;
class task;
class project;

// ─── Blocco grafico per una singola attività ──────────────────────────────────
// Rettangolo colorato con nome centrato, posizionato in overlay sul time grid.
class ActivityBlock : public QWidget {
    Q_OBJECT
public:
    explicit ActivityBlock(AbstractActivity* a, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    AbstractActivity* activity;
    bool hovered = false;
};

//Timegrid
class TimeGrid : public QWidget {
    Q_OBJECT
public:
    static constexpr int HOUR_HEIGHT   = 60;  // px per ora
    static constexpr int LABEL_WIDTH   = 52;  // larghezza colonna ore
    static constexpr int TOTAL_HOURS   = 24;
    static constexpr int TOTAL_HEIGHT  = HOUR_HEIGHT * TOTAL_HOURS;

    explicit TimeGrid(QWidget* parent = nullptr);

    // Converte minuti-da-mezzanotte → coordinata Y in pixel
    static int minutesToY(int totalMinutes);

protected:
    void paintEvent(QPaintEvent* event) override;
};

//Daywidget
class DayWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayWidget(ActivityManager& am, QWidget* parent = nullptr);

    // Imposta il giorno da visualizzare e ricostruisce i blocchi
    void setDate(const date& d);
    void refresh();

signals:
    void activityClicked(AbstractActivity* a);
    void activityDoubleClicked(AbstractActivity* a);

private:
    bool doubleClickPending = false;

private:
    ActivityManager& am;
    date             currentDate;

    // ── layout ───────────────────────────────────────────────────────────────
    QVBoxLayout*  mainLayout   = nullptr;
    QLabel*       headerLabel  = nullptr;   // "Lunedì 12 Giugno 2026"
    QScrollArea*  scrollArea   = nullptr;

    // Contenitore interno allo scroll: TimeGrid + ActivityBlock in overlay
    QWidget*      scrollContent = nullptr;
    TimeGrid*     timeGrid      = nullptr;
    // Sotto gli altri puntatori ai widget (es. sotto QLabel* headerLabel)
    QWidget* allDayContainer = nullptr;
    QVBoxLayout* allDayLayout    = nullptr; // Layout in cui inseriremo i blocchi All-Day


    // Blocchi attività (ricreati a ogni setDate)
    QList<ActivityBlock*> blocks;

    void buildHeader();
    void buildScrollArea();
    void buildAllDayArea();
    void populateBlocks();
    void clearBlocks();

    // Restituisce true se l'attività va mostrata nella day view
    // (esclude task e project)
    bool shouldShow(AbstractActivity* a) const;

    // Calcola top/height in pixel dai dati dell'attività
    // (usa DisplayVisitor per leggere orari)
    struct BlockGeometry { int top; int height; int left; int width; };
    BlockGeometry computeGeometry(AbstractActivity* a, int colIdx, int nCols) const;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif // DAYWIDGET_H