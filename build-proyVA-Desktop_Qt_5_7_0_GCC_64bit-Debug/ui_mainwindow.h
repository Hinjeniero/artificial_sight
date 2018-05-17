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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
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
    QPushButton *colorButton;
    QPushButton *loadButton;
    QCheckBox *showCornersCheck;
    QPushButton *initDispButton;
    QPushButton *propDispButton;
    QFrame *imageFrameS_Down;
    QFrame *imageFrameD_Down;
    QPushButton *loadGroundButton;
    QLCDNumber *estDispLcd;
    QLCDNumber *trueDispLcd;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *grayDifferenceBox;
    QSpinBox *vicinitySizeBox;
    QLabel *label_3;
    QLabel *label_4;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(896, 537);
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
        colorButton = new QPushButton(MainWindow);
        colorButton->setObjectName(QStringLiteral("colorButton"));
        colorButton->setEnabled(true);
        colorButton->setGeometry(QRect(740, 60, 131, 31));
        colorButton->setCheckable(true);
        colorButton->setChecked(false);
        loadButton = new QPushButton(MainWindow);
        loadButton->setObjectName(QStringLiteral("loadButton"));
        loadButton->setGeometry(QRect(740, 100, 131, 31));
        loadButton->setCheckable(true);
        loadButton->setChecked(false);
        showCornersCheck = new QCheckBox(MainWindow);
        showCornersCheck->setObjectName(QStringLiteral("showCornersCheck"));
        showCornersCheck->setGeometry(QRect(750, 30, 111, 20));
        initDispButton = new QPushButton(MainWindow);
        initDispButton->setObjectName(QStringLiteral("initDispButton"));
        initDispButton->setGeometry(QRect(740, 140, 131, 31));
        initDispButton->setCheckable(true);
        initDispButton->setChecked(false);
        propDispButton = new QPushButton(MainWindow);
        propDispButton->setObjectName(QStringLiteral("propDispButton"));
        propDispButton->setGeometry(QRect(740, 180, 131, 31));
        propDispButton->setCheckable(true);
        propDispButton->setChecked(false);
        imageFrameS_Down = new QFrame(MainWindow);
        imageFrameS_Down->setObjectName(QStringLiteral("imageFrameS_Down"));
        imageFrameS_Down->setGeometry(QRect(20, 280, 320, 240));
        imageFrameS_Down->setFrameShape(QFrame::StyledPanel);
        imageFrameS_Down->setFrameShadow(QFrame::Raised);
        imageFrameD_Down = new QFrame(MainWindow);
        imageFrameD_Down->setObjectName(QStringLiteral("imageFrameD_Down"));
        imageFrameD_Down->setGeometry(QRect(390, 280, 320, 240));
        imageFrameD_Down->setFrameShape(QFrame::StyledPanel);
        imageFrameD_Down->setFrameShadow(QFrame::Raised);
        loadGroundButton = new QPushButton(MainWindow);
        loadGroundButton->setObjectName(QStringLiteral("loadGroundButton"));
        loadGroundButton->setGeometry(QRect(740, 350, 131, 31));
        loadGroundButton->setCheckable(true);
        loadGroundButton->setChecked(false);
        estDispLcd = new QLCDNumber(MainWindow);
        estDispLcd->setObjectName(QStringLiteral("estDispLcd"));
        estDispLcd->setGeometry(QRect(740, 410, 131, 31));
        trueDispLcd = new QLCDNumber(MainWindow);
        trueDispLcd->setObjectName(QStringLiteral("trueDispLcd"));
        trueDispLcd->setGeometry(QRect(740, 470, 131, 31));
        label = new QLabel(MainWindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(740, 380, 131, 41));
        label_2 = new QLabel(MainWindow);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(750, 440, 101, 41));
        grayDifferenceBox = new QSpinBox(MainWindow);
        grayDifferenceBox->setObjectName(QStringLiteral("grayDifferenceBox"));
        grayDifferenceBox->setGeometry(QRect(830, 230, 41, 31));
        vicinitySizeBox = new QSpinBox(MainWindow);
        vicinitySizeBox->setObjectName(QStringLiteral("vicinitySizeBox"));
        vicinitySizeBox->setGeometry(QRect(830, 280, 41, 31));
        label_3 = new QLabel(MainWindow);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(750, 223, 81, 51));
        label_3->setAlignment(Qt::AlignCenter);
        label_3->setWordWrap(true);
        label_4 = new QLabel(MainWindow);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(750, 270, 81, 51));
        label_4->setLineWidth(1);
        label_4->setAlignment(Qt::AlignCenter);
        label_4->setWordWrap(true);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Proyecto de Visi\303\263n Artificial", 0));
        colorButton->setText(QApplication::translate("MainWindow", "Color Image", 0));
        loadButton->setText(QApplication::translate("MainWindow", "Load Images", 0));
        showCornersCheck->setText(QApplication::translate("MainWindow", "Show corners", 0));
        initDispButton->setText(QApplication::translate("MainWindow", "Initialize disparity", 0));
        propDispButton->setText(QApplication::translate("MainWindow", "Propagate disparity", 0));
        loadGroundButton->setText(QApplication::translate("MainWindow", "Load Ground Truth", 0));
        label->setText(QApplication::translate("MainWindow", "  Estimated disparity", 0));
        label_2->setText(QApplication::translate("MainWindow", "    True disparity", 0));
        label_3->setText(QApplication::translate("MainWindow", "Gray difference", 0));
        label_4->setText(QApplication::translate("MainWindow", "Propagation vicinity size", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
