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



using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    struct Region{
        int id;
        Point firstPoint;
        int pointCount;
        uchar grayLvl;
        std::vector<Point> borders;

        void setElements(int _id, Point _first, int _pointCount, uchar _grayLvl){
            id = _id;
            firstPoint = _first;
            pointCount = _pointCount;
            grayLvl = _grayLvl;
        }

        void incrementCount(){
            pointCount++;
        }

        void addPointToBorder(Point p){
            borders.push_back(p);
        }
    };
    typedef struct Region Region;

    VideoCapture *cap;
    RCDraw *visorS, *visorD;
    QImage *imgS, *imgD;
    Mat colorImage, grayImage, destColorImage, destGrayImage;
    Mat gray2ColorImage, destGray2ColorImage;
    bool capture, showColorImage, winSelected;
    Rect imageWindow;


public slots:
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);
    void analyzeAllRegions();
    void analyzeRegion(Point pStart, Mat image, Mat &analyzed);
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H