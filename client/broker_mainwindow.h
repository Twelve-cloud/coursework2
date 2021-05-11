#ifndef BROKER_MAINWINDOW_H
#define BROKER_MAINWINDOW_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class broker_mainwindow;
}

class BrokerMainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BrokerMainWindow(QWidget *parent = nullptr);
    ~BrokerMainWindow();

    void addCompanyLine(const QString& company);
    void addServiceLine(const QString& service);
    void addRequestLine(const QString& request);
    QString getCompany() const noexcept { return company; }
    QString getServiceName() const noexcept { return serviceName; }
    QString getServicePrice() const noexcept { return servicePrice; }
    QString getService() const noexcept { return serviceName + servicePrice + company + "~~~"; }
    QString getOrderLogin() const noexcept { return orderLogin; }
    void clearCompanies();
    void clearRequests();
    void clearServices();

public slots:
    void slotCompanyButtonClicked();
    void slotRequestsButtonClicked();
    void slotCompanyAddButtonClicked();
    void slotCompanyAddButtonLastClicked();
    void slotCompanyCancelAddClicked();
    void slotCompanyDeleteButtonClicked();
    void slotCompanyChangeButtonClicked();
    void slotCompanyChangeButtonLastClicked();
    void slotCompanyDoubleClick(QListWidgetItem* item);
    void slotToolButtonClicked();
    void slotAddServiceButtonClicked();
    void slotChangeServiceButtonClicked();
    void slotAddServiceButtonLastClicked();
    void slotChangeServiceButtonLastClicked();
    void slotDeleteServiceButtonClicked();
    void slotCancelAddServiceButtonClicked();
    void slotCancelRequestButtonClicked();
    void slotHandleRequestButtonClicked();

signals:
    void companyButtonClicked();
    void companyAddButtonLastClicked();
    void companyDeleteButtonClicked();
    void companyChangeButtonLastClicked();
    void companyDoubleClicked();
    void brokerHandleRequestClicked();
    void serviceChangeButtonLastClicked();
    void serviceAddButtonLastClicked();
    void serviceDeleteButtonClicked();
    void cancelRequestButtonClicked();
    void handleRequestButtonClicked();

private:
    QString serviceName;
    QString servicePrice;
    QString company;
    QString orderLogin;
    Ui::broker_mainwindow *ui;
};

#endif // BROKER_MAINWINDOW_H
