#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#define MAX_HEIGHT 240
#define MAX_WIDTH 320
#define CROSSHAIR_SIZE 1
#define BLOCKSIZE 5
#define K_VALUE 0.04
#define HARRIS_THRESHOLD 0.000001
#define WIDTH_VALUE 11
#define LOC_THRESHOLD 0.8
//std::sort, ordenar std::vector

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showColorImage = false;

    imgS = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorS = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgS, ui->imageFrameS);
    imgD = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorD = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgD, ui->imageFrameD);

    imgS_D = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorS_D = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgS_D, ui->imageFrameS);
    imgD_D = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorD_D = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgD_D, ui->imageFrameD);

    colorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    colorImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);

    grayImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);
    grayImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);

    destColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    destColorImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);

    destGrayImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);
    destGrayImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC1);

    gray2ColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    gray2ColorImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);

    destGray2ColorImage.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);
    destGray2ColorImage_2.create(MAX_HEIGHT,MAX_WIDTH,CV_8UC3);

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
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
    delete visorS_D;
    delete visorD_D;
    delete imgS;
    delete imgD;
    delete imgS_D;
    delete imgD_D;
}

void MainWindow::compute()
{

    analyzeAllRegions();

    if(showColorImage){
        memcpy(imgS->bits(), colorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), colorImage_2.data , 320*240*3*sizeof(uchar));

        memcpy(imgS_D->bits(), destColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD_D->bits(), destColorImage_2.data , 320*240*3*sizeof(uchar));
    }
    else{
        cvtColor(grayImage, gray2ColorImage, CV_GRAY2RGB);
        cvtColor(grayImage_2, gray2ColorImage_2, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage_2,destGray2ColorImage_2, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), gray2ColorImage_2.data , 320*240*3*sizeof(uchar));

        memcpy(imgS_D->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD_D->bits(), destGray2ColorImage_2.data , 320*240*3*sizeof(uchar));
    }

    visorS->update();
    visorD->update();
    visorS_D->update();
    visorD_D->update();
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
 *  Get all the corners detected by the harris filter, accodring to the defined values
 */
Mat MainWindow::getCorners(Mat srcImage){
    Mat outImage;
    Mat corners = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);
    cv::cornerHarris(srcImage, outImage, BLOCKSIZE, BLOCKSIZE, K_VALUE);
    for (int i = 0; i<outImage.rows; i++){
        for (int j=0; j<outImage.cols; j++){
            if(outImage.at<float>(i, j) > HARRIS_THRESHOLD)
                corners.at<uchar>(i, j) = 1;
        }
    }
    return corners;
}

Mat MainWindow::analyzeCorners(Mat corners){
    Mat counterparts = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);
    Mat result = Mat::zeros(1, MAX_WIDTH, CV_32FC1);
    int half_width = WIDTH_VALUE/2;
    for (int i=0; i<corners.rows; i++){
        for (int j=0; j<corners.cols; j++){
            if (i > half_width && j > half_width)
                Mat rect = grayImage(Rect(i-half_width, j-half_width, WIDTH_VALUE, WIDTH_VALUE));
                Mat tmpl = grayImage_2(Rect(i-half_width, 0, WIDTH_VALUE, MAX_WIDTH)); //TODO Crop the row so we dont have to compute so much.
                cv::matchTemplate(tmpl, rect, result, TM_CCOEFF_NORMED); //Return a row with the values in our case
                Point maxIndex;
                cv::minMaxLoc(result, NULL, NULL, NULL, maxIndex); //Checking which position of the window was the best
                if(result.at<float>(maxIndex.x, maxIndex.y) > LOC_THRESHOLD)
                    counterparts.at<uchar>(i, j) = 1;
        }
    }
    return counterparts;
}

/*
 * Load two files from the local disk to work on them
 */
void MainWindow::load_from_file(){
    ui->loadButton->setText("Selecting file");
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::currentPath());
    dialog.setFileMode(QFileDialog::ExistingFile);

    QStringList imagepaths =  dialog.getOpenFileNames(this, "Load images", QDir::currentPath(),
          "Image files (*.jpg *.jpeg *.bmp *.png) ;; All files (*.*)");
    if (imagepaths.size() > 1){ //If it's 2 or more, we will only use the first 2 tho
        qDebug("Selected files: ");
        for (auto imagepath: imagepaths){
            qDebug() << imagepath.toUtf8();
        }

        Mat auxImage = imread(imagepaths[0].toUtf8().constData(), IMREAD_COLOR); // Read the first file
        Mat auxImage_2 = imread(imagepaths[1].toUtf8().constData(), IMREAD_COLOR); // Read the second file

        cvtColor(auxImage,colorImage, CV_RGB2BGR);
        cv::resize(colorImage, auxImage, Size(MAX_WIDTH, MAX_HEIGHT));
        auxImage.copyTo(colorImage);
        cvtColor(colorImage, grayImage, CV_BGR2GRAY);

        if(colorImage.empty())//invalid input
           qDebug() << "Could not open or find " << imagepaths[0];

        cvtColor(auxImage_2, colorImage_2, CV_RGB2BGR);
        cv::resize(colorImage_2, auxImage_2, Size(MAX_WIDTH, MAX_HEIGHT));
        auxImage.copyTo(colorImage_2);
        cvtColor(colorImage_2, grayImage_2, CV_BGR2GRAY);

        if(colorImage_2.empty())//invalid input
           qDebug() << "Could not open or find " << imagepaths[1];

    }
    ui->loadButton->setText("Load Images");
    ui->loadButton->setChecked(false);
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


