#include "videoClient.h"
#include "ui_videoClient.h"
//#include <QMessageBox>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#if ENABLE_OPENCV
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

#endif


videoClient::videoClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatClient)
{
    ui->setupUi(this);
#if ENABLE_OPENCV
    isVideoOn = false;
    startVideo = false;
    carIsAutoRun = false;
    faceWidth = 92;	// Default dimensions for faces in the face recognition database. Added by Shervin.
    faceHeight = 112;	//	"		"		"		"		"		"		"		"
    nPersons                  = 0; // the number of people in the training set. Added by Shervin.
    nTrainFaces               = 0; // the number of training images
    nEigens                   = 0; // the number of eigenvalues
    sum                       = 0; // the number of acquaintance, for use of caculating probability
    total                     = 0; // the number of detection, for use of caculating probability
    videoIndex                = 1; // the index of video tape
#endif
    //ui->verticalLayout->insertWidget(3,slide);
    QString css = "QGroupBox {"
                "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
                "    stop:0 #DFDFDF, stop: 0.7 #7E7E7E, stop:1 #BCBCBC);"
                "  border: 2px solid #D0F201;"
                "  border-radius: 3px;"
                "};background-color: qlineargradient(spread:repeat, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(0, 0, 0, 232), stop:0.283019 rgba(123, 123, 123, 241), stop:0.580189 rgba(108, 117, 105, 230), stop:1 rgba(255, 255, 255, 235));";
    setStyleSheet(css);



    ui->connectToolButton->setStyleSheet("QToolButton{border-image: url(:/images/button_off.png);}"
                                          "QToolButton:checked{border-image: url(:/images/button_on.png);}");
    ui->connectToolButton->setCheckable(true);

    ui->widget->setVisible(false);


    connect(ui->uptoolButton,SIGNAL(pressed()),
            this,SLOT(slotCarCtrlForward_press()));
    connect(ui->uptoolButton,SIGNAL(released()),
            this,SLOT(slotCarCtrlStop()));

    connect(ui->downtoolButton,SIGNAL(pressed()),
            this,SLOT(slotCarCtrlBackward_press()));
    connect(ui->downtoolButton,SIGNAL(released()),
            this,SLOT(slotCarCtrlStop()));

    connect(ui->leftToolButton,SIGNAL(pressed()),
            this,SLOT(slotCarCtrlLeft_press()));
    connect(ui->leftToolButton,SIGNAL(released()),
            this,SLOT(slotCarCtrlStop()));

    connect(ui->rightToolButton,SIGNAL(pressed()),
            this,SLOT(slotCarCtrlRight_press()));
    connect(ui->rightToolButton,SIGNAL(released()),
            this,SLOT(slotCarCtrlStop()));

    connect(ui->stopToolButton,SIGNAL(clicked()),
            this,SLOT(slotCarCtrlAutoRun()));

    serverIP = new QHostAddress();

    justValue = -1;

//    timer = new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));

    ui->frameLabel->setFrameShape(QFrame::Panel);
    ui->frameLabel->setFrameShadow(QFrame::Sunken);

    ui->frameLabel->setVisible(true);

    ui->statusLabel->setVisible(false);
    ui->timeLabel->setVisible(false);
    ui->downtoolButton->setEnabled(false);
    ui->uptoolButton->setEnabled(false);
    ui->leftToolButton->setEnabled(false);
    ui->rightToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
    ui->beepToolButton->setEnabled(false);
    ui->lightToolButton->setEnabled(false);
    curLed = 0;
#if ENABLE_OPENCV
    if( loadTrainingData( &trainPersonNumMat ))
    {
        faceWidth = pAvgTrainImg->width;
        faceHeight = pAvgTrainImg->height;
    }
#endif

}

videoClient::~videoClient()
{
    delete ui;
}

void videoClient::slotVideoOnoff(bool state)
{
    if(state){
       QString ip = ui->srvLineEdit->text();
       if(!serverIP->setAddress(ip))
       {
           return;
       }
       bool ok;
       port = ui->portLineEdit->text().toInt(&ok);
       if(!ok){
           return;
       }

       tcpSocket = new TcpClient(0,this);
       connect(tcpSocket,SIGNAL(signalUpdateImg(QPixmap)),
               this,SLOT(slotUpdateImg(QPixmap)));
       connect(tcpSocket,SIGNAL(signalConnected(int)),
               this,SLOT(slotConnected1(int)));
       connect(tcpSocket,SIGNAL(signalDisconnected(int)),
               this,SLOT(slotDisconnected(int)));
       connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
               this,SLOT(slotError1(QAbstractSocket::SocketError)));

       tcpSocket->connectToHost(*serverIP,port);

   }
   else{
       tcpSocket->disconnectFromHost();
   }
}

