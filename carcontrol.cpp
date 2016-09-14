#include <QHostAddress>
#include "carcontrol.h"

#include "tcpclient.h"

CarControl::CarControl(QObject *parent) :
    QObject(parent)
{
    host.setAddress("192.168.1.102");
    port = 8080;
    isHostOK = true;
}
CarControl::CarControl(const QString &hostip, QObject *parent, qint16 p) :
    QObject(parent)
{
    host.setAddress(hostip);
    port = p;
    isHostOK = true;
}
void CarControl::setHost(const QString &hostip, qint16 p)
{
    host.setAddress(hostip);
    port = p;
}

void CarControl::running(const QString &dir,int steps)
{
    if(!isHostOK)
        return;
    TcpClient *ctrlCar = new TcpClient(1,this);

    ctrlCar->connectToHost(host,port);
    if(ctrlCar->waitForConnected(5000)){

        QTextStream out(ctrlCar);
        out << "GET /?action=command&command="+dir+"&value="+QString::number(steps)+"\r\n\r\n";
        isHostOK = true;
    }
    else
        isHostOK = false;
}
void CarControl::forward(int steps)
{
    running("car_forward_output",steps);
}

void CarControl::backward(int steps)
{
    running("car_backward_output",steps);
}

void CarControl::turnleft(int steps)
{
    running("car_left_output",steps);
}

void CarControl::turnright(int steps)
{
    running("car_right_output",steps);
}

void CarControl::stop()
{
    running("car_stop_output",0);
}

void CarControl::autorun()
{

}
