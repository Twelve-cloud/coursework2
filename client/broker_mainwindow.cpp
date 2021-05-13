#include "broker_mainwindow.h"
#include "ui_broker_mainwindow.h"
#include <QMessageBox>

BrokerMainWindow::BrokerMainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::broker_mainwindow)
{
    ui->setupUi(this);

    ui->addCompanyButton->close();
    ui->deleteCompanyButton->close();
    ui->changeCompanyButton->close();
    ui->companyWidget->close();
    ui->handleRequestButton->close();
    ui->cancelRequestButton->close();
    ui->handleWidget->close();
    ui->addServiceButton->close();
    ui->deleteServiceButton->close();
    ui->changeServiceButton->close();
    ui->toolButton -> close();
    ui->serviceWidget->close();
    ui->addCompanyButtonLast->close();
    ui->cancelAddCompanyButton->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    ui->changeCompanyButtonLast->close();
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->close();
    ui->serviceNameLabel->close();
    ui->serviceNameLineEdit->close();
    ui->priceServiceLabel->close();
    ui->priceServiceLineEdit->close();
    ui->changeServiceView->close();
    ui->cancelAddServiceButton->close();

    connect(ui -> companyButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyButtonClicked);
    connect(ui -> requestsButton, &QPushButton::clicked, this, &BrokerMainWindow::slotRequestsButtonClicked);
    connect(ui -> addCompanyButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyAddButtonClicked);
    connect(ui -> addCompanyButtonLast, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyAddButtonLastClicked);
    connect(ui -> cancelAddCompanyButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyCancelAddClicked);
    connect(ui -> deleteCompanyButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyDeleteButtonClicked);
    connect(ui -> changeCompanyButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyChangeButtonClicked);
    connect(ui -> changeCompanyButtonLast, &QPushButton::clicked, this, &BrokerMainWindow::slotCompanyChangeButtonLastClicked);
    connect(ui->companyWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotCompanyDoubleClick(QListWidgetItem*)));
    connect(ui->toolButton, &QToolButton::clicked, this, &BrokerMainWindow::slotToolButtonClicked);
    connect(ui->addServiceButton, &QPushButton::clicked, this, &BrokerMainWindow::slotAddServiceButtonClicked);
    connect(ui->changeServiceButton, &QPushButton::clicked, this, &BrokerMainWindow::slotChangeServiceButtonClicked);
    connect(ui->addServiceButtonLast, &QPushButton::clicked, this, &BrokerMainWindow::slotAddServiceButtonLastClicked);
    connect(ui ->deleteServiceButton, &QPushButton::clicked, this, &BrokerMainWindow::slotDeleteServiceButtonClicked);
    connect(ui-> changeServiceButtonLast, &QPushButton::clicked, this, &BrokerMainWindow::slotChangeServiceButtonLastClicked);
    connect(ui->cancelAddServiceButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCancelAddServiceButtonClicked);
    connect(ui->cancelRequestButton, &QPushButton::clicked, this, &BrokerMainWindow::slotCancelRequestButtonClicked);
    connect(ui->handleRequestButton, &QPushButton::clicked, this, &BrokerMainWindow::slotHandleRequestButtonClicked);
}

BrokerMainWindow::~BrokerMainWindow()
{
    delete ui;
}

void BrokerMainWindow::addCompanyLine(const QString &company)
{
    ui -> companyWidget -> addItem(company);
}

void BrokerMainWindow::addServiceLine(const QString &service)
{
    ui -> serviceWidget -> addItem(service);
}

void BrokerMainWindow::addRequestLine(const QString &request)
{
    ui -> handleWidget -> addItem(request);
}

void BrokerMainWindow::clearCompanies()
{
    ui -> companyWidget -> clear();
}

void BrokerMainWindow::clearRequests()
{
    ui -> handleWidget -> clear();
}

void BrokerMainWindow::clearServices()
{
    ui -> serviceWidget -> clear();
}

void BrokerMainWindow::slotCompanyButtonClicked()
{
    ui->handleRequestButton->close();
    ui->cancelRequestButton->close();
    ui->handleWidget->close();
    ui->addCompanyButton->show();
    ui->deleteCompanyButton->show();
    ui->changeCompanyButton->show();
    ui->companyWidget->show();
    ui ->companyWidget -> clear();

    ui ->companyButton -> setEnabled(false);
        ui -> requestsButton -> setEnabled(true);

    emit companyButtonClicked();
}