void videoClient::on_connectToolButton_toggled(bool checked)
{
    if(ui->checkBoxEnCar)
        slotCarCtrlOnoff(checked);
    if(ui->checkBoxEnVideo)
        slotVideoOnoff(checked);
}
void videoClient::slotStatus(QAbstractSocket::SocketState state)
{
    qDebug() << state;
}

void videoClient::slotCarCtrlOnoff(bool state)
{
    if(state){
       QString ip = ui->srvLineEdit->text();
       if(!serverIP->setAddress(ip))
       {
           return;
       }
       bool ok;
       port = ui->portLineEdit->text().toInt(&ok);
       if(!ok){
           return;
       }

       carCtrlSocket = new QTcpSocket(this);
       connect(carCtrlSocket,SIGNAL(connected()),
               this,SLOT(slotConnected2()));
       connect(carCtrlSocket,SIGNAL(disconnected()),
               this,SLOT(slotDisconnected2()));
       connect(carCtrlSocket,SIGNAL(error(QAbstractSocket::SocketError)),
               this,SLOT(slotError2(QAbstractSocket::SocketError)));
       carCtrlSocket->connectToHost(*serverIP,ui->ctrlPortLineEdit->text().toShort());
   }
   else{
       carCtrlSocket->disconnectFromHost();
   }
}


void videoClient::slotConnected1(int handle)
{
    Q_UNUSED(handle)
    QTextStream out(tcpSocket);
    out << "GET /?action=tcp\r\n\r\n";
    ui->frameLabel->setVisible(true);
    ui->widget->setVisible(false);
    //cvNamedWindow(windowTitle().toLocal8Bit().data());

}
void videoClient::slotConnected2()
{
    ui->downtoolButton->setEnabled(true);
    ui->uptoolButton->setEnabled(true);
    ui->leftToolButton->setEnabled(true);
    ui->rightToolButton->setEnabled(true);
    ui->stopToolButton->setEnabled(true);
    ui->beepToolButton->setEnabled(true);
    ui->lightToolButton->setEnabled(true);
}
void videoClient::slotDisconnected(int handle)
{
   Q_UNUSED(handle)

}
void videoClient::slotDisconnected2()
{
    ui->downtoolButton->setEnabled(false);
    ui->uptoolButton->setEnabled(false);
    ui->leftToolButton->setEnabled(false);
    ui->rightToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
    ui->beepToolButton->setEnabled(false);
    ui->lightToolButton->setEnabled(false);
}

void videoClient::slotError1(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
}

void videoClient::slotError2(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
    ui->downtoolButton->setEnabled(false);
    ui->uptoolButton->setEnabled(false);
    ui->leftToolButton->setEnabled(false);
    ui->rightToolButton->setEnabled(false);
    ui->stopToolButton->setEnabled(false);
    ui->beepToolButton->setEnabled(false);
    ui->lightToolButton->setEnabled(false);
}

void videoClient::sendCmd(char c)
{
        carCtrlSocket->write(&c,1);
}

void videoClient::slotCarCtrlForward_press()
{
    sendCmd('w');
}

void videoClient::slotCarCtrlBackward_press()
{
    sendCmd('s');
}

void videoClient::slotCarCtrlLeft_press()
{
    sendCmd('a');
}

void videoClient::slotCarCtrlRight_press()
{
    sendCmd('d');
}

void videoClient::slotCarCtrlAutoRun()
{
    sendCmd('r');
}

void videoClient::slotCarCtrlStop()
{
    sendCmd(' ');
}


void videoClient::on_beepToolButton_pressed()
{
    sendCmd('b');
}

void videoClient::on_beepToolButton_released()
{
    sendCmd('B');
}
void videoClient::on_lightToolButton_pressed()
{
    sendCmd('l');
}
void videoClient::on_lightToolButton_released()
{
    sendCmd('L');
}

