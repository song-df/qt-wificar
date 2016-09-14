#include <QApplication>
#include <QSize>
#include <QScreen>
#include <QDebug>
#include "videoClient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QScreen *screen = qApp->primaryScreen();
    QSize screenSize=screen->size();

    videoClient w;
    //w.resize(screenSize.width(),screenSize.height()-100);
    w.show();

    return a.exec();
}