void BrokerMainWindow::slotRequestsButtonClicked()
{
    ui -> handleWidget -> clear();
    ui->addCompanyButton->close();
    ui->deleteCompanyButton->close();
    ui->changeCompanyButton->close();
    ui->companyWidget->close();
    ui->handleRequestButton->show();
    ui->cancelRequestButton->show();
    ui->handleWidget->show();
    ui->addCompanyButtonLast->close();
    ui->changeCompanyButtonLast->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    ui->cancelAddCompanyButton->close();
    ui -> companyButton -> setEnabled(true);
    ui -> requestsButton -> setEnabled(false);

    emit brokerHandleRequestClicked();
}

void BrokerMainWindow::slotCompanyAddButtonClicked()
{
    ui->addCompanyButtonLast->show();
    ui->cancelAddCompanyButton->show();
    ui->companyNameLabel->show();
    ui->companyNameLineEdit->show();
    ui->companyAddView -> show();
    ui -> companyNameLineEdit -> clear();
}

void BrokerMainWindow::slotCompanyAddButtonLastClicked()
{
    ui->addCompanyButtonLast->close();
    ui->cancelAddCompanyButton->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    company = ui -> companyNameLineEdit -> text();

    if (company.size() < 6 || company.size() > 16)
    {
        QMessageBox::information(nullptr, "Информация", "Длина имени компании от 6 до 16 символов", QMessageBox::Ok);
        return;
    }

    for (const auto& ch : company)
    {
        if (!ch.isDigit() && !ch.isLetter() && ch != ' ')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные", QMessageBox::Ok);
            return;
        }
    }

    emit companyAddButtonLastClicked();
}

void BrokerMainWindow::slotCompanyCancelAddClicked()
{
    ui->addCompanyButtonLast->close();
    ui -> changeCompanyButtonLast -> close();
    ui->cancelAddCompanyButton->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    ui -> companyNameLineEdit -> clear();
}

void BrokerMainWindow::slotCompanyDeleteButtonClicked()
{
    bool isSelected = false;
    for (std::size_t i = 0; i < ui -> companyWidget -> count(); i++)
    {
        if (ui -> companyWidget -> item(i) -> isSelected())
        {
            company = ui -> companyWidget -> item(i) -> text();
            isSelected = true;
        }
    }

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите компанию", QMessageBox::Ok);
        return;
    }

    emit companyDeleteButtonClicked();
}

void BrokerMainWindow::slotCompanyChangeButtonClicked()
{
    ui->changeCompanyButtonLast->show();
    ui->cancelAddCompanyButton->show();
    ui->companyNameLabel->show();
    ui->companyNameLineEdit->show();
    ui->companyAddView -> show();
    ui -> companyNameLineEdit -> clear();
}

void BrokerMainWindow::slotCompanyChangeButtonLastClicked()
{
    ui->changeCompanyButtonLast->close();
    ui->cancelAddCompanyButton->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    QString new_company;
    new_company = ui -> companyNameLineEdit -> text();

    if (new_company.size() < 6 || new_company.size() > 16)
    {
        QMessageBox::information(nullptr, "Информация", "Длина имени компании от 6 до 16 символов", QMessageBox::Ok);
        return;
    }

    for (const auto& ch : new_company)
    {
        if (!ch.isDigit() && !ch.isLetter() && ch != ' ')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные", QMessageBox::Ok);
            return;
        }
    }

    bool isSelected = false;
    for (std::size_t i = 0; i < ui -> companyWidget -> count(); i++)
    {
        if (ui -> companyWidget -> item(i) -> isSelected())
        {
            company = ui -> companyWidget -> item(i) -> text();
            isSelected = true;
        }
    }

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите компанию", QMessageBox::Ok);
        return;
    }

    company += "~~~" + new_company + "~~~";

    emit companyChangeButtonLastClicked();
}

