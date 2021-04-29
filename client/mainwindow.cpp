#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QString& strHost, const qint32& nPort, QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow), nextBlockSize(0)
{
    ui -> setupUi(this);
    tcpSocket = new QTcpSocket(this);
    tcpSocket -> connectToHost(strHost, nPort);

    connect(&authWindow, &AuthentificationWindow::registrationButtonClicked, [=](){ authWindow.hide(); regiWindow.show(); });
    connect(&regiWindow, &RegistrationWindow::backButtonClicked, [=]() { regiWindow.hide(); authWindow.show(); });
    connect(&regiWindow, &RegistrationWindow::registrationButtonClicked, this, &MainWindow::slotRegistrationClicked);
    connect(&authWindow, &AuthentificationWindow::signInButtonClicked, this, &MainWindow::slotSignInClicked);
    connect(ui -> accountSettingsButton, &QPushButton::clicked, this, [=]()
        {
            if (ui -> accountListWidget -> isVisible())
                ui -> accountListWidget -> close();
            else
                ui -> accountListWidget -> show();
        });

    ui -> accountListWidget -> close();
    authWindow.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotRegistrationClicked()
{
    //sendToServer("REG", (authWindow.getLogin() + "|" + authWindow.getPassword()).toStdString().c_str());
}

void MainWindow::slotSignInClicked()
{
    authWindow.close();
    this -> show();
//    if (!authWindow.isEmptyLine())
//    {
//        sendToServer(SendingCodes::AUTHENTIFICATION, authWindow.getLogin() + "|" + authWindow.getPassword());
//    }
//    else
//    {
//        authWindow.setError("Заполните все поля");
//    }
}

void MainWindow::handleResult(const qint32& result, const QString& string)
{

}

void MainWindow::sendToServer(const QString& command, const QString& string)
{
    tcpSocket -> write(command.toStdString().c_str());
    size_t commandSize = sizeof(string);
    tcpSocket -> write((char*)commandSize);
    tcpSocket -> write(string.toStdString().c_str());
    tcpSocket -> waitForBytesWritten();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(tcpSocket);
    qint16 result;
    QString string;

    while (true)
    {

        if (!nextBlockSize)
        {
            if (tcpSocket -> bytesAvailable() < qint64(sizeof(quint32)))
            {
                break;
            }
            in >> nextBlockSize;
        }

        if (tcpSocket -> bytesAvailable() < nextBlockSize)
        {
            break;
        }
        in >> result >> string;
        handleResult(result, string);
        nextBlockSize = 0;
    }
}
