#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "authentification.h"
#include "registration.h"
#include "client_entity.h"
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

public slots:
    void slotRegistrationClicked();
    void slotSignInClicked();

private:
    Ui::MainWindow *ui;
    AuthentificationWindow authWindow;
    RegistrationWindow regiWindow;
    ClientEntity socket;
};
#endif // MAINWINDOW_H
