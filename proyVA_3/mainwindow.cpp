#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define MAX_WIDTH 320
#define MAX_HEIGHT 240
#define MAX_DISTANCE 50
RNG rng(12345);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBothSides);
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
    if(capture && cap->isOpened())
    {
        *cap >> colorImage;

        cvtColor(colorImage, grayImage, CV_BGR2GRAY);
        cvtColor(colorImage, colorImage, CV_BGR2RGB);

    }

    if (ui->objectComboBox->currentIndex() == -1){ //CURRENT INDEX IS -1 WHEN EMPTY
        ui->delButton->setEnabled(false);
        ui->horizontalSlider->setEnabled(false);
    }else{
        detect_objects();
        ui->delButton->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        ui->horizontalSlider->setMaximum(collection.elementSize(ui->objectComboBox->currentIndex())-1);
        show_object_image();
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

void MainWindow::show_object_image(){
    //To make everything black again
    destGrayImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);
    Mat image = collection.getImageFromElement(ui->objectComboBox->currentIndex(), ui->horizontalSlider->value());

    Rect Roi((MAX_WIDTH-image.cols)/2, (MAX_HEIGHT-image.rows)/2, image.cols, image.rows);
    image.copyTo(destGrayImage(Roi));
}

void MainWindow::add_to_collection(){
    qDebug("add collection");
    Mat selected_rect = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    std::vector <cv::KeyPoint> keypoints;
    Mat image, descriptors;
    ULTRADETECTOR->detect(selected_rect, keypoints); //Getting keypoints of the frame
    ULTRADETECTOR->compute(selected_rect, keypoints, descriptors); //Getting descriptors of the keypoints
    selected_rect.copyTo(image);

    if(!detect_frame(descriptors))//If it already exists in the collection
        collection.addNewElement(image, descriptors);//Adding to the collection
    else{
        collection.addImageToElement(ui->objectComboBox->currentIndex(), selected_rect);
        collection.addDescriptorsToElement(ui->objectComboBox->currentIndex(), descriptors);
    }
    train_matcher();
    QString name = "Object "+ QString::number(collection.size());
    ui->objectComboBox->addItem(name);
}

void MainWindow::train_matcher(){
    ULTRAMATCHER->clear();
    for (auto descriptors_vector: collection.get_descriptors_list()){ //For each vector of descriptors
        ULTRAMATCHER->add(descriptors_vector); //Training the matcher with each descriptor that the image has
    }
}

void MainWindow::delete_from_collection(){
    qDebug("delete from collection");
    qDebug() << "Deleting object with index " << ui->objectComboBox->currentIndex();
}

/*
 * Detects if the selected object exists already
 */
bool MainWindow::detect_frame(cv::Mat descriptors){
    return false;//Do the same as down there, but only with the rectangle.
}

void MainWindow::detect_objects(){
    std::vector <cv::KeyPoint> keypoints;
    Mat descriptors;
    ULTRADETECTOR->detect(grayImage, keypoints); //Getting keypoints of the actual frame
    ULTRADETECTOR->compute(grayImage, keypoints, descriptors); //Getting descriptors of the keypoints
    std::vector<cv::DMatch> matches;
    std::map<int, std::vector<int>> objectIdx;

    ULTRAMATCHER->match(descriptors, matches);

    for (auto dmatch: matches){
        if (dmatch.distance < MAX_DISTANCE)
            objectIdx[dmatch.imgIdx].push_back(dmatch.queryIdx);
    }
    for (auto object: objectIdx){
        if (object.second.size() > 5){ //If more than 5 matches in this object
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
            std::vector<cv::Point2f> points;
            for (auto point: object.second){
                points.push_back(keypoints[point].pt);
                cv::drawMarker(grayImage, points.back(), color, MARKER_TILTED_CROSS, 5);
                cv::drawMarker(colorImage, points.back(), color, MARKER_TILTED_CROSS, 5);
            }
            cv::Rect rect = cv::boundingRect(points);
            cv::rectangle(grayImage, rect, color);
            cv::rectangle(colorImage, rect, color);
            cv::putText(grayImage, "object "+std::to_string(object.first+1),(rect.br()+rect.tl())*0.5, FONT_HERSHEY_SIMPLEX, 0.5, color);
            cv::putText(colorImage, "object "+std::to_string(object.first+1),(rect.br()+rect.tl())*0.5, FONT_HERSHEY_SIMPLEX, 0.5, color);
        }
    }
}

void MainWindow::save_collection(){
    qDebug("Huehuehue");
}

void MainWindow::load_collection(){
    qDebug("Huehuehuheu");
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


