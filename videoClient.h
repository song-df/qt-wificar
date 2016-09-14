#ifndef CHATCLIENT_H
#define CHATCLIENT_H
#define ENABLE_OPENCV   0
#include <QWidget>
#include <QHostAddress>
#include "tcpclient.h"

#if ENABLE_OPENCV
//#include <cv.h>
//#include <ml.h>
//#include <cv.hpp>
//#include <cxcore.hpp>
#endif

#include <vector>
#include <string>
#include <QString>

namespace Ui {
    class ChatClient;
}

class videoClient : public QWidget
{
    Q_OBJECT

public:
    explicit videoClient(QWidget *parent = 0);
    ~videoClient();

private:
    Ui::ChatClient *ui;

    QString userName;
    int port;
    QHostAddress *serverIP;
    TcpClient *tcpSocket,*tcpSocket2;

    QTimer *timer;

    void sendRequest();
#if ENABLE_OPENCV
    IplImage *cvxCopyQImage(const QImage &qImage, IplImage *pIplImage=0);
    QImage& cvxCopyIplImage(const IplImage *pIplImage, QImage &qImage);
    IplImage* convertImageToGreyscale(const IplImage *imageSrc);
    int detectFaceInImage(const IplImage *inputImg );
    IplImage* cropImage(const IplImage *img, const CvRect region);
    int loadTrainingData(CvMat ** pTrainPersonNumMat);
#endif
    QTcpSocket *carCtrlSocket;

signals:
    void signalUpdatePixmap(QPixmap);
public slots:

private slots:
    void slotVideoOnoff(bool);
    void slotConnected1(int );
    void slotConnected2();
    void slotDisconnected(int);
    void slotDisconnected2();
    void slotStatus(QAbstractSocket::SocketState);
    void slotError1(QAbstractSocket::SocketError);
    void slotError2(QAbstractSocket::SocketError);
    void slotLedUpCtrl();
    void slotLedDownCtrl();
    void slotUpdateImg(QPixmap pix);
    void slotCarCtrlOnoff(bool state);

    void slotCarCtrlForward_press();
    void slotCarCtrlBackward_press();
    void slotCarCtrlLeft_press();
    void slotCarCtrlRight_press();
    void slotCarCtrlAutoRun();
    void slotCarCtrlStop();
    void on_beepToolButton_pressed();
    void on_beepToolButton_released();
    void on_lightToolButton_pressed();
    void on_lightToolButton_released();

    void on_connectToolButton_toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
private:
    int justValue;
    bool isVideoOn;
    bool    carIsAutoRun;
    int curLed;

    void sendCmd(char c);
#if ENABLE_OPENCV

    QList<QString> personNames;

    int faceWidth;	// Default dimensions for faces in the face recognition database. Added by Shervin.
    int faceHeight;	//	"		"		"		"		"		"		"		"
    int nPersons; // the number of people in the training set. Added by Shervin.
    int nTrainFaces; // the number of training images
    int nEigens; // the number of eigenvalues
    int sum; // the number of acquaintance, for use of caculating probability
    int total; // the number of detection, for use of caculating probability
    int videoIndex; // the index of video tape


    QList<QRect> faces;
    bool startVideo;
    CvHaarClassifierCascade* faceCascade;
    IplImage ** eigenVectArr; // eigenvectors
    CvMat * eigenValMat; // eigenvalues
    CvMat * projectedTrainFaceMat; // projected training faces
    IplImage * pAvgTrainImg;
    CvMat * trainPersonNumMat;
#endif
};

#endif // CHATCLIENT_H