void videoClient::slotLedUpCtrl()
{
    QString ip = ui->srvLineEdit->text();
    if(!serverIP->setAddress(ip))
    {
        return;
    }
    bool ok;
    port = ui->portLineEdit->text().toInt(&ok);
    if(!ok){
        return;
    }
    qDebug() << "curLed:"<<curLed;
        if(curLed >= 0  && curLed < 8)
        {
            tcpSocket2 = new TcpClient(this);

            tcpSocket2->connectToHost(*serverIP,port);
            tcpSocket2->waitForConnected();
            QTextStream out(tcpSocket2);
            out << "GET /?action=command&command=led_off_output&value="+QString::number(curLed)+"\r\n\r\n";
        }
        if(curLed >= 0 && curLed < 7)
        {
            curLed++;
            tcpSocket2 = new TcpClient(this);

            tcpSocket2->connectToHost(*serverIP,port);
            tcpSocket2->waitForConnected();
            QTextStream out(tcpSocket2);
            out << "GET /?action=command&command=led_on_output&value="+QString::number(curLed)+"\r\n\r\n";

        }

}

void videoClient::slotLedDownCtrl()
{
    QString ip = ui->srvLineEdit->text();
    if(!serverIP->setAddress(ip))
    {
        return;
    }
    bool ok;
    port = ui->portLineEdit->text().toInt(&ok);
    if(!ok){
        return;
    }

    if(curLed >= 0  && curLed < 8)
    {
        tcpSocket2 = new TcpClient(this);

        tcpSocket2->connectToHost(*serverIP,port);
        tcpSocket2->waitForConnected();
        QTextStream out(tcpSocket2);
        out << "GET /?action=command&command=led_off_output&value="+QString::number(curLed)+"\r\n\r\n";
    }
    if(curLed > 0 && curLed < 8)
    {
        curLed--;
        tcpSocket2 = new TcpClient(this);

        tcpSocket2->connectToHost(*serverIP,port);
        tcpSocket2->waitForConnected();
        QTextStream out(tcpSocket2);
        out << "GET /?action=command&command=led_on_output&value="+QString::number(curLed)+"\r\n\r\n";
    }
}

void videoClient::slotUpdateImg(QPixmap pix)
{
#if ENABLE_OPENCV
    IplImage *p = cvxCopyQImage(pix.toImage());
    //cvShowImage(windowTitle().toLocal8Bit().data(),p);
    //cvWaitKey(0);
    if(videoGraySwitch->isChecked()){

        //IplImage* target= p;
        IplImage *greyImg = convertImageToGreyscale(p);

    //    IplImage* target_hsv=cvCreateImage( cvGetSize(target), IPL_DEPTH_8U, 3 );

    //    IplImage* target_hue=cvCreateImage( cvGetSize(target), IPL_DEPTH_8U, 3 );

    //    cvCvtColor(target,target_hsv,CV_BGR2HSV);       //转化到HSV空间

    //    cvSplit( target_hsv, target_hue, NULL, NULL, NULL );    //获得H分量

    //    IplImage* h_plane=cvCreateImage( cvGetSize(target_hsv),IPL_DEPTH_8U,1 );

    //     int hist_size[]={255};          //将H分量的值量化到[0,255]

    //     float ranges[1][2]={ {0,360} };    //H分量的取值范围是[0,360)

    //     CvHistogram* hist;//=cvCreateHist(1, hist_size,0, ranges, 1);

    //     cvCalcHist(&target_hue, hist, 0, NULL);
    //    IplImage* rawImage;
    //    IplImage* result=cvCreateImage(cvGetSize(rawImage),IPL_DEPTH_8U,1);

    //     cvCalcBackProject(&rawImage,result,hist);
        // Perform face detection on the input image, using the given Haar cascade classifier.

        //int fs = detectFaceInImage(greyImg);
        //if(fs > 0){
        //    qDebug() << faces;
            //}
        QImage img;
        QImage image = cvxCopyIplImage(greyImg,img);
        ui->frameLabel->setPixmap(QPixmap::fromImage(image));
    }
    else{
        QImage img;
        QImage image = cvxCopyIplImage(p,img);
        ui->frameLabel->resize(image.size());
        ui->frameLabel->setPixmap(QPixmap::fromImage(image));
    }
#else
    ui->frameLabel->setPixmap(pix);
#endif
}

#if ENABLE_OPENCV
IplImage *videoClient::cvxCopyQImage(const QImage &qImage, IplImage *pIplImage)
{
   if(qImage.isNull()) return pIplImage;

   // 创建IplImage

   if(!pIplImage)
   {
      int w = qImage.width();
      int h = qImage.height();

      pIplImage = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 3);
      if(!pIplImage) return NULL;
   }

   // 复制像素

   int x, y;
   for(x = 0; x < pIplImage->width; ++x)
   {
      for(y = 0; y < pIplImage->height; ++y)
      {
         QRgb rgb = qImage.pixel(x, y);
         cvSet2D(pIplImage, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
      }
   }

   return pIplImage;
}

