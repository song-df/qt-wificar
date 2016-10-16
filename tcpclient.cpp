#include "tcpclient.h"
#include <QPixmap>
TcpClient::TcpClient(bool isCtrl, QObject *parent) :
    QTcpSocket(parent)
{

    connect(this,SIGNAL(connected()),SLOT(slotConnected()));
    connect(this,SIGNAL(disconnected()),SLOT(slotDisconnected()));
    connect(this,SIGNAL(readyRead()),SLOT(slotReadData()));
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(slotError(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(stateChanged(QAbstractSocket::SocketState)),SLOT(slotStatusChange(QAbstractSocket::SocketState)));
    frameLength = 0;
    isCtrlSocket = isCtrl;
    videoBuffer.resize(12000);
    videoHeadBuffer.resize(128);
    isGotFrameHead = false;
}

void TcpClient::slotStatusChange(QAbstractSocket::SocketState state)
{
    Q_UNUSED(state);
    qDebug() << "status:"<<state;
}

void TcpClient::slotReadData()
{

    //qDebug() << bytesAvailable();
    if(!isGotFrameHead && bytesAvailable()>=82)
    {
        QByteArray ba;
        ba.resize(82);
        //read(videoHeadBuffer.data(),82);
        read(ba.data(),82);
        //accessdata(ba);
        //qDebug() << ba;
        bool ok;
        frameLength = QString(ba).mid(42,8).toInt(&ok);
        //videoHeadBuffer.clear();
        if(!ok){
            qDebug() << "got frame length fail";
            exit(-1);
        }
        //qDebug() << "framelength = " << frameLength;
        isGotFrameHead = true;
        //ui->statusLabel->setText(QString::number(frameLength));
    }

    if(isGotFrameHead && frameLength > 0 && bytesAvailable() >= frameLength)
    {
        QByteArray ba;
        ba.resize(frameLength);
        //read(videoBuffer.data(),frameLength);
        read(ba.data(),frameLength);
        QPixmap pix;
        pix.loadFromData(ba);
        emit signalUpdateImg(pix);
        //videoBuffer.clear();
        isGotFrameHead = false;
        frameLength = 0;
    }
}

void TcpClient::slotConnected()
{
    emit signalConnected(socketDescriptor());
}

void TcpClient::slotDisconnected()
{
    emit signalDisconnected(socketDescriptor());
}

void TcpClient::slotError(QAbstractSocket::SocketError)
{
    emit signalError(errorString());
}
