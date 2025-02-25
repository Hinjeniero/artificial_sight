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
//cv::resize
//winO.copyto(winD) (Haev to copy to window)
//Have to clear image (not window), so lets put a black image over before adjustign everything again
//Set capture to false when loading an image.
//When copying, have to center the part of the image. 240-h/2, 320-w/2
//To create auxiliar image, no need to define it (type/size). Use MAT directly, and opencv will do it
//MatWinO(data) = grayImage (imageWindow)
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

    void load_from_file();
    void save_to_file();
    void copy_image();
    void resize_image();
    void enlarge_image();

    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H