void BrokerMainWindow::slotCompanyDoubleClick(QListWidgetItem* item)
{
    company = item -> text();

    ui->addCompanyButton->close();
    ui->deleteCompanyButton->close();
    ui->changeCompanyButton->close();
    ui->companyWidget->close();
    ui->addCompanyButtonLast->close();
    ui->changeCompanyButtonLast->close();
    ui->companyNameLabel->close();
    ui->companyNameLineEdit->close();
    ui->companyAddView -> close();
    ui->cancelAddCompanyButton->close();
    ui->addServiceButton->show();
    ui->deleteServiceButton->show();
    ui->changeServiceButton->show();
    ui->serviceWidget->show();
    ui->toolButton -> show();
    ui->requestsButton->setEnabled(false);
    ui->companyButton->setEnabled(false);

    emit companyDoubleClicked();
}

void BrokerMainWindow::slotToolButtonClicked()
{
    ui->addCompanyButton->show();
    ui->deleteCompanyButton->show();
    ui->changeCompanyButton->show();
    ui->companyWidget->show();
    ui->addServiceButton->close();
    ui->deleteServiceButton->close();
    ui->changeServiceButton->close();
    ui->serviceWidget->close();
    ui->toolButton -> close();
    ui->serviceWidget->clear();
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->close();
    ui->serviceNameLabel->close();
    ui->serviceNameLineEdit->close();
    ui->priceServiceLabel->close();
    ui->priceServiceLineEdit->close();
    ui->changeServiceView->close();
    ui->cancelAddServiceButton->close();
    ui->requestsButton->setEnabled(true);
    ui->companyButton->setEnabled(true);
}

void BrokerMainWindow::slotAddServiceButtonClicked()
{
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->show();
    ui->serviceNameLabel->show();
    ui->serviceNameLineEdit->show();
    ui->priceServiceLabel->show();
    ui->priceServiceLineEdit->show();
    ui->changeServiceView->show();
    ui->cancelAddServiceButton->show();
    ui->serviceNameLineEdit->clear();
    ui->priceServiceLineEdit->clear();
}

void BrokerMainWindow::slotChangeServiceButtonClicked()
{
    ui->addServiceButtonLast->close();
    ui->changeServiceButtonLast->show();
    ui->serviceNameLabel->show();
    ui->serviceNameLineEdit->show();
    ui->priceServiceLabel->show();
    ui->priceServiceLineEdit->show();
    ui->changeServiceView->show();
    ui->cancelAddServiceButton->show();
    ui->serviceNameLineEdit->clear();
    ui->priceServiceLineEdit->clear();
}

void BrokerMainWindow::slotAddServiceButtonLastClicked()
{
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->close();
    ui->serviceNameLabel->close();
    ui->serviceNameLineEdit->close();
    ui->priceServiceLabel->close();
    ui->priceServiceLineEdit->close();
    ui->changeServiceView->close();
    ui->cancelAddServiceButton->close();
    serviceName = ui -> serviceNameLineEdit -> text();
    servicePrice = ui -> priceServiceLineEdit -> text();

    if ((serviceName.size() < 6 || serviceName.size() > 32) || (servicePrice.size() < 6 || servicePrice.size() > 32))
    {
        QMessageBox::information(nullptr, "Информация", "Длина полей должна быть от 6 до 32 символов", QMessageBox::Ok);
        return;
    }

    for (const auto& ch : serviceName)
    {
        if (!ch.isDigit() && !ch.isLetter() && ch != ' ')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные", QMessageBox::Ok);
            return;
        }
    }

    for (const auto& ch : servicePrice)
    {
        if (!ch.isDigit() && ch != '.')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные о цене", QMessageBox::Ok);
            return;
        }
    }

    emit serviceAddButtonLastClicked();
}

void BrokerMainWindow::slotDeleteServiceButtonClicked()
{
    std::string serviceNameAndPrice;

    bool isSelected = false;
    for (std::size_t i = 0; i < ui -> serviceWidget -> count(); i++)
    {
        if (ui -> serviceWidget -> item(i) -> isSelected())
        {
            serviceNameAndPrice = ui -> serviceWidget -> item(i) -> text().toStdString();
            isSelected = true;
        }
    }

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите услугу", QMessageBox::Ok);
        return;
    }

    serviceName = serviceNameAndPrice.substr(0, serviceNameAndPrice.find(',')).c_str();
    serviceNameAndPrice.erase(0, serviceNameAndPrice.find(':') + 2);
    servicePrice = serviceNameAndPrice.c_str();

    emit serviceDeleteButtonClicked();
}

