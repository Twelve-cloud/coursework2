#ifndef CLIENT_ENTITY_H
#define CLIENT_ENTITY_H

#include <QWidget>
#include <QTcpSocket>

class ClientEntity : public QWidget
{
    Q_OBJECT

public:
    ClientEntity(const QString& strHost, const qint32& nPort);
    virtual ~ClientEntity();

    void sendToServer(const QString& command, const QString& string);

private slots:
        void slotReadyRead();

private:
    QTcpSocket* tcpSocket;
    qint32 nextBlockSize;
};

#endif // CLIENT_ENTITY_H
