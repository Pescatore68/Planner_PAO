#ifndef DAYWIDGET_H
#define DAYWIDGET_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QCheckBox>

class AbstractActivity; // forward declaration
class task;             // forward declaration
class project;          // forward declaration


class DayWidget : public QWidget {
    Q_OBJECT

public:
    explicit DayWidget(QWidget* parent = nullptr);
    void setActivity(AbstractActivity* a);


#endif // DAYWIDGET_H
