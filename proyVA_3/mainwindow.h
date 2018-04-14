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
    void show_object_image();
    bool detect_frame(cv::Mat descriptors);
    void detect_objects();
    void train_matcher();
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
    std::vector <QColor> colorList;
    struct ObjectList{
        std::vector<std::vector<cv::Mat>> mlist; //Images list, inception. Second lvl vector, vector of images
        std::vector<std::vector<cv::Mat>> dlist; //Descriptors list
        bool isEmpty = true;

        void add_image_to_element(int indexElement, cv::Mat image, cv::Mat descriptors){
            mlist[indexElement].push_back(image);
            dlist[indexElement].push_back(descriptors);
            isEmpty = false;
        }

        bool collection_is_empty(){
            for (auto object: mlist){
                if (object.size() > 0) //We check every std::vector containing the images of each object, everything contained within mlist
                    return false;
            }
            return true;
        }

        void del_image_from_element(int indexElement, int indexImage){
            mlist[indexElement].erase(mlist[indexElement].begin()+indexImage);
            dlist[indexElement].erase(dlist[indexElement].begin()+indexImage);
            if (collection_is_empty()) //If the object contains no more images
                isEmpty = true;
        }

        cv::Mat get_image_from_element(int indexElement, int indexImage){
            return mlist[indexElement][indexImage];
        }

        int size(){
            return mlist.size();
        }

        int element_size(int indexElement){
            return mlist[indexElement].size();
        }

        void add_empty_object(){
            std::vector<cv::Mat> emptyM;
            std::vector<cv::Mat> emptyD;
            mlist.push_back(emptyM);
            dlist.push_back(emptyD);
        }

        std::vector<std::vector<cv::Mat>> get_image_list(){
            return mlist;
        }

        std::vector<std::vector<cv::Mat>> get_descriptors_list(){
            return dlist;
        }

        int get_element_index(int indexImage){
            for (int i=0; i<mlist.size();i++){
                indexImage -= mlist[i].size();
                if (indexImage < 0)
                    return i+1;
            }
        return -1;
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
    void save_collection();

    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H
