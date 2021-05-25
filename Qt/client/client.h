#pragma once
#include <QCoreApplication>
#include <QDataStream>
#include <QTcpSocket>
#include <QString>
#include <QDebug>
#include <QString>
#include <memory>

class Client : public QObject
{
    Q_OBJECT
    std::unique_ptr<QTcpSocket> sock_;
public slots:
    void on_ready_read();
    void on_error(QAbstractSocket::SocketError err);
    void write(QString);
public:
    Client(const QString &addr, int port);
};
