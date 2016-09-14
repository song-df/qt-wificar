#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>
#include <QPixmap>

class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(bool isCtrlSocket,QObject *parent = 0);
    
signals:
    void signalUpdateImg(QPixmap);
    void signalConnected(int);
    void signalDisconnected(int);
    void signalError(QString);
public slots:
    void slotConnected();
    void slotDisconnected();
    void slotReadData();
    void slotError(QAbstractSocket::SocketError);
    void slotStatusChange(QAbstractSocket::SocketState state);
private:
    bool isGotFrameHead;
    int frameLength;
    bool isCtrlSocket;

    QByteArray videoBuffer;
    QByteArray videoHeadBuffer;
};

#endif // TCPCLIENT_H
