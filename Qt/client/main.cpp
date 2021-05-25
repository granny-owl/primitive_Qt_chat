#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include "client.h"
#include <iostream>

class console_worker : public QObject{
    Q_OBJECT
public:
    console_worker() = default;
public slots:
    void on_mes(QString);
    void start();
signals:
    void mes(QString);
};

void console_worker::on_mes(QString mes){
    qDebug() << "mes:" << mes;
}

void console_worker::start(){
    std::string line;
    while (std::getline(std::cin, line)){
        QString str(line.data());
        emit mes(str);
    }
}

#include "main.moc" 
int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    auto client = new Client("localhost", 1337);
    QThread *thr = new QThread();
    auto cli_wrkr = new console_worker(); //make cli worker
    cli_wrkr->moveToThread(thr); //send it to different thread

    QCoreApplication::connect(thr, &QThread::started, cli_wrkr, &console_worker::start, Qt::QueuedConnection);
    QCoreApplication::connect(cli_wrkr, &console_worker::mes, client, &Client::write, Qt::QueuedConnection);
    thr->start();
    
    int q = a.exec();
    return q;
}