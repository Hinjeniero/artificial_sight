#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    imgS = new QImage(320,240, QImage::Format_RGB888);
    visorS = new RCDraw(320,240, imgS, ui->imageFrameS);
    imgD = new QImage(320,240, QImage::Format_RGB888);
    visorD = new RCDraw(320,240, imgD, ui->imageFrameD);

    colorImage.create(240,320,CV_8UC3);
    grayImage.create(240,320,CV_8UC1);
    destColorImage.create(240,320,CV_8UC3);
    destGrayImage.create(240,320,CV_8UC1);
    gray2ColorImage.create(240,320,CV_8UC3);
    destGray2ColorImage.create(240,320,CV_8UC3);

    //TODO fill lut gray table

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->pixelTButton,SIGNAL(clicked(bool)),this,SLOT(openPixelDialog()));
    connect(pixelTDialog.okButton,SIGNAL(clicked(bool)),this,SLOT(closePixelDialog()));
    connect(ui->operOrderButton,SIGNAL(clicked(bool)),this,SLOT(openOrderDialog()));
    connect(operOrderDialog.okButton,SIGNAL(clicked(bool)),this,SLOT(closeOrderDialog()));
    connect(ui->kernelButton,SIGNAL(clicked(bool)),this,SLOT(openFilterDialog()));
    connect(lFilterDialog.okButton,SIGNAL(clicked(bool)),this,SLOT(closeFilterDialog()));
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
//    qDebug() << "guassWidthBox " << ui->gaussWidthBox->value();
//    qDebug() << "thresholdSpinBox " << ui->thresholdSpinBox->value();

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

    switch(ui->operationComboBox->currentIndex()){
        case 0: //Transform pixel
            transformPixel();
            break;
        case 1: //Thresholding
            thresholding();
            break;
        case 2: //Equalize
            equalize();
            break;
        case 3: //Guassian Blur
            guassianBlur();
            break;
        case 4: //Median Blur
            medianBlur();
            break;
        case 5: //Linear filter
            linearFilter();
            break;
        case 6: //Dilate
            dilate();
            break;
        case 7: //Erode
            erode();
            break;
        case 8: //Apply several (Can't process this in the switch)
            break;
    }
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

void MainWindow::transformPixel(){
//    qDebug() << pixelTDialog.origPixelBox1->value();
//    qDebug() << pixelTDialog.origPixelBox2->value();
//    qDebug() << pixelTDialog.origPixelBox3->value();
//    qDebug() << pixelTDialog.origPixelBox4->value();
//    qDebug() << pixelTDialog.newPixelBox1->value();
//    qDebug() << pixelTDialog.newPixelBox2->value();
//    qDebug() << pixelTDialog.newPixelBox3->value();
//    qDebug() << pixelTDialog.newPixelBox4->value();
}

void MainWindow::thresholding(){

}

void MainWindow::equalize(){

}

void MainWindow::guassianBlur(){

}

void MainWindow::medianBlur(){

}

void MainWindow::linearFilter(){

}

void MainWindow::dilate(){

}

void MainWindow::erode(){

}

void MainWindow::openPixelDialog(){
    pixelTDialog.show();
}

void MainWindow::closePixelDialog(){
    pixelTDialog.hide();
}

void MainWindow::openOrderDialog(){
    operOrderDialog.show();
}

void MainWindow::closeOrderDialog(){
    operOrderDialog.hide();
}

void MainWindow::openFilterDialog(){
    lFilterDialog.show();
}

void MainWindow::closeFilterDialog(){
    lFilterDialog.hide();
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


