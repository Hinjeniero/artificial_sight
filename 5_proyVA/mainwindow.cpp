#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showColorImage = false;

    imgS = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    imgS_D = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorS = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgS, ui->imageFrameS);
    visorS_D = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgS_D, ui->imageFrameS_Down);
    imgD = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    imgD_D = new QImage(MAX_WIDTH,MAX_HEIGHT, QImage::Format_RGB888);
    visorD = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgD, ui->imageFrameD);
    visorD_D = new RCDraw(MAX_WIDTH,MAX_HEIGHT, imgD_D, ui->imageFrameD_Down);

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

    currentRegions.create(MAX_HEIGHT, MAX_WIDTH, CV_16UC1);

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(load_from_file()));
    connect(ui->initDispButton,SIGNAL(clicked(bool)),this,SLOT(initializeDisparity()));
    connect(ui->propDispButton,SIGNAL(clicked(bool)),this,SLOT(propagateDisparity()));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    timer.start(60);

    fillEuclideanMatrixes();
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
    if (!currentCorners.empty()){ //If we got an image loaded and got them cornerz :P
        if(ui->showCornersCheck->isChecked()) //If showCorners is checked
            drawCorners(currentCorners, visorS);
    }
    //analyzeAllRegions();
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

void MainWindow::fillEuclideanMatrixes(){
    float threshold = BLOCKSIZE; //<sqrt((BLOCKSIZE/2)*(BLOCKSIZE/2) + (BLOCKSIZE/2)*(BLOCKSIZE/2));
    for (int i=0; i<MAX_HEIGHT; i++){
        for (int j=0; j<MAX_WIDTH; j++){
            double result = sqrt(i*i + j*j);
            euclideanDistances[i][j] = result;
            if (result <= threshold)
                euclideanNeighbors[i][j] = true; //With those distances, it's a neighbor
        }
    }
    euclideanNeighbors[BLOCKSIZE/2][BLOCKSIZE/2] = true; //Cuz yes
}

/*
 *  Get all the corners detected by the harris filter, accodring to the defined values
 */
void MainWindow::getCorners(){
    std::vector<Corner> corners;
    Mat outImage = Mat::zeros(grayImage.size(), CV_32FC1);
    cv::cornerHarris(grayImage, outImage, BLOCKSIZE, APERTURE, K_VALUE);
    for (int i = 0; i<outImage.rows; i++){
        for (int j=0; j<outImage.cols; j++){
            if(outImage.at<float>(i, j) > HARRIS_THRESHOLD){
                Corner c(i, j, outImage.at<float>(i, j));
                corners.push_back(c);
            }
        }
    }
    std::sort(corners.begin(), corners.end(), std::greater<Corner>()); //Ordering the vector

    qDebug()<<"Size before -> " << corners.size();

    int xDifference, yDifference, xPoint, yPoint;
    for (unsigned int i=0; i<corners.size()-1; i++){
        xPoint = corners[i].p.x;
        yPoint = corners[i].p.y;
        for (unsigned int j=i+1; j<corners.size(); j++){
           xDifference = xPoint - corners[j].p.x;
           yDifference = yPoint - corners[j].p.y;
           if (euclideanNeighbors[xDifference][yDifference]){ //If it's a neighbor sqrt(xDifference*xDifference + yDifference*yDifference) <= BLOCKSIZE
               corners.erase(corners.begin() + j);
           }
       }
    }

    qDebug()<<"Size after -> " << corners.size();

    for (unsigned int i=0; i<corners.size(); i++){
        currentCorners.push_back(corners[i].p);
        qDebug() <<"(" <<corners[i].p.x <<","<<corners[i].p.y<<")" <<", hvalue "<<corners[i].harrisValue;
    }

    xDifference = abs(corners[0].p.x - corners[1].p.x);
    yDifference = abs(corners[0].p.y - corners[1].p.y);
    qDebug() <<"(" << xDifference <<","<< yDifference<<")";
    qDebug() << euclideanNeighbors[xDifference][yDifference];

//    analyzeCorners(currentCorners);
}

void MainWindow::drawCorners(std::vector<Point> corners, RCDraw *visor){
    for (unsigned int i=0; i<corners.size(); i++){
        visor->drawSquare(QPoint(corners[i].x, corners[i].y), CROSSHAIR_SIZE, CROSSHAIR_SIZE, Qt::green);
    }
}

//TODO: test
void MainWindow::analyzeCorners(){
    Mat result = Mat::zeros(1, MAX_WIDTH, CV_32FC1);
    int half_width = WIDTH_VALUE/2;
    for (unsigned int i=0; i<currentCorners.size(); i++){
        if (currentCorners[i].x > half_width && currentCorners[i].y > half_width){
            Mat rect = grayImage(Rect(currentCorners[i].x-half_width, currentCorners[i].y-half_width, WIDTH_VALUE, WIDTH_VALUE));
            Mat tmpl = grayImage_2(Rect(currentCorners[i].x-half_width, 0, WIDTH_VALUE, MAX_WIDTH)); //TODO Crop the row to < Xr so we dont have to compute so much.
            cv::matchTemplate(tmpl, rect, result, TM_CCOEFF_NORMED); //Return a row with the values in our case
            Point maxIndex;
            cv::minMaxLoc(result, NULL, NULL, NULL, &maxIndex); //Checking which position of the window was the best
            if(result.at<float>(maxIndex.x, maxIndex.y) > LOC_THRESHOLD){
                currentDisparities.at<short>(currentCorners[i].x, currentCorners[i].y) = abs(currentCorners[i].x-maxIndex.x);
                fixedSpots.at<bool>(currentCorners[i].x, currentCorners[i].y) = 1;
            }

        }
    }
}


