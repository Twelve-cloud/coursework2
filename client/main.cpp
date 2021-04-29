#include "mainwindow.h"
#include "registration.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QString filename = (app.applicationDirPath() + "\\style.css");
    if (!QFile::exists(filename))
    {
        qFatal("Can't loading styles.");
    }
    QFile style(filename);
    style.open(QIODevice::ReadOnly);
    app.setStyleSheet(style.readAll());


    MainWindow w("127.0.0.1", 3360);
    return app.exec();
}
