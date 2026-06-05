#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "Headers/task.h"
#include "Headers/date.h"
#include "Headers/HourMinute.h"
#include "Headers/routine.h"
#include "Headers/tagManager.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tagManager tm;
    tag* uni = tm.newTag("uni", QColor(128, 128, 128));

    // --- test task ---
    task t("Comprare latte", "Al supermercato",
           date(31, 12, 2025), HourMinute(9, 0));

    //test routine

    Routine r("palestra", "", uni, HourMinute(16, 0), HourMinute(18, 0), date(31, 12, 2025), date(31, 12, 2026), Routine::Frequency::Monthly);

    QLabel* label = new QLabel(this);
    label->setText(QString::fromStdString(r.summary()));
    label->setGeometry(120, 120, 500, 120);
    label->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    label->setWordWrap(true);
    label->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}