/**
 *Analyze all the regions, assigning them to the Mat imgReg, and draws them in the right panel
 *Also draws the borders if the box is checked
 */
void MainWindow::analyzeAllRegions(){
    Mat analyzed_pixels;
    int i, j, regionIndex;
    analyzed_pixels = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_8UC1);

    for (i=0; i<currentRegions.rows; i++){ //Filling with -1's
        for (j=0; j<currentRegions.cols; j++){
            currentRegions.at<int>(i, j) = -1;
        }
    }
    regionIndex = 0;
    for (i=0; i<currentRegions.rows; i++){ //This is the imgRegions filled with -1's, from 0 to 240
        for (j=0; j<currentRegions.cols; j++){ //From 0 to 320
            if (analyzed_pixels.at<uchar>(i, j) == 0){ //If it hasn't been analyzed yet
                Region reg(regionIndex, i, j, grayImage.at<uchar>(i, j)); //Creating region
                //reg.setElements(regionIndex, Point(i, j), 0, grayImage.at<uchar>(i, j)); //Setting elements of region
                analyzeRegion(Point(i, j), currentRegions, reg, analyzed_pixels); //We analyze this region
                allCurrentRegions[regionIndex] = reg; //Assign it to the map
                regionIndex++; //Prepare for the next region
            }
        }
    }
}

/**
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

//TODO: test
void MainWindow::initializeDisparity(){
    getCorners(); //Getting and filling currentCorners
    analyzeCorners(); //Getting and filling currentDisparities and fixedSpots
    analyzeAllRegions(); //Getting and filling currentRegions and allCurrentRegions

    std::map <short, std::tuple<short, short, float>> regionsValues;  // Array: First one is the Point Count, second one is the Total disparity value, third one is the mean value of each region

    for (auto regionInfo: regionsValues){ //Initializing the tuple
        std::get<0>(regionInfo.second) = 0;
        std::get<1>(regionInfo.second) = 0;
        std::get<2>(regionInfo.second) = 0;
    }

    for (int i=0; i<fixedSpots.rows; i++){ //Getting the number of points of each region and their disparity values
        for (int j=0; j<fixedSpots.cols; j++){
            if (fixedSpots.at<bool>(i, j)){ //This is a fixed point
                std::get<0>(regionsValues[currentRegions.at<short>(i, j)]) += 1; //We add one to the point count of the fixed point's region.
                std::get<1>(regionsValues[currentRegions.at<short>(i, j)]) += currentDisparities.at<float>(i, j); //We add the disparity value to the total disparity value of the region
            }
        }
    }
    for (auto regionInfo: regionsValues){ //calculating the mean of each region
        if(std::get<0>(regionInfo.second) > 0) //If there more than 0 fixed points in that region
             std::get<2>(regionInfo.second) =  std::get<1>(regionInfo.second)/ std::get<0>(regionInfo.second); //Calculating the mean for each region
    }


    for (int i=0; i<currentRegions.rows; i++){ //Pasting the mean value in each not fixed point
        for (int j=0; j<currentRegions.cols; j++){
            if (!fixedSpots.at<bool>(i, j))
                currentDisparities.at<float>(i, j) = std::get<2>(regionsValues[currentRegions.at<short>(i, j)]);
        }
    }
    ui->initDispButton->setChecked(false);
    initializeDone = true;
}

//TODO: test
void MainWindow::propagateDisparity(){
    int vicinity = ui->vicinitySizeBox->value();
    int vicinitySize = vicinity/2;
    if(!initializeDone)
        initializeDisparity();
    for (int i=0; i<fixedSpots.rows; i++){ //Looping through the fixed points
        for (int j=0; j<fixedSpots.cols; j++){
            //Here starts the good shit
            if (!fixedSpots.at<bool>(i, j)){ //This is not a fixed point
                float total = 0;
                for (int n=i-vicinitySize; n>i+vicinitySize; n++){ //Looping through the vicinity of the point, including itself
                    for (int m=j-vicinitySize; m>j+vicinitySize; m++){
                        if(currentRegions.at<short>(i, j) == currentRegions.at<short>(n, m)){ //If both points belong to the same region
                            if(n<0 || m<0 || n>MAX_HEIGHT || m>MAX_WIDTH) //Checking if out of bounds, you know, the borders and all that
                                total += currentDisparities.at<float>(m, n);
                        }
                    }
                }
                currentDisparities.at<float>(i, j) = total/(vicinity*vicinity);//Assigning the mean to the point
            }

        }
    }
}

//TODO: test
void MainWindow::visualRepresentation(){


}

/**
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
        auxImage_2.copyTo(colorImage_2);
        cvtColor(colorImage_2, grayImage_2, CV_BGR2GRAY);

        if(colorImage_2.empty())//invalid input
           qDebug() << "Could not open or find " << imagepaths[1];
        imgLoaded = true;
        allCurrentRegions.clear(); //In case we loaded an image before, we need to get rid of this
        initializeDone = false;
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

/*
//               qDebug("YYEEESSS");
//               qDebug() << euclideanDistances[xDifference][yDifference];
//               qDebug() <<"xdif ->"<<xDifference<<", ydif ->"<<yDifference;
//               qDebug("ENDYYEEESSS");
//                   if(corners[j].p.x == 176 && corners[j].p.y == 30){
//                       qDebug("----Yes----");
//                       qDebug() << euclideanDistances[xDifference][yDifference];
//                       qDebug() <<"xdif ->"<<xDifference<<", ydif ->"<<yDifference;
//                       qDebug() << "Point1<"<<xPoint<<","<<yPoint<<">, Point2<"<<corners[j].p.x<<","<<corners[j].p.y<<">";
//                       qDebug("----end Yes----");
//                   }
*/

