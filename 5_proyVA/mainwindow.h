#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <rcdraw.h>

#define MAX_HEIGHT 240
#define MAX_WIDTH 320
#define CROSSHAIR_SIZE 2
#define BLOCKSIZE 5
#define APERTURE 3
#define K_VALUE 0.04
#define HARRIS_THRESHOLD 0.000001
#define WIDTH_VALUE 11
#define LOC_THRESHOLD 0.8

using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Default methods
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Added methods
    void analyzeCorners();
    void drawCorners(std::vector<Point> corners, RCDraw *visor);
    void visualRepresentation();
    void fillEuclideanMatrixes(); //Method to fill those two LUT matrixes at the start of the program
    bool isEuclideanNeighbor(Point i, Point j);

private:
    /**
     * @brief Region struct, that contains the graylvl, the first point, the id, and the pointcount.
     * The setElements method act as a substitute constructor
     */
    struct Region{
        int id;
        Point firstPoint;
        int pointCount;
        uchar grayLvl;
        std::vector<Point> borders;
        Region(): id(0), firstPoint(0,0), pointCount(1), grayLvl(0){}
        Region(int _id, int x, int y, uchar _grayLvl):
            id(_id), firstPoint(x, y), pointCount(1), grayLvl(_grayLvl){}

        void incrementCount(){
            pointCount++;
        }

        void addPointToBorder(Point p){
            borders.push_back(p);
        }
    };typedef struct Region Region;

    /**
     * @brief The Corner struct, contains a point (coordinates), and the harris value asociated with it.
     */
    struct Corner{
        Point p;
        float harrisValue;
        Corner(int x, int y, float hValue) : p(x, y), harrisValue(hValue){}

        bool operator > (const Corner& cor)const{
            return (harrisValue > cor.harrisValue);
        }

        bool operator < (const Corner& cor)const{
            return (harrisValue < cor.harrisValue);
        }

    };typedef struct Corner Corner;

    //LUT tables to speedup the euclidean distance check for neighbors when deleting corners
    double euclideanDistances[MAX_HEIGHT][MAX_WIDTH];
    bool euclideanNeighbors[MAX_HEIGHT][MAX_WIDTH];

    //Added variables that are needed for this program
    bool imgLoaded = false;
    bool initializeDone = false;
    std::map<int, Region> allCurrentRegions;
    Mat currentRegions;
    std::vector<Point> currentCorners;
    Mat currentDisparities = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, CV_16UC1);
    Mat fixedSpots = Mat::zeros(MAX_HEIGHT, MAX_WIDTH, IPL_DEPTH_1U);

    //Default variables
    Ui::MainWindow *ui;
    QTimer timer;
    VideoCapture *cap;
    RCDraw *visorS, *visorD, *visorS_D, *visorD_D;
    QImage *imgS, *imgD, *imgS_D, *imgD_D;
    Mat colorImage, colorImage_2, grayImage, grayImage_2, destColorImage, destColorImage_2, destGrayImage, destGrayImage_2;
    Mat gray2ColorImage, destGray2ColorImage, gray2ColorImage_2, destGray2ColorImage_2;
    bool capture, showColorImage, winSelected;
    Rect imageWindow;

public slots:
    //Default slots
    void compute();
    void change_color_gray(bool color);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

    //Added methods to slots
    void load_from_file();
    void getCorners();
    void initializeDisparity();
    void propagateDisparity();
    void analyzeAllRegions();
    void analyzeRegion(Point pStart, Mat &imgReg, Region region, Mat &analyzed);
};


#endif // MAINWINDOW_H

//        void setElements(int _id, Point _first, int _pointCount, uchar _grayLvl){
//            id = _id;
//            firstPoint = _first;
//            pointCount = _pointCount;
//            grayLvl = _grayLvl;
//        }
