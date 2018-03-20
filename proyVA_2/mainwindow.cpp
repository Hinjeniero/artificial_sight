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
    cap->set(CV_CAP_PROP_FRAME_WIDTH, MAX_WIDTH);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, MAX_HEIGHT);
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

    fill_lut_table();

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));

    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(load_from_file()));
    connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(save_to_file()));

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

void MainWindow::fill_lut_table()
{
    lut_grays.clear();
    int difference_new, difference_orig, init_orig, init_new;
    for (int i=0; i<256; i++){
        if (i<pixelTDialog.origPixelBox2->value()){//first section
            difference_new = pixelTDialog.newPixelBox2->value() - pixelTDialog.newPixelBox1->value();
            difference_orig = pixelTDialog.origPixelBox2->value() - pixelTDialog.origPixelBox1->value();
            init_orig = pixelTDialog.origPixelBox1->value();
            init_new = pixelTDialog.newPixelBox1->value();
        }else if(i<pixelTDialog.origPixelBox3->value()){//second section
            difference_new = pixelTDialog.newPixelBox3->value() - pixelTDialog.newPixelBox2->value();
            difference_orig = pixelTDialog.origPixelBox3->value() - pixelTDialog.origPixelBox2->value();
            init_orig = pixelTDialog.origPixelBox2->value();
            init_new = pixelTDialog.newPixelBox2->value();
        }else{ //third and last section
            difference_new = pixelTDialog.newPixelBox4->value() - pixelTDialog.newPixelBox3->value();
            difference_orig = pixelTDialog.origPixelBox4->value() - pixelTDialog.origPixelBox3->value();
            init_orig = pixelTDialog.origPixelBox3->value();
            init_new = pixelTDialog.newPixelBox3->value();
        }
        lut_grays.push_back((i-init_orig)*((float)difference_new/difference_orig)+init_new);
        //QDebug()<<lut_grays[i];
    }
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
    applyOperation(ui->operationComboBox->currentIndex());
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
}

void MainWindow::transformPixel(){
    for (int i=0; i<MAX_HEIGHT; i++){
        for (int j=0; j<MAX_WIDTH; j++){
            destGrayImage.at<uchar>(i, j) = (uchar)lut_grays[grayImage.at<uchar>(i, j)];
        }
    }
}

void MainWindow::thresholding(){
    threshold(grayImage, destGrayImage, ui->thresholdSpinBox->value(), 255, THRESH_BINARY);
    threshold(colorImage, destColorImage, ui->thresholdSpinBox->value(), 255, THRESH_BINARY);
}

void MainWindow::equalize(){
    equalizeHist(grayImage, destGrayImage);
}

void MainWindow::gaussianBlur(){
    cv::Size2i size (0,0);
    GaussianBlur(grayImage, destGrayImage, size, (double)ui->gaussWidthBox->value()/5);
    GaussianBlur(colorImage, destColorImage, size, (double)ui->gaussWidthBox->value()/5);
}

void MainWindow::medianBlur(){
    cv::medianBlur(grayImage, destGrayImage, 3);
    cv::medianBlur(colorImage, destColorImage, 3);
}

void MainWindow::linearFilter(){
    Mat kernel;
    kernel.create(3, 3, CV_32FC1);
    kernel.at<float>(0,0) = lFilterDialog.kernelBox11->value();
    kernel.at<float>(0,1) = lFilterDialog.kernelBox12->value();
    kernel.at<float>(0,2) = lFilterDialog.kernelBox13->value();
    kernel.at<float>(1,0) = lFilterDialog.kernelBox21->value();
    kernel.at<float>(1,1) = lFilterDialog.kernelBox22->value();
    kernel.at<float>(1,2) = lFilterDialog.kernelBox23->value();
    kernel.at<float>(2,0) = lFilterDialog.kernelBox31->value();
    kernel.at<float>(2,1) = lFilterDialog.kernelBox32->value();
    kernel.at<float>(2,2) = lFilterDialog.kernelBox33->value();
    cv::filter2D(grayImage, destGrayImage, -1, kernel, Point(-1, -1), lFilterDialog.addedVBox->value());
    cv::filter2D(colorImage, destColorImage, -1, kernel, Point(-1, -1), lFilterDialog.addedVBox->value());
}

void MainWindow::dilate(){
    Mat element;
    threshold(grayImage, destGray2ColorImage, ui->thresholdSpinBox->value(), 255, THRESH_BINARY);
    cv::dilate(destGray2ColorImage, destGrayImage, element);
    cv::dilate(colorImage, destColorImage, element);
}

void MainWindow::erode(){
    Mat element;
    threshold(grayImage, destGray2ColorImage, ui->thresholdSpinBox->value(), 255, THRESH_BINARY);
    cv::erode(destGray2ColorImage, destGrayImage, element);
    cv::erode(colorImage, destColorImage, element);
}

void MainWindow::severalFilters(){
    Mat originalGray, originalColor;
    grayImage.copyTo(originalGray);
    colorImage.copyTo(originalColor);
    if(operOrderDialog.firstOperCheckBox->isChecked())
        applyOperation(operOrderDialog.operationComboBox1->currentIndex());
        destGrayImage.copyTo(grayImage);
        destColorImage.copyTo(colorImage);
    if(operOrderDialog.secondOperCheckBox->isChecked())
        applyOperation(operOrderDialog.operationComboBox2->currentIndex());
        destGrayImage.copyTo(grayImage);
        destColorImage.copyTo(colorImage);
    if(operOrderDialog.thirdOperCheckBox->isChecked())
        applyOperation(operOrderDialog.operationComboBox3->currentIndex());
        destGrayImage.copyTo(grayImage);
        destColorImage.copyTo(colorImage);
    if(operOrderDialog.fourthOperCheckBox->isChecked())
        applyOperation(operOrderDialog.operationComboBox4->currentIndex());
    originalGray.copyTo(grayImage);
    originalColor.copyTo(colorImage);
}

void MainWindow::applyOperation(int opIndex){
    switch(opIndex){
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
            gaussianBlur();
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
        case 8: //Apply several
            severalFilters();
            break;
    }
}

void MainWindow::openPixelDialog(){
    pixelTDialog.show();
}

void MainWindow::closePixelDialog(){
    fill_lut_table();
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


