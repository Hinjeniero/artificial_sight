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
    bool detect_frame(cv::Mat descriptors);
    void detect_objects();
    void train_matcher();
    void save_collection();
    void load_collection();
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
    cv::Ptr<cv::ORB> ULTRADETECTOR = cv::ORB::create();
    cv::Ptr<cv::BFMatcher> ULTRAMATCHER = cv::BFMatcher::create(NORM_HAMMING);

    struct ObjectList{
        std::vector<std::vector<cv::Mat>> mlist; //Second lvl vector, vector of images
        std::vector<std::vector<cv::Mat>> dlist; //descriptors list
        bool isEmpty = true;

        void addNewElement(cv::Mat elementImage, cv::Mat elementDescriptors){
            if (isEmpty)
                isEmpty = false;
            std::vector<cv::Mat> aux;
            aux.push_back(elementImage);
            mlist.push_back(aux);
            dlist.push_back(elementDescriptors);
        }

        void addImageToElement(int indexElement, cv::Mat image){
            mlist[indexElement].push_back(image);
        }

        void addDescriptorsToElement(int indexElement, cv::Mat descriptors){
            dlist[indexElement].push_back(descriptors);
        }

        void delImageFromElement(int indexElement, int indexImage){
            mlist[indexElement].erase(mlist[indexElement].begin()+indexImage);
            if (mlist[indexElement].size() <= 0){//If the object contains no more images
                mlist.erase(mlist.begin()+indexElement); //We delete the object
                if (mlist.size() <= 0) //IF the collection has no more objects
                    isEmpty = true;
            }
        }

        cv::Mat getImageFromElement(int indexElement, int indexImage){
            return mlist[indexElement][indexImage];
        }

        int size(){
            return mlist.size();
        }

        int elementSize(int indexElement){
            return mlist[indexElement].size();
        }

        std::vector<std::vector<cv::Mat>> get_object_list(){
            return mlist;
        }

        std::vector<std::vector<cv::Mat>> get_descriptors_list(){
            return dlist;
        }
    };
    typedef struct ObjectList ObjectList;

    ObjectList collection;

public slots:
    void compute();
    void start_stop_capture(bool start);
    void change_color_gray(bool color);

    void add_to_collection();
    void delete_from_collection();

    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H
