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
    QString getCompany() const noexcept { return company; }
    void clearCompanies();

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

signals:
    void companyButtonClicked();
    void companyAddButtonLastClicked();
    void companyDeleteButtonClicked();
    void companyChangeButtonLastClicked();
    void companyDoubleClicked();

private:
    QString serviceName;
    QString servicePrice;
    QString serviceCompany;
    QString company;
    Ui::broker_mainwindow *ui;
};

#endif // BROKER_MAINWINDOW_H