QImage& videoClient::cvxCopyIplImage(const IplImage *pIplImage, QImage &qImage)
{
   if(!pIplImage) return qImage;

   // 调整qImage的大小

   if(qImage.isNull())
   {
      int w = pIplImage->width;
      int h = pIplImage->height;

      qImage = QImage(w, h, QImage::Format_RGB32);
   }

   // 复制像素

   int x, y;
   for(x = 0; x < pIplImage->width; ++x)
   {
      for(y = 0; y < pIplImage->height; ++y)
      {
         CvScalar color = cvGet2D(pIplImage, y, x);

         int r = color.val[2];
         int g = color.val[1];
         int b = color.val[0];

         qImage.setPixel(x, y, qRgb(r,g,b));
      }
   }

   return qImage;
}

IplImage* videoClient::convertImageToGreyscale(const IplImage *imageSrc)
{
    IplImage *imageGrey=NULL;
    Mat mat;
    // Either convert the image to greyscale, or make a copy of the existing greyscale image.
    // This is to make sure that the user can always call cvReleaseImage() on the output, whether it was greyscale or not.
    if (imageSrc->nChannels == 3)
    {
        imageGrey = cvCreateImage( cvGetSize(imageSrc), IPL_DEPTH_8U, 1 );
        cvCvtColor( imageSrc, imageGrey, CV_BGR2GRAY );
        //blur(,&mat,Size(3,3));
    }
    else
    {
        imageGrey = cvCloneImage(imageSrc);
    }
    return imageGrey;
}

int videoClient::detectFaceInImage(const IplImage *inputImg )
{
    const CvSize minFeatureSize = cvSize(50, 50);
    int flags;
    flags = CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH;	// Only search for 1 face.
    if (startVideo)   //lmzlmzlmz
    {
        flags = CV_HAAR_FIND_BIGGEST_OBJECT;
    }
    //const float search_scale_factor = 1.1f;
    IplImage *detectImg;
    CvMemStorage* storage;
    double t;
    CvSeq* rects;

    storage = cvCreateMemStorage(0);
    cvClearMemStorage( storage );

    // If the image is color, use a greyscale copy of the image.
    detectImg = (IplImage*)inputImg;	// Assume the input image is to be used.
    cvEqualizeHist( detectImg , detectImg );


    // Detect all the faces.
    t = (double)cvGetTickCount();
    rects = cvHaarDetectObjects( detectImg,
                                 faceCascade,
                                 storage,
                                 1.1,//search_scale_factor,
                                 3,
                                 flags,
                                 minFeatureSize );

    // Get the first detected face (the biggest).
    if( rects -> total > 0 )
    {
        for( int i = 0 ; i < rects -> total ; ++i )
        {
            CvRect *r = (CvRect*)cvGetSeqElem( rects, i );

            // check if detected objects is actually a face (contains eyes)
            IplImage* faceImage = cropImage(inputImg , *r);
//            if( eyeDetection( faceImage ) )
            {
              //objects.push_back( *r );
                faces.append(QRect(r->x,r->y,r->width,r->height));
            }
            cvReleaseImage( &faceImage );
        }
    }
    else
    {
        faces.clear( );
        cvReleaseMemStorage( &storage );
        return 0;
    }
    t = (double)cvGetTickCount() - t;

        printf("[Face Detection took %d ms and found %d objects]\n",
               cvRound( t/((double)cvGetTickFrequency()*1000.0) ), rects->total );

//    pos_dir = getCentralPoint(objects);
//    if( pos_dir.direction != -1 )
//        control_motor( pos_dir.direction );

    //cvReleaseHaarClassifierCascade( &cascade );
    //cvReleaseImage( &detectImg );
    cvReleaseMemStorage( &storage );


    return faces.count();	// Return the biggest face found, or (-1,-1,-1,-1).
}

