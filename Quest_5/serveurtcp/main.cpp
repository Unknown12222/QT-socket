//#include <QtWidgets>
#include <QtNetwork>

class Server : public QObject
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr) : QObject(parent)
    {
        tcpServer = new QTcpServer(this);

        connect(tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);

        if (!tcpServer->listen(QHostAddress::Any, 12345)) {
            qDebug() << "Server could not start. Error:" << tcpServer->errorString();
        } else {
            qDebug() << "Server started.";
        }
    }

private slots:
    void newConnection()
    {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

        connect(clientSocket, &QTcpSocket::readyRead, this, &Server::readData);
        connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);

        qDebug() << "Client connected.";
    }

    void readData()
    {
        QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
        if (!clientSocket)
            return;

        QString data = QString::fromUtf8(clientSocket->readAll());
        qDebug() << "Received data from client:" << data;

        if (data == "Hello") {
            qDebug() << "Sending response: Bonjour";
            clientSocket->write("Bonjour");
        }
    }

private:
    QTcpServer *tcpServer;
    void initServer();


};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    return a.exec();
}

#include "main.moc"
