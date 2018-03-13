/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QFrame *imageFrameS;
    QFrame *imageFrameD;
    QPushButton *captureButton;
    QPushButton *colorButton;
    QPushButton *loadButton;
    QFrame *processingFrame;
    QComboBox *operationComboBox;
    QLabel *operationLabel;
    QLabel *gaussianLabel;
    QSpinBox *thresholdSpinBox;
    QLabel *gaussianLabel_2;
    QPushButton *kernelButton;
    QPushButton *operOrderButton;
    QPushButton *pixelTButton;
    QSpinBox *gaussWidthBox;
    QLabel *label;
    QPushButton *saveButton;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(860, 398);
        imageFrameS = new QFrame(MainWindow);
        imageFrameS->setObjectName(QStringLiteral("imageFrameS"));
        imageFrameS->setGeometry(QRect(20, 20, 320, 240));
        imageFrameS->setFrameShape(QFrame::StyledPanel);
        imageFrameS->setFrameShadow(QFrame::Raised);
        imageFrameD = new QFrame(MainWindow);
        imageFrameD->setObjectName(QStringLiteral("imageFrameD"));
        imageFrameD->setGeometry(QRect(390, 20, 320, 240));
        imageFrameD->setFrameShape(QFrame::StyledPanel);
        imageFrameD->setFrameShadow(QFrame::Raised);
        captureButton = new QPushButton(MainWindow);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setGeometry(QRect(740, 20, 101, 31));
        captureButton->setCheckable(true);
        captureButton->setChecked(true);
        colorButton = new QPushButton(MainWindow);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setGeometry(QRect(740, 60, 101, 31));
        colorButton->setCheckable(true);
        colorButton->setChecked(false);
        loadButton = new QPushButton(MainWindow);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(740, 100, 101, 31));
        loadButton->setCheckable(false);
        loadButton->setChecked(false);
        processingFrame = new QFrame(MainWindow);
        processingFrame->setObjectName(QStringLiteral("processingFrame"));
        processingFrame->setGeometry(QRect(20, 290, 831, 91));
        processingFrame->setFrameShape(QFrame::StyledPanel);
        processingFrame->setFrameShadow(QFrame::Raised);
        operationComboBox = new QComboBox(processingFrame);
        operationComboBox->setObjectName(QStringLiteral("operationComboBox"));
        operationComboBox->setGeometry(QRect(10, 50, 161, 27));
        operationLabel = new QLabel(processingFrame);
        operationLabel->setObjectName(QStringLiteral("operationLabel"));
        operationLabel->setGeometry(QRect(30, 30, 121, 17));
        gaussianLabel = new QLabel(processingFrame);
        gaussianLabel->setObjectName(QStringLiteral("gaussianLabel"));
        gaussianLabel->setGeometry(QRect(170, 30, 111, 17));
        thresholdSpinBox = new QSpinBox(processingFrame);
        thresholdSpinBox->setObjectName(QStringLiteral("thresholdSpinBox"));
        thresholdSpinBox->setGeometry(QRect(290, 50, 60, 27));
        thresholdSpinBox->setMaximum(255);
        thresholdSpinBox->setValue(120);
        gaussianLabel_2 = new QLabel(processingFrame);
        gaussianLabel_2->setObjectName(QStringLiteral("gaussianLabel_2"));
        gaussianLabel_2->setGeometry(QRect(280, 30, 81, 17));
        kernelButton = new QPushButton(processingFrame);
        kernelButton->setObjectName(QStringLiteral("kernelButton"));
        kernelButton->setGeometry(QRect(550, 40, 111, 41));
        operOrderButton = new QPushButton(processingFrame);
        operOrderButton->setObjectName(QStringLiteral("operOrderButton"));
        operOrderButton->setGeometry(QRect(660, 40, 161, 41));
        pixelTButton = new QPushButton(processingFrame);
        pixelTButton->setObjectName(QStringLiteral("pixelTButton"));
        pixelTButton->setGeometry(QRect(370, 40, 181, 41));
        gaussWidthBox = new QSpinBox(processingFrame);
        gaussWidthBox->setObjectName(QStringLiteral("gaussWidthBox"));
        gaussWidthBox->setGeometry(QRect(200, 50, 60, 27));
        gaussWidthBox->setMinimum(3);
        gaussWidthBox->setMaximum(19);
        gaussWidthBox->setSingleStep(2);
        gaussWidthBox->setValue(3);
        label = new QLabel(MainWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(340, 270, 221, 31));
        label->setAutoFillBackground(true);
        label->setTextFormat(Qt::RichText);
        saveButton = new QPushButton(MainWindow);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(740, 140, 101, 31));
        saveButton->setCheckable(false);
        saveButton->setChecked(false);
        QWidget::setTabOrder(captureButton, colorButton);
        QWidget::setTabOrder(colorButton, loadButton);
        QWidget::setTabOrder(loadButton, saveButton);
        QWidget::setTabOrder(saveButton, operationComboBox);
        QWidget::setTabOrder(operationComboBox, gaussWidthBox);
        QWidget::setTabOrder(gaussWidthBox, thresholdSpinBox);
        QWidget::setTabOrder(thresholdSpinBox, pixelTButton);
        QWidget::setTabOrder(pixelTButton, kernelButton);
        QWidget::setTabOrder(kernelButton, operOrderButton);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Proyecto de Visi\303\263n Artificial", 0));
        captureButton->setText(QApplication::translate("MainWindow", "Stop Capture", 0));
        colorButton->setText(QApplication::translate("MainWindow", "Color Image", 0));
        loadButton->setText(QApplication::translate("MainWindow", "Load Image", 0));
        operationComboBox->clear();
        operationComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Transform pixel", 0)
         << QApplication::translate("MainWindow", "Thresholding", 0)
         << QApplication::translate("MainWindow", "Equalize", 0)
         << QApplication::translate("MainWindow", "Gaussian Blur", 0)
         << QApplication::translate("MainWindow", "Median Blur", 0)
         << QApplication::translate("MainWindow", "Linear Filter", 0)
         << QApplication::translate("MainWindow", "Dilate", 0)
         << QApplication::translate("MainWindow", "Erode", 0)
         << QApplication::translate("MainWindow", "Apply several...", 0)
        );
        operationLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Select operation</span></p></body></html>", 0));
        gaussianLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-weight:600;\">Gaussian width</span></p></body></html>", 0));
        gaussianLabel_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-weight:600;\">Threshold</span></p></body></html>", 0));
        kernelButton->setText(QApplication::translate("MainWindow", "Set Kernel", 0));
        operOrderButton->setText(QApplication::translate("MainWindow", "Set Operation Order", 0));
        pixelTButton->setText(QApplication::translate("MainWindow", "Set Pixel Transformation", 0));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:20pt; font-weight:600;\">Image Processing</span></p></body></html>", 0));
        saveButton->setText(QApplication::translate("MainWindow", "Save Image", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
