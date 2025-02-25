#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define MAX_HEIGHT 240
#define MAX_WIDTH 320
#define CROSSHAIR_SIZE 1

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

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(load_from_file()));
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

    analyzeAllRegions();

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

/*
 *Analyze all the regions, assigning them to the Mat imgReg, and draws them in the right panel
 *Also draws the borders if the box is checked
 */
void MainWindow::analyzeAllRegions(){
    Mat analyzed_pixels;
    Mat imgReg;
    std::map<int, Region> allRegions;
    int i, j, regionIndex;
    imgReg.create(MAX_HEIGHT, MAX_WIDTH, CV_32SC1);
    analyzed_pixels = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);

    for (i=0; i<imgReg.rows; i++){ //Filling with -1's
        for (j=0; j<imgReg.cols; j++){
            imgReg.at<int>(i, j) = -1;
        }
    }
    regionIndex = 0;
    for (i=0; i<imgReg.rows; i++){ //This is the imgRegions filled with -1's, from 0 to 240
        for (j=0; j<imgReg.cols; j++){ //From 0 to 320
            if (analyzed_pixels.at<uchar>(i, j) == 0){ //If it hasn't been analyzed yet
                Region reg; //Creating region
                reg.setElements(regionIndex, Point(i, j), 0, grayImage.at<uchar>(i, j)); //Setting elements of region
                analyzeRegion(Point(i, j), imgReg, reg, analyzed_pixels); //We analyze this region
                allRegions[regionIndex] = reg; //Assign it to the map
                regionIndex++; //Prepare for the next region
            }
        }
    }
    drawRegionsImage(allRegions, imgReg);
    if (ui->drawBorderCheck->isChecked())
        drawBordersImage(getBordersImage(imgReg));
}

/*
 * Analyze each individual region, to see until which pixel it extends
 */
void MainWindow::analyzeRegion(Point pStart, Mat &imgReg, Region region, Mat &analyzed){
    std::vector<Point> list;
    Point currentP;
    uchar firstGray = region.grayLvl;
    unsigned int i = 0;
    int difference = 0;
    list.push_back(pStart);
    while (i < list.size()){
        currentP = list[i];
        if (currentP.x >= 0 && currentP.x < MAX_HEIGHT && currentP.y >= 0 && currentP.y < MAX_WIDTH
                && analyzed.at<uchar>(currentP.x, currentP.y) == 0){
            difference = abs(firstGray-grayImage.at<uchar>(currentP.x, currentP.y));
            if (difference < ui->grayDifferenceBox->value()){ //If the difference between points is acceptable to belong to the same region
                region.incrementCount(); //We add one to the point count of the region
                analyzed.at<uchar>(currentP.x, currentP.y) = 1; //We mark it as analyzed
                imgReg.at<int>(currentP.x, currentP.y) = region.id; //Assigning in the regions image the id of this one

                //Adding neighbours
                list.push_back(Point(currentP.x-1, currentP.y));
                list.push_back(Point(currentP.x+1, currentP.y));
                list.push_back(Point(currentP.x, currentP.y+1));
                list.push_back(Point(currentP.x, currentP.y-1));
            }
        }
        i++;
    }
    list.clear(); //Freeing the space that the list reserved
}

/*
 * Draws the regions in the right panel
 */
void MainWindow::drawRegionsImage(std::map<int, Region> regions, Mat imgReg){
    for (int i=0; i<imgReg.rows; i++){
        for (int j=0; j<imgReg.cols; j++){
            destGrayImage.at<uchar>(i, j) = regions[imgReg.at<int>(i, j)].grayLvl;
        }
    }
}


/*
 * Returns a list with all the points that are a border of some kind (That have neighbour of another region)
 */
std::vector<Point> MainWindow::getBordersImage(Mat imgReg){
    int currentRegion;
    std::vector<Point> borders;
    for (int i=0; i<imgReg.rows; i++){
        for (int j=0; j<imgReg.cols; j++){
            currentRegion = imgReg.at<int>(i, j);
            //CHecking the 4 direct neighbours
            if (i>0 && (imgReg.at<int>(i-1, j) != currentRegion))
                borders.push_back(Point(i-1, j));
            if (i<MAX_WIDTH-1 && (imgReg.at<int>(i+1, j) != currentRegion))
                borders.push_back(Point(i+1, j));
            if (j>0 && (imgReg.at<int>(i, j-1) != currentRegion))
                borders.push_back(Point(i, j-1));
            if (j<MAX_HEIGHT-1 && (imgReg.at<int>(i, j+1) != currentRegion))
                borders.push_back(Point(i, j+1));
        }
    }
    return borders;
}

/*
 * Draw the points of the borders parameter in a green color
 */
void MainWindow::drawBordersImage(std::vector<Point> borders){
    for (unsigned int i=0; i<borders.size(); i++){
        visorD->drawSquare(QPoint(borders[i].y, borders[i].x), CROSSHAIR_SIZE, CROSSHAIR_SIZE, Qt::green);
    }
}

/*
 * Load a file from the local disk to work on it
 */
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
    ui->loadButton->setChecked(false);
    ui->captureButton->setText("Start capture");
    ui->captureButton->setChecked(false);
    capture = false;

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


