#pragma once
#include <QTcpSocket>
#include <QDebug>
#include <QTcpServer>
#include <QString>
#include <algorithm>
#include <memory>

class Client : public QObject
{
    Q_OBJECT
    using sock_ptr_t = std::unique_ptr<QTcpSocket>;
    sock_ptr_t client_socket_;
    int client_id_;

public:
    Client(sock_ptr_t &&foo, int bar);
    auto get_socket_ptr() const -> const sock_ptr_t &;
    int get_id();
    QTcpSocket* get_socket();
public slots:
    void write(QString mes);
    void displayError(QAbstractSocket::SocketError) const;
};