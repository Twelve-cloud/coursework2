#include "client_entity.h"

ClientEntity::ClientEntity(const QString& strHost, const qint32& nPort) : nextBlockSize(0)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket -> connectToHost(strHost, nPort);
}

ClientEntity::~ClientEntity() {}

void ClientEntity::sendToServer(const QString& command, const QString& string)
{
    tcpSocket -> write(command.toStdString().c_str(), 4);

    char sizeStr[4096];
    itoa(string.size(), sizeStr, 10);
    tcpSocket -> write(sizeStr, 4096);

    tcpSocket -> write(string.toStdString().c_str());
    tcpSocket -> waitForBytesWritten();
}

void ClientEntity::slotReadyRead()
{
    QDataStream in(tcpSocket);
    QString string;

    while (true)
    {

        if (!nextBlockSize)
        {
            if (tcpSocket -> bytesAvailable() < qint64(sizeof(quint32)))
            {
                break;
            }
            in >> nextBlockSize;
        }

        if (tcpSocket -> bytesAvailable() < nextBlockSize)
        {
            break;
        }
        in >> string;
        handleResult(string);
        nextBlockSize = 0;
    }
}
