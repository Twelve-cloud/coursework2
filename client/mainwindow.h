#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "authentification.h"
#include "chat_window.h"
#include "consultant_mainwindow.h"
#include "broker_mainwindow.h"
#include "registration.h"
#include "client_entity.h"
#include "histogram.h"
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

    void handleResult(const QString& command);

public slots:
    void slotRegistrationClicked();
    void slotSignInClicked();
    void slotOrderClicked();
    void slotConsultationClicked();
    void slotCloseConsultationClicked();
    void slotCancelRequestClicked();
    void slotAcceptRequestClicked();
    void slotEndChatClicked();
    void slotSendMessageClicked();
    void slotReadyRead();
    void slotCompanyButtonClicked();
    void slotServiceDoubleClick(QListWidgetItem* item);
    void slotOrderServiceClicked();
    void slotCancelServiceClicked();

private:
    Ui::MainWindow *ui;
    AuthentificationWindow authWindow;
    RegistrationWindow regiWindow;
    ConsultantMainWindow consultantMainWindow;
    BrokerMainWindow brokerMainWindow;
    Histogram histogram;
    ChatWindow chatWindow;
    ClientEntity socket;
    QString role;
    QString userService;
    QString userServicePrice;
};
#endif // MAINWINDOW_H
