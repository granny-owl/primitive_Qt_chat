#include "client.h"
#include <QDataStream>

Client::Client(sock_ptr_t &&foo, int bar){
    client_socket_ = std::move(foo);
    client_id_ = bar;
    QObject::connect(client_socket_.get(), SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(displayError(QAbstractSocket::SocketError)));
}

auto Client::get_socket_ptr() const -> const sock_ptr_t &{
    return client_socket_;
}

int Client::get_id(){
    return client_id_;
}

QTcpSocket* Client::get_socket(){
    return client_socket_.get();
}

void Client::write(QString mes){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << mes;
    client_socket_->write(block);
}

void Client::displayError(QAbstractSocket::SocketError err) const{
    const auto prefix = "\nClient::displayError:";
    qDebug().noquote() << prefix << err;
}

