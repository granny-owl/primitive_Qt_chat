#include <QCoreApplication>
#include <QTimer>
#include "server.h"
#include "client.h"

#include "main.moc"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server(QHostAddress::Any, 1337);
    int q = a.exec();
    return q;
}
