#include "Headers/UI/week.h"
#include "qdatetime.h"

WeekWidget::WeekWidget(QWidget* parent)
    : QWidget(parent), current(date::today()) {
    setup();
}

void WeekWidget::setup() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Header giorni
    QHBoxLayout* headerLayout = new QHBoxLayout();
    static const char* days[] = {"Lun","Mar","Mer","Gio","Ven","Sab","Dom"};
    for (int i = 0; i < 7; i++) {
        QLabel* l = new QLabel(days[i]);
        l->setAlignment(Qt::AlignCenter);
        headerLayout->addWidget(l);
    }
    layout->addLayout(headerLayout);

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);

    // 7 colonne vuote
    QHBoxLayout* daysLayout = new QHBoxLayout();
    for (int i = 0; i < 7; i++) {
        QWidget* dayCol = new QWidget();
        QVBoxLayout* colLayout = new QVBoxLayout(dayCol);

        QLabel* dateLabel = new QLabel();
        dateLabel->setAlignment(Qt::AlignCenter);
        dateLabel->setObjectName(QString("weekDate_%1").arg(i));
        colLayout->addWidget(dateLabel);

        QFrame* colLine = new QFrame();
        colLine->setFrameShape(QFrame::HLine);
        colLayout->addWidget(colLine);

        // spazio vuoto per le attività
        QWidget* activitiesArea = new QWidget();
        activitiesArea->setObjectName(QString("weekActivities_%1").arg(i));
        new QVBoxLayout(activitiesArea);
        colLayout->addWidget(activitiesArea);
        colLayout->addStretch();

        daysLayout->addWidget(dayCol);
    }
    layout->addLayout(daysLayout);
}

void WeekWidget::setWeek(const date& d) {
    current = d;
    QDate qd(d.getYear(), d.getMonth(), d.getDay());
    QDate monday = qd.addDays(-(qd.dayOfWeek() - 1));

    for (int i = 0; i < 7; i++) {
        QDate current = monday.addDays(i);
        QLabel* dateLabel = findChild<QLabel*>(QString("weekDate_%1").arg(i));
        if (dateLabel)
            dateLabel->setText(current.toString("dd/MM"));
    }
}