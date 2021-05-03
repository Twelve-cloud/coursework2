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

    QString getCommand() const noexcept { return command; }
    QString getData() const noexcept { return data; }

private slots:
        void slotReadyRead();

signals:
        void readyRead();

private:
    QTcpSocket* tcpSocket;
    qint32 nextBlockSize;
    QString command;
    QString data;
};

#endif // CLIENT_ENTITY_H
