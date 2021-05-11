#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMovie>

void getFields(std::string& str, const std::size_t& argc, ...);

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

    connect(&chatWindow, &ChatWindow::endChatClicked, this, &MainWindow::slotEndChatClicked);
    connect(&chatWindow, &ChatWindow::sendMessageClicked, this, &MainWindow::slotSendMessageClicked);


    connect(&brokerMainWindow, &BrokerMainWindow::companyButtonClicked, this, &MainWindow::slotCompanyButtonClicked);
    connect(&brokerMainWindow, &BrokerMainWindow::companyAddButtonLastClicked, this, [=]()
                                                                                     {
                                                                                         socket.sendToServer("ADC", brokerMainWindow.getCompany());
                                                                                     });
    connect(&brokerMainWindow, &BrokerMainWindow::companyDeleteButtonClicked, this, [=]()
                                                                                    {
                                                                                        socket.sendToServer("DLC", brokerMainWindow.getCompany());
                                                                                    });
    connect(&brokerMainWindow, &BrokerMainWindow::companyChangeButtonLastClicked, this, [=]()
                                                                                    {
                                                                                        socket.sendToServer("CHC", brokerMainWindow.getCompany());
                                                                                    });

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
    socket.sendToServer("EXT", "");
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
            authWindow.close();
            brokerMainWindow.show();
        }
    }
    else if (command == "FAU")
    {
        authWindow.setError("Неверный логин или пароль");
    }
    else if (command == "BAN")
    {
        authWindow.setError("Вы забанены до " + socket.getData());
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
    else if (command == "CRC" && (role == "USER" || role == "CONSULTANT"))
    {
        char login[32], companion[32];
        std::string str = socket.getData().toStdString();
        getFields(str, 3, login, companion);

        if (authWindow.getLogin() == login)
        {
            chatWindow.setCompanion(companion);
            chatWindow.show();
            this -> close();
        }
    }
    else if (command == "CLC" && (role == "USER" || role == "CONSULTANT"))
    {
        char login[32], companion[32];
        std::string str = socket.getData().toStdString();
        getFields(str, 3, login, companion);

        if (authWindow.getLogin() == login && chatWindow.getCompanion() == companion)
        {
            chatWindow.close();
            chatWindow.clearChat();
            if (role == "USER")
            {
                this -> show();
                ui -> loading -> close();
                ui -> closeLoading -> close();
                ui -> consultationButton -> setEnabled(true);
            }
            else if (role == "CONSULTANT")
            {
                consultantMainWindow.show();
                consultantMainWindow.deleteRequestLine(consultantMainWindow.getLoginCurrentItem());
                consultantMainWindow.setDefaultLabel();
            }
        }
    }
    else if (command == "MSD" && (role == "USER" || role == "CONSULTANT"))
    {
        char login[32], message[4096];
        std::string str = socket.getData().toStdString();
        getFields(str, 3, login, message);

        if (authWindow.getLogin() == login)
        {
            chatWindow.renderMessage("RECIEVER", message);
        }
    }
    else if (command == "GTC" && role == "BROKER")
    {
        char company[32];
        std::string str = socket.getData().toStdString();

        while (str != "")
        {
            getFields(str, 2, company);
            brokerMainWindow.addCompanyLine(company);
        }
    }
    else if (command == "SAC" && role == "BROKER")
    {
        brokerMainWindow.clearCompanies();
        socket.sendToServer("GTC", "");
    }
    else if (command == "FAC" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Компания уже существует", QMessageBox::Ok);
    }
    else if (command == "SDC" && role == "BROKER")
    {
        brokerMainWindow.clearCompanies();
        socket.sendToServer("GTC", "");
    }
    else if (command == "FDC" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Ошибка, эта компания уже удалена другим брокером", QMessageBox::Ok);
    }
    else if (command == "SCC" && role == "BROKER")
    {
        brokerMainWindow.clearCompanies();
        socket.sendToServer("GTC", "");
    }
    else if (command == "FCC" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Компания уже существует", QMessageBox::Ok);
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
    chatWindow.setCompanion(consultantMainWindow.getLoginCurrentItem());
    socket.sendToServer("RAP", chatWindow.getCompanion() + "~~~" + authWindow.getLogin() + "~~~");
    chatWindow.show();
    consultantMainWindow.close();
}

void MainWindow::slotEndChatClicked()
{
    socket.sendToServer("CLC", chatWindow.getCompanion() + "~~~" + authWindow.getLogin() + "~~~");
    chatWindow.close();
    if (role == "USER")
    {
        this -> show();
        ui -> loading -> close();
        ui -> closeLoading -> close();
        ui -> consultationButton -> setEnabled(true);
    }
    else if (role == "CONSULTANT")
    {
        consultantMainWindow.setDefaultLabel();
        consultantMainWindow.show();
        consultantMainWindow.deleteRequestLine(consultantMainWindow.getLoginCurrentItem());
    }
}

void MainWindow::slotSendMessageClicked()
{
    chatWindow.renderMessage("SENDER", chatWindow.getMessage());
    socket.sendToServer("MSD", chatWindow.getCompanion() + "~~~" + chatWindow.getMessage() + "~~~");
}

void MainWindow::slotReadyRead()
{
    QString command = socket.getCommand();
    handleResult(command);
}

void MainWindow::slotCompanyButtonClicked()
{
    socket.sendToServer("GTC", "");
}
