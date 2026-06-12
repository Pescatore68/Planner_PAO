#ifndef WEEKWIDGET_H
#define WEEKWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include "Headers/date.h"

class WeekWidget : public QWidget {
    Q_OBJECT
private:
    date current;
    void setup();

public:
    explicit WeekWidget(QWidget* parent = nullptr);
    void setWeek(const date& d);

signals:
    void dateClicked(const date& d);
};
#endif