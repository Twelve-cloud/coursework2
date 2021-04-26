#include "mainwindow.h"
#include "registration.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString filename = "C:\\Users\\twelv\\Dropbox\\coursework3\\client\\style.css";
    if (!QFile::exists(filename))
    {
        qFatal("Can't loading styles.");
    }
    QFile style(filename);
    style.open(QIODevice::ReadOnly);
    app.setStyleSheet(style.readAll());


    MainWindow w;
    return app.exec();
}