void BrokerMainWindow::slotChangeServiceButtonLastClicked()
{
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->close();
    ui->serviceNameLabel->close();
    ui->serviceNameLineEdit->close();
    ui->priceServiceLabel->close();
    ui->priceServiceLineEdit->close();
    ui->changeServiceView->close();
    ui->cancelAddServiceButton->close();

    QString new_service, new_price;
    new_service = ui -> serviceNameLineEdit -> text();
    new_price = ui -> priceServiceLineEdit -> text();

    if ((new_service.size() < 6 || new_service.size() > 32) || (new_price.size() < 6 || new_price.size() > 32))
    {
        QMessageBox::information(nullptr, "Информация", "Длина полей должна быть от 6 до 32 символов", QMessageBox::Ok);
        return;
    }

    for (const auto& ch : new_service)
    {
        if (!ch.isDigit() && !ch.isLetter() && ch != ' ')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные о названии услуги", QMessageBox::Ok);
            return;
        }
    }

    for (const auto& ch : new_price)
    {
        if (!ch.isDigit() && ch != '.')
        {
            QMessageBox::information(nullptr, "Информация", "Неверно введены данные о цене", QMessageBox::Ok);
            return;
        }
    }



    bool isSelected = false;
    std::string serviceNameAndPrice;
    for (std::size_t i = 0; i < ui -> serviceWidget -> count(); i++)
    {
        if (ui -> serviceWidget -> item(i) -> isSelected())
        {
            serviceNameAndPrice = ui -> serviceWidget -> item(i) -> text().toStdString();
            isSelected = true;
        }
    }

    serviceName = serviceNameAndPrice.substr(0, serviceNameAndPrice.find(',')).c_str();
    serviceNameAndPrice.erase(0, serviceNameAndPrice.find(':') + 2);
    servicePrice = serviceNameAndPrice.c_str();

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите компанию", QMessageBox::Ok);
        return;
    }

    serviceName += "~~~" + new_service + "~~~";
    servicePrice += "~~~" + new_price + "~~~";

    emit serviceChangeButtonLastClicked();
}

void BrokerMainWindow::slotCancelAddServiceButtonClicked()
{
    ui->changeServiceButtonLast->close();
    ui->addServiceButtonLast->close();
    ui->serviceNameLabel->close();
    ui->serviceNameLineEdit->close();
    ui->priceServiceLabel->close();
    ui->priceServiceLineEdit->close();
    ui->changeServiceView->close();
    ui->cancelAddServiceButton->close();
}

void BrokerMainWindow::slotCancelRequestButtonClicked()
{
    std::string loginAndService;

    bool isSelected = false;
    for (std::size_t i = 0; i < ui -> handleWidget -> count(); i++)
    {
        if (ui -> handleWidget -> item(i) -> isSelected())
        {
            loginAndService = ui -> handleWidget -> item(i) -> text().toStdString();
            isSelected = true;
        }
    }

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите заявку", QMessageBox::Ok);
        return;
    }

    orderLogin = loginAndService.substr(0, loginAndService.find('-')).c_str();
    loginAndService.erase(0, loginAndService.find('-') + 3);
    serviceName = loginAndService.c_str();

    emit cancelRequestButtonClicked();
}

void BrokerMainWindow::slotHandleRequestButtonClicked()
{
    std::string loginAndService;

    bool isSelected = false;
    for (std::size_t i = 0; i < ui -> handleWidget -> count(); i++)
    {
        if (ui -> handleWidget -> item(i) -> isSelected())
        {
            loginAndService = ui -> handleWidget -> item(i) -> text().toStdString();
            isSelected = true;
        }
    }

    if (!isSelected)
    {
        QMessageBox::information(nullptr, "Информация", "Выберите заявку", QMessageBox::Ok);
        return;
    }

    orderLogin = loginAndService.substr(0, loginAndService.find('-')).c_str();
    loginAndService.erase(0, loginAndService.find('-') + 3);
    serviceName = loginAndService.c_str();

    emit handleRequestButtonClicked();
}
