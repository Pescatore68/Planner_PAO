#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "Headers/mainwindow.h"
#include "Headers/Model/ActivityManager.h"
#include "Headers/Model/tagManager.h"
#include "Headers/Model/Persistence/JSONformat.h"
#include "Headers/Model/Persistence/xmlFormat.h"
#include <QApplication>
#include "Headers/UI/StyleHelper.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyleSheet(StyleHelper::getApplicationStyle());

    ActivityManager am;
    tagManager      tm;

    MainWindow w(am, tm);
    w.show();

    int exitCode = app.exec();

    return exitCode;
}