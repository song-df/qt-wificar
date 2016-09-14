#ifndef CARCONTROL_H
#define CARCONTROL_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class CarControl : public QObject
{
    Q_OBJECT
public:
    explicit CarControl(QObject *parent = 0);
    CarControl(const QString &hostip,QObject *parent = 0,qint16 p=8080);
    void setHost(const QString &hostip,qint16 p);
signals:
    
public slots:
    void running(const QString &dir,int steps);
    void forward(int steps);
    void backward(int steps);
    void turnleft(int steps);
    void turnright(int steps);
    void stop();
    void autorun();

private:
    QHostAddress host;
    qint16 port;
    bool isHostOK;
};

#endif // CARCONTROL_H
