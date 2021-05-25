#pragma once
#include <QCoreApplication>
#include <QTcpSocket>
#include <QDataStream>
#include <QTcpServer>
#include <vector>
#include <algorithm>
#include <sstream>
#include "client.h"
#include <memory>

class Server : public QObject
{
    Q_OBJECT
    std::vector<std::unique_ptr<Client>> clients_;
    int count_ = 0;
    std::unique_ptr<QTcpServer> qt_server_;
public slots:
    void on_connection();
    void on_disconnect();
    void on_accept_error(QAbstractSocket::SocketError);
    void list(int, QTcpSocket*);
    void send(int, std::string);
    void on_ready_read();
public:
    Server(QHostAddress addr, int port);
};