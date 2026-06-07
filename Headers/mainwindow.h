#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"
#include "Headers/AbstractActivity.h"
#include <QMainWindow>
#include <QStackedWidget>

class DetailWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

    QStackedWidget* stack;
    DetailWidget*   detailWidget;
    // aggiungeremo listWidget ecc.

public:
    explicit MainWindow(QWidget* parent = nullptr);

    void showDetail( AbstractActivity* a);
    void showList();
};

#endif