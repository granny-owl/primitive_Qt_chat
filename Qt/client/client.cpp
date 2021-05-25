#include "client.h"
#include <iostream>
#include <memory>

void Client::on_ready_read(){
    QDataStream ds;
    ds.setDevice(sock_.get());
    ds.setVersion(QDataStream::Qt_5_10);
    QString message;
    ds >> message;
    qDebug().noquote() << "\nmessage" << message;
}

void Client::on_error(QAbstractSocket::SocketError err){
    QString message = "\nerror occured\n";
    message += sock_->errorString();
    qDebug().noquote() << message;
}


Client::Client(const QString &addr, int port){
    sock_ = std::make_unique<QTcpSocket>(); //new QTcpSocket()
    sock_->connectToHost("localhost", 1337);
    QCoreApplication::connect(sock_.get(), &QTcpSocket::readyRead, this, &Client::on_ready_read);
    QCoreApplication::connect(sock_.get(), SIGNAL(error(QAbstractSocket::SocketError)),
                              this, SLOT(on_error(QAbstractSocket::SocketError)));
}

void Client::write(QString mes){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << mes;
    sock_->write(block);
}