// Returns a new image that is a cropped version of the original image.
IplImage* videoClient::cropImage(const IplImage *img, const CvRect region)
{
    IplImage *imageTmp;
    IplImage *imageRGB;
    CvSize size;
    size.height = img->height;
    size.width = img->width;

    if (img->depth != IPL_DEPTH_8U) {
            printf("ERROR in cropImage: Unknown image depth of %d given in cropImage() instead of 8 bits per pixel.\n", img->depth);
            exit(1);
    }

    // First create a new (color or greyscale) IPL Image and copy contents of img into it.
    imageTmp = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
    cvCopy(img, imageTmp, NULL);

    // Create a new image of the detected region
    // Set region of interest to that surrounding the face
    cvSetImageROI(imageTmp, region);
    // Copy region of interest (i.e. face) into a new iplImage (imageRGB) and return it
    size.width = region.width;
    size.height = region.height;
    imageRGB = cvCreateImage(size, IPL_DEPTH_8U, img->nChannels);
    cvCopy(imageTmp, imageRGB, NULL);	// Copy just the region.

    cvReleaseImage( &imageTmp );
    return imageRGB;
}
int videoClient::loadTrainingData(CvMat ** pTrainPersonNumMat)
{
    CvFileStorage * fileStorage;
    int i;

    // create a file-storage interface
    fileStorage = cvOpenFileStorage( "facedata.xml", 0, CV_STORAGE_READ );
    if( !fileStorage )
    {
        printf("Can't open training database file 'facedata.xml'.\n");
        return 0;
    }

    // Load the person names. Added by Shervin.
    personNames.clear();	// Make sure it starts as empty.
    nPersons = cvReadIntByName( fileStorage, 0, "nPersons", 0 );
    if (nPersons == 0)
    {
        printf("No people found in the training database 'facedata.xml'.\n");
        return 0;
    }
    // Load each person's name.
    for (i=0; i<nPersons; i++)
    {
        QString sPersonName;
        char varname[200];
        snprintf( varname, sizeof(varname)-1, "personName_%d", (i+1) );
        sPersonName = QString(cvReadStringByName(fileStorage, 0, varname ));
        personNames.push_back( sPersonName );
    }

    // Load the data
    nEigens = cvReadIntByName(fileStorage, 0, "nEigens", 0);
    nTrainFaces = cvReadIntByName(fileStorage, 0, "nTrainFaces", 0);
    *pTrainPersonNumMat = (CvMat *)cvReadByName(fileStorage, 0, "trainPersonNumMat", 0);
    eigenValMat  = (CvMat *)cvReadByName(fileStorage, 0, "eigenValMat", 0);
    projectedTrainFaceMat = (CvMat *)cvReadByName(fileStorage, 0, "projectedTrainFaceMat", 0);
    pAvgTrainImg = (IplImage *)cvReadByName(fileStorage, 0, "avgTrainImg", 0);
    eigenVectArr = (IplImage **)cvAlloc(nTrainFaces*sizeof(IplImage *));

    for(i=0; i<nEigens; i++)
    {
        char varname[200];
        snprintf( varname, sizeof(varname)-1, "eigenVect_%d", i );
        eigenVectArr[i] = (IplImage *)cvReadByName(fileStorage, 0, varname, 0);
    }

    // release the file-storage interface
    cvReleaseFileStorage( &fileStorage );

    printf("Training data loaded (%d training images of %d people):\n", nTrainFaces, nPersons);
    printf("People: ");
    if (nPersons > 0)
    {
            qDebug() << personNames[0];
    }

    for (i=1; i<nPersons; i++)
    {
            qDebug() << personNames[i];
    }
    printf(".\n");

    return 1;
}
#endif


void videoClient::paintEvent(QPaintEvent *)
{
    QPainter p(this);
//    QPixmap pix(":/images/vcar.png");
    p.drawPixmap(0,0,QPixmap(":/images/bg.jpg").scaled(size()));
//    p.drawPixmap(11,10,QPixmap(":/images/westlakemakerspace.jpg").scaledToWidth(width()-22));
//    p.drawPixmap((width()-pix.width()/2)/2,height()-pix.height(),pix.scaledToWidth(width()/2));
}

void videoClient::keyPressEvent(QKeyEvent *k)
{
    char c;
    switch(k->key()){
    case Qt::Key_O:
        c='o';
        break;
    case Qt::Key_F:
        c='f';
        break;
    case Qt::Key_W:
        c='w';
        break;
    case Qt::Key_S:
        c='s';
        break;
    case Qt::Key_A:
        c='a';
        break;
    case Qt::Key_D:
        c='d';
        break;
    case Qt::Key_I:
        c='i';
        break;
    case Qt::Key_K:
        c='k';
        break;
    case Qt::Key_J:
        c='j';
        break;
    case Qt::Key_L:
        c='l';
        break;
    case Qt::Key_Space:
    case Qt::Key_X:
        c=' ';
        break;
    case Qt::Key_R:
        c='r';
        break;
    }
    sendCmd(c);
}
