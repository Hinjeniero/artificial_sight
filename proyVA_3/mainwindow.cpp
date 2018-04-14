#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <dirent.h>

#define MAX_WIDTH 320
#define MAX_HEIGHT 240
#define MAX_DISTANCE 50 //Distance between saved descriptors and current descriptors
#define NUMBER_ELEMENTS 3 //Number of objects in the list
#define CROSSHAIR_SIZE 5 //Size of rectangles on the coincident keypoints

inline std::vector<QColor> rndColors(){
    std::vector<QColor> colors;
    float currentHue = 0.0;
    for (int i = 0; i < NUMBER_ELEMENTS; i++){
        colors.push_back(QColor::fromHslF(currentHue, 1.0, 0.5) );
        currentHue += 0.618033988749895f;
        currentHue = std::fmod(currentHue, 1.0f);
    }
    return colors;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setTickPosition(QSlider::TicksBothSides);
    cap = new VideoCapture(1);
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

    for (int i=0; i<NUMBER_ELEMENTS; i++){
        QString name = "Object "+ QString::number(i+1);
        ui->objectComboBox->addItem(name);
        collection.add_empty_object();
    }
    colorList = rndColors();
    load_collection(); //Load the existing collection at the start
    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(add_to_collection()));
    connect(ui->delButton,SIGNAL(clicked(bool)),this,SLOT(delete_from_collection()));
    connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(save_collection()));
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

    if (!collection.isEmpty){
        detect_objects();
        ui->saveButton->setEnabled(true);
        ui->horizontalSlider->setEnabled(true);
        if (collection.element_size(ui->objectComboBox->currentIndex()) > 0)
            ui->horizontalSlider->setMaximum(collection.element_size(ui->objectComboBox->currentIndex())-1);
            show_object_image();
    }else
        ui->saveButton->setEnabled(false);

    if(collection.element_size(ui->objectComboBox->currentIndex()) <= 0)
        ui->delButton->setEnabled(false);
    else
        ui->delButton->setEnabled(true);

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
    if (!collection.get_image_list()[ui->objectComboBox->currentIndex()].empty()){ //if the current object has images
        Mat image = collection.get_image_from_element(ui->objectComboBox->currentIndex(), ui->horizontalSlider->value());
        Rect Roi((MAX_WIDTH-image.cols)/2, (MAX_HEIGHT-image.rows)/2, image.cols, image.rows);
        image.copyTo(destGrayImage(Roi));
    }
}

void MainWindow::add_to_collection(){
    Mat selected_rect = grayImage(Rect(imageWindow.x,imageWindow.y,imageWindow.width,imageWindow.height));
    std::vector <cv::KeyPoint> keypoints;
    Mat image, descriptors;
    ULTRADETECTOR->detect(selected_rect, keypoints); //Getting keypoints of the frame
    if (keypoints.size()> 0){ //We neither need an image with no keypoints, or it is useful.
        ULTRADETECTOR->compute(selected_rect, keypoints, descriptors); //Getting descriptors of the keypoints
        selected_rect.copyTo(image);
        collection.add_image_to_element(ui->objectComboBox->currentIndex(), image, descriptors);
        train_matcher();
    }else
        qDebug("Choose a surface with more difference between pixels.");
    winSelected = false;
    ui->addButton->setEnabled(false);
}

void MainWindow::train_matcher(){
    ULTRAMATCHER->clear();
    for (auto descriptors_vector: collection.get_descriptors_list()){ //For each vector of descriptors
        ULTRAMATCHER->add(descriptors_vector); //Training the matcher with each descriptor that the image has
    }
}

void MainWindow::delete_from_collection(){
    if(collection.element_size(ui->objectComboBox->currentIndex()) > 0)
        collection.del_image_from_element(ui->objectComboBox->currentIndex(), ui->horizontalSlider->value());
        train_matcher();
        destGrayImage = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);
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
            objectIdx[collection.get_element_index(dmatch.imgIdx)].push_back(dmatch.queryIdx);
    }

    for (auto object: objectIdx){
        if (object.second.size() > 5){ //If more than 5 matches in this object
            std::vector<cv::Point2f> points;
            for (auto point: object.second)
                points.push_back(keypoints[point].pt);
            cv::Rect rect = cv::boundingRect(points);
            QColor color = colorList[object.first];
            visorS->drawSquare(QPoint(rect.x+rect.width/2, rect.y+rect.height/2), rect.width, rect.height, color);
            visorS->drawText(QPoint(rect.x+rect.width/2, rect.y+rect.height/2), QString("Object ")+QString::number(object.first), 11, color);
            for (unsigned int x=0; x<points.size(); x++){
                visorS->drawSquare(QPoint(points[x].x, points[x].y), CROSSHAIR_SIZE, CROSSHAIR_SIZE, color);
            }
        }
    }
}

void MainWindow::save_collection(){
    qDebug("Saving collection");
    remove("*.mat"); //Deletes all the last collection, if it ever existed
    for (int i=0; i<collection.get_image_list().size(); i++){
        for (int j=0; j<collection.get_image_list()[i].size(); j++){
            cv::imwrite(std::to_string(i)+"_"+std::to_string(j)+".jpg", collection.get_image_from_element(i, j));
        }
    }
    qDebug("Collection saved");
}

void MainWindow::load_collection(){
    qDebug("Trying to load collection");
    DIR *d;
    struct dirent *dir;
    std::vector<std::string> dirlist;
    d = opendir(".");
    if (d){ //Gets all the files in the root dir
        while ((dir = readdir(d)) != NULL)
          dirlist.push_back(dir->d_name);
    }
    for (auto imagepath: dirlist){
        int result = imagepath.find(".jpg"); //Check if the file is a .mat
        if (result != -1){
            int object_number = std::stoi(imagepath.substr(0, imagepath.find("_")));
            if(object_number > (NUMBER_ELEMENTS-1)){
                Mat image = imread(imagepath, IMREAD_COLOR); //Read the file
                std::vector <cv::KeyPoint> keypoints;
                Mat descriptors;
                ULTRADETECTOR->detect(grayImage, keypoints); //Getting keypoints of the actual frame
                ULTRADETECTOR->compute(grayImage, keypoints, descriptors); //Getting descriptors of the keypoints
                collection.add_image_to_element(object_number, image, descriptors);
            }else
                qDebug("Some images were not loaded since they couldnt fit in the current collection");
        }
    }
    qDebug("Collection loaded");
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


