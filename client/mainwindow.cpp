#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMovie>

MainWindow::MainWindow(const QString& strHost, const qint32& nPort, QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow), socket(strHost, nPort)
{
    ui -> setupUi(this);

    connect(&authWindow, &AuthentificationWindow::registrationButtonClicked, [=]()
        {
            authWindow.hide();
            regiWindow.show();
        });
    connect(&regiWindow, &RegistrationWindow::backButtonClicked, [=]()
        {
            regiWindow.hide();
            authWindow.show();
        });

    connect(&regiWindow, &RegistrationWindow::registrationButtonClicked, this, &MainWindow::slotRegistrationClicked);
    connect(&authWindow, &AuthentificationWindow::signInButtonClicked, this, &MainWindow::slotSignInClicked);

    connect(ui -> accountSettingsButton, &QPushButton::clicked, this, [=]()
        {
            if (ui -> accountListWidget -> isVisible())
            {
                ui -> accountListWidget -> close();
            }
            else
            {
                ui -> accountListWidget -> show();
            }
        });
    connect(ui -> consultationButton, &QPushButton::clicked, this, &MainWindow::slotConsultationClicked);
    connect(ui -> closeLoading,  &QPushButton::clicked, this, &MainWindow::slotCloseConsultationClicked);
    connect(ui -> orderButton, &QPushButton::clicked, this, &MainWindow::slotOrderClicked);

    connect(&consultantMainWindow, &ConsultantMainWindow::acceptRequestClicked, this, &MainWindow::slotAcceptRequestClicked);
    connect(&consultantMainWindow, &ConsultantMainWindow::cancelRequestClicked, this, &MainWindow::slotCancelRequestClicked);


    connect(&socket, &ClientEntity::readyRead, this, &MainWindow::slotReadyRead);

    ui -> loading -> close();
    QMovie* movie = new QMovie("loading.gif");
    ui -> loading -> setMovie(movie);
    movie -> start();
    ui -> closeLoading -> close();
    ui -> accountListWidget -> close();
    authWindow.show();
}

MainWindow::~MainWindow()
{
    socket.sendToServer("EXT", "0");
    delete ui;
}

void MainWindow::handleResult(const QString& command)
{
    if (command == "SRG")
    {
        regiWindow.close();
        regiWindow.clearLines();
        QMessageBox::information(nullptr, "Информация", "Регистрация успешно завершена", QMessageBox::Ok);
        authWindow.show();
    }
    else if (command == "FRG")
    {
        regiWindow.setError("Такой аккаунт уже существует");
    }
    else if (command == "SAU")
    {
        role = socket.getData();
        if (role == "USER")
        {
            authWindow.close();
            this -> show();
        }
        else if (role == "CONSULTANT")
        {
            authWindow.close();
            consultantMainWindow.show();
        }
        else if (role == "BROKER")
        {

        }
    }
    else if (command == "FAU")
    {
        authWindow.setError("Неверный логин или пароль");
    }
    else if (command == "RCN" && role == "CONSULTANT")
    {
        consultantMainWindow.addRequestLine(socket.getData());
    }
    else if (command == "RCL" && role == "CONSULTANT")
    {
        consultantMainWindow.deleteRequestLine(socket.getData());
    }
    else if (command == "RDL" && socket.getData() == authWindow.getLogin())
    {
        ui -> loading -> close();
        ui -> closeLoading -> close();
        ui -> consultationButton -> setEnabled(true);
        QMessageBox::information(nullptr, "Информация", "Вы были удалены из очереди сотрудником", QMessageBox::Ok);
    }
}

void MainWindow::slotRegistrationClicked()
{
    socket.sendToServer("REG", regiWindow.getLogin() + "~~~" + regiWindow.getPassword() + "~~~");
}

void MainWindow::slotSignInClicked()
{
    if (!authWindow.isEmptyLines())
    {
        socket.sendToServer("ATH", authWindow.getLogin() + "~~~" + authWindow.getPassword() + "~~~");
    }
    else
    {
        authWindow.setError("Заполните все поля");
    }
}

void MainWindow::slotOrderClicked()
{
    QMessageBox::information(nullptr, "Информация", "Ваша заявка принята. \nС вами свяжется консультант в ближайшее время", QMessageBox::Ok);
}

void MainWindow::slotConsultationClicked()
{
    socket.sendToServer("CNS", authWindow.getLogin());
    ui -> consultationButton -> setEnabled(false);
}

void MainWindow::slotCloseConsultationClicked()
{
    socket.sendToServer("CCN", authWindow.getLogin());
    ui -> consultationButton -> setEnabled(true);
}

void MainWindow::slotCancelRequestClicked()
{
    socket.sendToServer("RDL", consultantMainWindow.getLoginCurrentItem());
    consultantMainWindow.deleteRequestLine(consultantMainWindow.getLoginCurrentItem());
}

void MainWindow::slotAcceptRequestClicked()
{

}

void MainWindow::slotReadyRead()
{
    QString command = socket.getCommand();
    handleResult(command);
}
