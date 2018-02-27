#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ui_pixelTForm.h>
#include <ui_operOrderForm.h>
#include <ui_lFilterForm.h>
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


class PixelTDialog: public QDialog, public Ui::PixelTForm{
    Q_OBJECT
    public:
        PixelTDialog(QDialog *parent=0):QDialog (parent){
            setupUi(this);
        }
};

class OperOrderDialog: public QDialog, public Ui::OperOrderForm{
    Q_OBJECT
    public:
        OperOrderDialog(QDialog *parent=0):QDialog (parent){
            setupUi(this);
        }
};

class LFilterDialog: public QDialog, public Ui::LFilterForm{
    Q_OBJECT
    public:
        LFilterDialog(QDialog *parent=0):QDialog (parent){
            setupUi(this);
        }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PixelTDialog pixelTDialog;
    OperOrderDialog operOrderDialog;
    LFilterDialog lFilterDialog;
    QTimer timer;

    VideoCapture *cap;
    RCDraw *visorS, *visorD;
    QImage *imgS, *imgD;
    Mat colorImage, grayImage, destColorImage, destGrayImage;
    Mat gray2ColorImage, destGray2ColorImage;
    bool capture, showColorImage, winSelected;
    Rect imageWindow;

    std::vector<int> lut_grays;

public slots:
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);

    void transformPixel();
    void thresholding();
    void equalize();
    void guassianBlur();
    void medianBlur();
    void linearFilter();
    void dilate();
    void erode();

    void openPixelDialog();
    void closePixelDialog();
    void openOrderDialog();
    void closeOrderDialog();
    void openFilterDialog();
    void closeFilterDialog();
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H
