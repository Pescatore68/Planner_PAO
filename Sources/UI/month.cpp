#include "Headers/UI/month.h"
#include <QVBoxLayout>

MonthWidget::MonthWidget(QWidget* parent)
    : QWidget(parent)
{
    setup();
}

void MonthWidget::setup()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    calendar = new QCalendarWidget(this);

    layout->addWidget(calendar);

    connect(calendar, &QCalendarWidget::clicked,
            this, &MonthWidget::dateClicked);
}