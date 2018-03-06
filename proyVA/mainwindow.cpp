#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define MAX_WIDTH 320
#define MAX_HEIGHT 240

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap = new VideoCapture(0);
    if(!cap->isOpened())
        cap = new VideoCapture(1);
    capture = true;
    showColorImage = false;
    winSelected = false;
    cap->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    imgS = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorS = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgS, ui->imageFrameS);
    imgD = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorD = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgD, ui->imageFrameD);

    colorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    grayImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);
    destColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    destGrayImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);
    gray2ColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    destGray2ColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(save_to_file()));
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(load_from_file()));
    connect(ui->copyButton,SIGNAL(clicked(bool)),this,SLOT(copy_image()));
    connect(ui->resizeButton,SIGNAL(clicked(bool)),this,SLOT(resize_image()));
    connect(ui->enlargeButton,SIGNAL(clicked(bool)),this,SLOT(enlarge_image()));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    timer.start(60);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cap;
    delete visorS;
    delete visorD;
    delete imgS;
    delete imgD;

}

void MainWindow::compute()
{

    if(capture && cap->isOpened())
    {
        *cap >> colorImage;

        cvtColor(colorImage, grayImage, CV_BGR2GRAY);
        cvtColor(colorImage, colorImage, CV_BGR2RGB);

    }


    if(showColorImage)
    {
        memcpy(imgS->bits(), colorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destColorImage.data , 320*240*3*sizeof(uchar));
    }
    else
    {
        cvtColor(grayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));

    }

    if(winSelected)
    {
        visorS->drawSquare(QPointF(imageWindow.x+imageWindow.width/2, imageWindow.y+imageWindow.height/2), imageWindow.width,imageWindow.height, Qt::green );
    }
    visorS->update();
    visorD->update();

}

void MainWindow::start_stop_capture(bool start)
{
    if(start)
    {
        ui->captureButton->setText("Stop capture");
        capture = true;
    }
    else
    {
        ui->captureButton->setText("Start capture");
        capture = false;
    }
}

void MainWindow::change_color_gray(bool color)
{
    if(color)
    {
        ui->colorButton->setText("Gray image");
        showColorImage = true;
    }
    else
    {
        ui->colorButton->setText("Color image");
        showColorImage = false;
    }
}


void MainWindow::load_from_file(){
    ui->loadButton->setText("Selecting file");
    QString imagepath =  QFileDialog::getOpenFileName(this,"Load image", QDir::currentPath(),
          "Image files (*.jpg *.jpeg *.bmp *.png) ;; All files (*.*)");
    if(!imagepath.isNull()){
        qDebug() << "selected file path: " << imagepath.toUtf8();
        if (capture){
            ui->captureButton->setChecked(false);
            capture = false;
        }
        Mat auxImage = imread(imagepath.toUtf8().constData(), IMREAD_COLOR); // Read the file
        cvtColor(auxImage,colorImage, CV_RGB2BGR);
        cv::resize(colorImage, auxImage, Size(MAX_WIDTH, MAX_HEIGHT));
        auxImage.copyTo(colorImage);
        cvtColor(colorImage, grayImage, CV_BGR2GRAY);

        if(colorImage.empty())//invalid input
           qDebug("Could not open or find the image");
    }
    ui->loadButton->setText("Load file");
}

void MainWindow::save_to_file(){
    ui->saveButton->setText("Saving file");
    QString imagepath = QFileDialog::getSaveFileName(this, "Save image",
                               QDir::currentPath(),
                               "Images (*.png *.xpm *.jpg *.jpeg *.bmp)");
    if(!imagepath.isNull()){
        qDebug() << "Saving to " << imagepath.toUtf8();
        if(showColorImage)
            imwrite(imagepath.toUtf8().constData(), destColorImage);
        else
            imwrite(imagepath.toUtf8().constData(), destGrayImage);
    }
    ui->saveButton->setText("Save to file");

void MainWindow::copy_image(){
    if (!winSelected){
        colorImage.copyTo(destColorImage);
        grayImage.copyTo(destGrayImage);
        return;
    }

    //To make everything black again
    destColorImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC3);
    destGrayImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);

    Mat auxColor = colorImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    Mat auxGray = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    Rect Roi((MAX_WIDTH-imageWindow.width)/2, (MAX_HEIGHT-imageWindow.height)/2, imageWindow.width, imageWindow.height);
    auxColor.copyTo(destColorImage(Roi));
    auxGray.copyTo(destGrayImage(Roi));

}

void MainWindow::resize_image(){
    if (!winSelected)
        return;
    Mat croppedColorImage = colorImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    Mat croppedGrayImage = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    cv::resize(croppedColorImage, destColorImage, Size(MAX_WIDTH, MAX_HEIGHT));
    cv::resize(croppedGrayImage, destGrayImage, Size(MAX_WIDTH, MAX_HEIGHT));
}

void MainWindow::enlarge_image(){
    if(winSelected){
        int width, height;
        width = MAX_WIDTH;
        height = imageWindow.height * ((float)MAX_WIDTH/imageWindow.width);
        if((float)imageWindow.width/MAX_WIDTH < (float)imageWindow.height/MAX_HEIGHT){
            width = imageWindow.width * ((float)MAX_HEIGHT/imageWindow.height);
            height = MAX_HEIGHT;
        }

        Mat auxColor = colorImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
        Mat auxGray = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));

        Mat resizedColor, resizedGray;
        cv::resize(auxColor, resizedColor, Size(width, height));
        cv::resize(auxGray, resizedGray, Size(width, height));

        //To make everything black again
        destColorImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC3);
        destGrayImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);

        Rect Roi((MAX_WIDTH-width)/2, (MAX_HEIGHT-height)/2, width, height);
        resizedColor.copyTo(destColorImage(Roi));
        resizedGray.copyTo(destGrayImage(Roi));
        return;
    }else{
        colorImage.copyTo(destColorImage);
        grayImage.copyTo(destGrayImage);
    }
}

void MainWindow::selectWindow(QPointF p, int w, int h)
{
    QPointF pEnd;
    if(w>0 && h>0)
    {
        imageWindow.x = p.x()-w/2;
        if(imageWindow.x<0)
            imageWindow.x = 0;
        imageWindow.y = p.y()-h/2;
        if(imageWindow.y<0)
            imageWindow.y = 0;
        pEnd.setX(p.x()+w/2);
        if(pEnd.x()>=320)
            pEnd.setX(319);
        pEnd.setY(p.y()+h/2);
        if(pEnd.y()>=240)
            pEnd.setY(239);
        imageWindow.width = pEnd.x()-imageWindow.x;
        imageWindow.height = pEnd.y()-imageWindow.y;

        winSelected = true;
    }
}

void MainWindow::deselectWindow()
{
    winSelected = false;
}


