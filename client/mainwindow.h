#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "authentification.h"
#include "registration.h"
#include <QTcpSocket>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString& strHost, const qint32& nPort, QWidget *parent = nullptr);
    ~MainWindow();

    void handleResult(const qint32& result, const QString& string);
    void sendToServer(const QString& command, const QString& string);

public slots:
    void slotRegistrationClicked();
    void slotSignInClicked();
    void slotReadyRead();

private:
    Ui::MainWindow *ui;
    AuthentificationWindow authWindow;
    RegistrationWindow regiWindow;
    QTcpSocket* tcpSocket;
    qint32 nextBlockSize;
};
#endif // MAINWINDOW_H
