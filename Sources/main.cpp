#include <QApplication>
#include "Headers/mainwindow.h"
#include "Headers/ActivityManager.h"
#include "Headers/tagManager.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ActivityManager am;
    tagManager tm;

    MainWindow w(am, tm);
    w.show();

    return app.exec();
}