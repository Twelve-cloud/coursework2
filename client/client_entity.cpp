#include "client_entity.h"

ClientEntity::ClientEntity(const QString& strHost, const qint32& nPort) : nextBlockSize(0)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket -> connectToHost(strHost, nPort);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &ClientEntity::slotReadyRead);
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
    char com[4];
    tcpSocket -> read(com, 4);

    char sizeStr[4096];
    tcpSocket -> read(sizeStr, 4096);
    int size = atoi(sizeStr);

    char strData[size];
    tcpSocket -> read(strData, size);

    command = com;
    data = strData;

    emit readyRead();
}
