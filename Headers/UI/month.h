#ifndef MONTHWIDGET_H
#define MONTHWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QVBoxLayout>

class MonthWidget : public QWidget {
    Q_OBJECT

private:
    QCalendarWidget* calendar;

    void setup();

public:
    explicit MonthWidget(QWidget* parent = nullptr);

signals:
    void dateClicked(const QDate& d);
};

#endif