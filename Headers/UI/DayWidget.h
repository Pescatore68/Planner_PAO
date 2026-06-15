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

class ActivityBlock : public QWidget {
    Q_OBJECT
private:
    AbstractActivity* activity;
    bool hovered = false;
    bool selected = false;

protected:
    void paintEvent(QPaintEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

public:
    explicit ActivityBlock(AbstractActivity* a, QWidget* parent = nullptr);

    void setSelectedState(bool isSelected);
    bool isSelectedState() const;
};

//Timegrid
class TimeGrid : public QWidget {
    Q_OBJECT
public:
    static constexpr int HOUR_HEIGHT   = 120;
    static constexpr int LABEL_WIDTH   = 52;
    static constexpr int TOTAL_HOURS   = 24;
    static constexpr int TOTAL_HEIGHT  = HOUR_HEIGHT * TOTAL_HOURS;

    explicit TimeGrid(QWidget* parent = nullptr);
    static int minutesToY(int totalMinutes);

protected:
    void paintEvent(QPaintEvent* event) override;
};

//Daywidget
class DayWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayWidget(ActivityManager& am, QWidget* parent = nullptr);


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

    QVBoxLayout*  mainLayout   = nullptr;
    QLabel*       headerLabel  = nullptr;
    QScrollArea*  scrollArea   = nullptr;


    QWidget*      scrollContent = nullptr;
    TimeGrid*     timeGrid      = nullptr;

    QWidget* allDayContainer = nullptr;
    QVBoxLayout* allDayLayout    = nullptr;


    QList<ActivityBlock*> blocks;

    void buildHeader();
    void buildScrollArea();
    void buildAllDayArea();
    void populateBlocks();
    void clearBlocks();

    //true if activity is all day
    bool shouldShow(AbstractActivity* a) const;

    //calculate text heght in px
    struct BlockGeometry { int top; int height; int left; int width; };
    BlockGeometry computeGeometry(AbstractActivity* a, int colIdx, int nCols) const;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
};

#endif // DAYWIDGET_H