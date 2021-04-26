#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>

class MyClient : public QWidget
{
Q_OBJECT

public:
    explicit MyClient(const QString& strHost, const qint32 nPort, QWidget* pwgt = 0);

private slots:
    void slotReadyRead   ();
    void slotError       (QAbstractSocket::SocketError error);
    void slotSendToServer();
    void slotConnected   ();

private:
    QTcpSocket* tcpSocket;
    quint16     nextBlockSize;
};

#endif // CLIENT_H
