#include "server.h"
#include "client.h"


Server::Server(QHostAddress addr, int port){
    qt_server_ = std::make_unique<QTcpServer>();
    QCoreApplication::connect(qt_server_.get(), &QTcpServer::newConnection,
                              this, &Server::on_connection);
    QCoreApplication::connect(qt_server_.get(), &QTcpServer::acceptError,
                              this, &Server::on_accept_error);
    qt_server_->listen(addr, port);
}

void Server::on_connection(){
    auto raw_ptr = qt_server_->nextPendingConnection();
    qDebug().noquote() << "\ngot connection\n" << raw_ptr->peerAddress() << "\n";
    auto new_socket = std::unique_ptr<QTcpSocket>(std::move(raw_ptr));
    QCoreApplication::connect(new_socket.get(), &QAbstractSocket::disconnected,
                              this, &Server::on_disconnect);
    QCoreApplication::connect(new_socket.get(), &QIODevice::readyRead, this, &Server::on_ready_read);
    auto new_client = std::make_unique<Client>(std::move(new_socket), count_++);
    auto &cli = clients_.emplace_back(std::move(new_client));

    cli->write("\nconnect successful\n");
}

void Server::on_disconnect(){
    QObject *s = sender();
    QTcpSocket *cli_sock = dynamic_cast<QTcpSocket *>(s);
    auto peer_addr = cli_sock->peerAddress();
    qDebug() << "\ndisconnected\n" << peer_addr;

    auto pred = [&](const auto &cli) {
        return cli->get_socket_ptr()->peerAddress() == cli_sock->peerAddress();
    };
    auto it = std::find_if(clients_.begin(), clients_.end(), pred);
    if (it != clients_.end())
    {
        auto &ptr = *it;
        clients_.erase(it);
    }
}

void Server::on_accept_error(QAbstractSocket::SocketError err){
    const auto prefix = "\nServer::on_accept_error:";
    qDebug() << prefix << err;
}


void Server::list(int id, QTcpSocket* p){
    auto id_str = QString::number(id);
    QString temp = "\n";  
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10); 
    for (auto& el : clients_) {
        temp += "id: ";
        int n = el->get_id();
        temp += QString::number(n);
        if (n == id) temp += " (you)";
        temp += '\n';
    }
    out << temp;
    p->write(block);
}

void Server::send(int id, std::string mes){
    auto pred = [&](auto& el) {
        return el->get_id() == id;
    };
    auto cl_ptr = std::find_if(clients_.begin(), clients_.end(), pred);
    auto cl = cl_ptr->get(); 
    auto socket = cl->get_socket();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << QString::fromStdString(mes);
    socket->write(block); 
}


void Server::on_ready_read(){
    QDataStream ds;
    QObject *s = sender();
    QTcpSocket *cli_sock = dynamic_cast<QTcpSocket*>(s);
    ds.setDevice(cli_sock);
    ds.setVersion(QDataStream::Qt_5_10);
    QString message;
    ds >> message;
    auto pred = [&](auto& el){
        return el->get_socket()->peerAddress() == cli_sock->peerAddress();
    };
    auto cl_ptr = std::find_if(clients_.begin(), clients_.end(), pred);
    auto cl = cl_ptr->get(); 
    qDebug().noquote() << "\nmes:" << message << "\nfrom id:" << cl->get_id() << "\n";
    std::string std_message = message.toStdString();
    std::stringstream ss(std_message);
    std::string temp;
    ss >> temp;
    if (temp == "/list") {
        list(cl->get_id(), cl->get_socket());
    }
    if (temp == "/send") {
        int id;
        ss >> id;
        temp.clear();
        std_message = ss.str();
        std_message.erase(0, 8);
        temp = "\nid ";
        temp += std::to_string(cl->get_id());
        temp += " sent: ";
        temp += std_message;
        send(id, temp);
    }
}