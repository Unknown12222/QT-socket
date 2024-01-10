
#include <QtNetwork>

class Client : public QObject
{
    Q_OBJECT

public:
    Client(QObject *parent = nullptr) : QObject(parent)
    {
        tcpSocket = new QTcpSocket(this);

        connect(tcpSocket, &QTcpSocket::connected, this, &Client::connected);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readData);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &Client::disconnected);

        tcpSocket->connectToHost("localhost", 12345);
    }
    virtual ~Client() {}

private slots:
    void connected()
    {
        qDebug() << "Connected to server.";
        tcpSocket->write("Hello");
    }

    void readData()
    {
        QString data = QString::fromUtf8(tcpSocket->readAll());
        qDebug() << "Received data from server:" << data;
    }

    void disconnected()
    {
        qDebug() << "Disconnected from server.";
        tcpSocket->deleteLater();
    }

private:
    QTcpSocket *tcpSocket;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Client client;
    return a.exec();
}

#include "main.moc"

