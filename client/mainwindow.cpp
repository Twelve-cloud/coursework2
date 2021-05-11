#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMovie>
#include <QString>

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
    connect(&brokerMainWindow, &BrokerMainWindow::companyDoubleClicked, this, [=]()
                                                                                    {
                                                                                        socket.sendToServer("GBC", brokerMainWindow.getCompany());
                                                                                    });
    connect(&brokerMainWindow, &BrokerMainWindow::brokerHandleRequestClicked, this, [=]()
                                                                                    {
                                                                                        socket.sendToServer("BGR", "");
                                                                                    });
    connect(&brokerMainWindow, &BrokerMainWindow::serviceChangeButtonLastClicked, this, [=] ()
                                                                                    {
                                                                                        socket.sendToServer("CHS", brokerMainWindow.getService());
                                                                                    });
    connect(&brokerMainWindow, &BrokerMainWindow::serviceAddButtonLastClicked, this, [=]()
                                                                                      {
                                                                                         socket.sendToServer("ADS", brokerMainWindow.getServiceName() + "~~~" + brokerMainWindow.getServicePrice() + "~~~" + brokerMainWindow.getCompany() + "~~~");
                                                                                      });
    connect(&brokerMainWindow, &BrokerMainWindow::serviceDeleteButtonClicked, this, [=]()
                                                                                    {
                                                                                       socket.sendToServer("DES", brokerMainWindow.getServiceName() + "~~~" + brokerMainWindow.getServicePrice() + "~~~" + brokerMainWindow.getCompany() + "~~~");
                                                                                    });
    connect(&brokerMainWindow, &BrokerMainWindow::cancelRequestButtonClicked, this, [=]()
                                                                                    {
                                                                                       socket.sendToServer("CRE", brokerMainWindow.getOrderLogin() + "~~~" + brokerMainWindow.getServiceName() + "~~~");
                                                                                    });

    connect(&socket, &ClientEntity::readyRead, this, &MainWindow::slotReadyRead);
    connect(ui -> orderServiceButton, &QPushButton::clicked, this, &MainWindow::slotOrderServiceClicked);
    connect(ui -> cancelServiceButton, &QPushButton::clicked, this, &MainWindow::slotCancelServiceClicked);
    connect(ui->serviceWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotServiceDoubleClick(QListWidgetItem*)));

    ui -> loading -> close();
    QMovie* movie = new QMovie("loading.gif");
    ui -> loading -> setMovie(movie);
    movie -> start();
    ui -> closeLoading -> close();
    ui -> accountListWidget -> close();
    ui -> serviceWidget -> close();
    ui -> orderServiceView -> close();
    ui -> serviceNameLabel -> close();
    ui -> serviceNameLabel -> setStyleSheet("color: black");
    ui -> servicePriceLabel -> close();
    ui -> servicePriceLabel -> setStyleSheet("color: black");
    ui -> orderServiceButton -> close();
    ui -> cancelServiceButton -> close();
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
                ui -> orderButton -> setEnabled(true);
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
    else if (command == "GBC" && role == "BROKER")
    {
        char service[32], price[32];
        std::string str = socket.getData().toStdString();

        while (str != "")
        {
            getFields(str, 3, service, price);
            brokerMainWindow.addServiceLine(QString(service) + ", Цена: " + price);
        }
    }
    else if (command == "CRS" && role == "USER")
    {
        char service[32];
        std::string str = socket.getData().toStdString();

        while (str != "")
        {
            getFields(str, 2, service);
            ui -> serviceWidget -> addItem(QString(service));
        }
    }
    else if (command == "GSP" && role == "USER")
    {
        char service[32], price[32];
        std::string str = socket.getData().toStdString();
        getFields(str, 3, service, price);
        userService = service;
        userServicePrice = price;
        ui -> serviceNameLabel -> setText("Услуга: " + QString(service));
        ui -> servicePriceLabel -> setText("Средняя цена на рынке: " + QString(price));
    }
    else if (command == "HCS" && role == "BROKER")
    {
        brokerMainWindow.clearRequests();
        socket.sendToServer("BGR", "");
    }
    else if (command == "BGR" && role == "BROKER")
    {
        char account[32], service[32];
        std::string str = socket.getData().toStdString();

        while (str != "")
        {
            getFields(str, 3, account, service);
            brokerMainWindow.addRequestLine(QString(account) + "---" + QString(service));
        }
    }
    else if (command == "SCS" && role == "BROKER")
    {
        brokerMainWindow.clearServices();
        socket.sendToServer("GBC", brokerMainWindow.getCompany());
    }
    else if (command == "FCS" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Услуга уже существует", QMessageBox::Ok);
    }
    else if (command == "SAS" && role == "BROKER")
    {
        brokerMainWindow.clearServices();
        socket.sendToServer("GBC", brokerMainWindow.getCompany());
    }
    else if (command == "FAS" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Услуга уже существует", QMessageBox::Ok);
    }
    else if (command == "SDS" && role == "BROKER")
    {
        brokerMainWindow.clearServices();
        socket.sendToServer("GBC", brokerMainWindow.getCompany());
    }
    else if (command == "FDS" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Ошибка", QMessageBox::Ok);
    }
    else if (command == "SCR" && role == "BROKER")
    {
        brokerMainWindow.clearRequests();
        socket.sendToServer("BGR", "");
    }
    else if (command == "FCR" && role == "BROKER")
    {
        QMessageBox::information(nullptr, "Информация", "Ошибка", QMessageBox::Ok);
    }
    else if (command == "FAR" && role == "USER")
    {
        QMessageBox::information(nullptr, "Информация", "Ваша заявка в обработке", QMessageBox::Ok);
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
    ui -> serviceWidget -> show();
    ui -> serviceWidget -> clear();
    ui -> orderButton -> setEnabled(false);
    ui -> consultationButton -> setEnabled(true);
    socket.sendToServer("CRS", "");
}

void MainWindow::slotConsultationClicked()
{
    ui -> serviceWidget -> close();
    ui -> consultationButton -> setEnabled(false);
    ui -> orderButton -> setEnabled(false);
    socket.sendToServer("CNS", authWindow.getLogin());
}

void MainWindow::slotCloseConsultationClicked()
{
    ui -> consultationButton -> setEnabled(true);
    ui -> orderButton -> setEnabled(true);
    socket.sendToServer("CCN", authWindow.getLogin());
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
        ui -> orderButton -> setEnabled(true);
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

void MainWindow::slotServiceDoubleClick(QListWidgetItem* item)
{
    ui -> orderServiceView -> show();
    ui -> serviceNameLabel -> show();
    ui -> servicePriceLabel -> show();
    ui -> orderServiceButton -> show();
    ui -> cancelServiceButton -> show();
    socket.sendToServer("GSP", item -> text());
}

void MainWindow::slotOrderServiceClicked()
{
    socket.sendToServer("COS", authWindow.getLogin() + "~~~" + userService + "~~~");
    ui -> orderServiceView -> close();
    ui -> serviceNameLabel -> close();
    ui -> servicePriceLabel -> close();
    ui -> orderServiceButton -> close();
    ui -> cancelServiceButton -> close();
}

void MainWindow::slotCancelServiceClicked()
{
    ui -> orderServiceView -> close();
    ui -> serviceNameLabel -> close();
    ui -> servicePriceLabel -> close();
    ui -> orderServiceButton -> close();
    ui -> cancelServiceButton -> close();
}
