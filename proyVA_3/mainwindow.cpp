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
    connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(add_to_collection()));
    connect(ui->delButton,SIGNAL(clicked(bool)),this,SLOT(delete_from_collection()));
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
    if (ui->objectComboBox->currentIndex() == -1){ //CURRENT INDEX IS -1 WHEN EMPTY
        ui->delButton->setEnabled(false);
    }

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

void MainWindow::add_to_collection(){
    qDebug("add collection");
    Mat aux = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));

    cv::Ptr<cv::ORB> ULTRADETECTOR = cv::ORB::create();
    std::vector <cv::KeyPoint> keypoints;
    Mat descriptors;

    ULTRADETECTOR->detect(aux, keypoints);
    ULTRADETECTOR->compute(aux, keypoints, descriptors);
    //qDebug() << descriptors.cols << "-" <<descriptors.rows;

    std::vector <cv::Mat> element;
    element.push_back(aux); //ONLY PUSHING THE REFERENCE
    element.push_back(descriptors); //ONLY PUSHING THE REFERENCE
    collection.addElement(element);

    QString name = "Object "+ QString::number(collection.size());
    ui->objectComboBox->addItem(name);
    ui->delButton->setEnabled(true); //In case the combobox was empty before
}

void MainWindow::delete_from_collection(){
    qDebug("delete from collection");
    qDebug() << "Deleting object with index " << ui->objectComboBox->currentIndex();
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
        ui->addButton->setEnabled(true);

    }
}

void MainWindow::deselectWindow()
{
    winSelected = false;
    ui->addButton->setEnabled(false);
}


