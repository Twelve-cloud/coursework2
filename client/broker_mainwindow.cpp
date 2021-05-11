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
    for (std::size_t i = 0; i < ui -> companyWidget -> count(); i++)
    {
        if (ui -> companyWidget -> item(i) -> isSelected())
        {
            company = ui -> companyWidget -> item(i) -> text();
        }
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

}
