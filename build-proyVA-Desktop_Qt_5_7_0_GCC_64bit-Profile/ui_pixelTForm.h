/********************************************************************************
** Form generated from reading UI file 'pixelTForm.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PIXELTFORM_H
#define UI_PIXELTFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_PixelTForm
{
public:
    QSpinBox *origPixelBox1;
    QSpinBox *origPixelBox4;
    QSpinBox *origPixelBox2;
    QSpinBox *origPixelBox3;
    QSpinBox *newPixelBox4;
    QSpinBox *newPixelBox2;
    QSpinBox *newPixelBox3;
    QSpinBox *newPixelBox1;
    QLabel *origLabel;
    QLabel *origLabel_2;
    QPushButton *okButton;

    void setupUi(QDialog *PixelTForm)
    {
        if (PixelTForm->objectName().isEmpty())
            PixelTForm->setObjectName(QStringLiteral("PixelTForm"));
        PixelTForm->resize(360, 245);
        origPixelBox1 = new QSpinBox(PixelTForm);
        origPixelBox1->setObjectName(QStringLiteral("origPixelBox1"));
        origPixelBox1->setEnabled(true);
        origPixelBox1->setGeometry(QRect(30, 60, 61, 27));
        origPixelBox1->setReadOnly(true);
        origPixelBox1->setMaximum(255);
        origPixelBox4 = new QSpinBox(PixelTForm);
        origPixelBox4->setObjectName(QStringLiteral("origPixelBox4"));
        origPixelBox4->setEnabled(true);
        origPixelBox4->setGeometry(QRect(270, 60, 61, 27));
        origPixelBox4->setReadOnly(true);
        origPixelBox4->setMaximum(255);
        origPixelBox4->setValue(255);
        origPixelBox2 = new QSpinBox(PixelTForm);
        origPixelBox2->setObjectName(QStringLiteral("origPixelBox2"));
        origPixelBox2->setEnabled(true);
        origPixelBox2->setGeometry(QRect(110, 60, 61, 27));
        origPixelBox2->setReadOnly(false);
        origPixelBox2->setMaximum(255);
        origPixelBox2->setValue(85);
        origPixelBox3 = new QSpinBox(PixelTForm);
        origPixelBox3->setObjectName(QStringLiteral("origPixelBox3"));
        origPixelBox3->setEnabled(true);
        origPixelBox3->setGeometry(QRect(190, 60, 61, 27));
        origPixelBox3->setReadOnly(false);
        origPixelBox3->setMaximum(255);
        origPixelBox3->setValue(170);
        newPixelBox4 = new QSpinBox(PixelTForm);
        newPixelBox4->setObjectName(QStringLiteral("newPixelBox4"));
        newPixelBox4->setEnabled(true);
        newPixelBox4->setGeometry(QRect(270, 130, 61, 27));
        newPixelBox4->setReadOnly(false);
        newPixelBox4->setMaximum(255);
        newPixelBox4->setValue(255);
        newPixelBox2 = new QSpinBox(PixelTForm);
        newPixelBox2->setObjectName(QStringLiteral("newPixelBox2"));
        newPixelBox2->setEnabled(true);
        newPixelBox2->setGeometry(QRect(110, 130, 61, 27));
        newPixelBox2->setReadOnly(false);
        newPixelBox2->setMaximum(255);
        newPixelBox2->setValue(85);
        newPixelBox3 = new QSpinBox(PixelTForm);
        newPixelBox3->setObjectName(QStringLiteral("newPixelBox3"));
        newPixelBox3->setEnabled(true);
        newPixelBox3->setGeometry(QRect(190, 130, 61, 27));
        newPixelBox3->setReadOnly(false);
        newPixelBox3->setMaximum(255);
        newPixelBox3->setValue(170);
        newPixelBox1 = new QSpinBox(PixelTForm);
        newPixelBox1->setObjectName(QStringLiteral("newPixelBox1"));
        newPixelBox1->setEnabled(true);
        newPixelBox1->setGeometry(QRect(30, 130, 61, 27));
        newPixelBox1->setReadOnly(false);
        newPixelBox1->setMaximum(255);
        origLabel = new QLabel(PixelTForm);
        origLabel->setObjectName(QStringLiteral("origLabel"));
        origLabel->setGeometry(QRect(110, 20, 161, 21));
        origLabel->setTextFormat(Qt::RichText);
        origLabel_2 = new QLabel(PixelTForm);
        origLabel_2->setObjectName(QStringLiteral("origLabel_2"));
        origLabel_2->setGeometry(QRect(120, 100, 131, 21));
        origLabel_2->setTextFormat(Qt::RichText);
        okButton = new QPushButton(PixelTForm);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(130, 190, 98, 27));
        QWidget::setTabOrder(origPixelBox1, origPixelBox2);
        QWidget::setTabOrder(origPixelBox2, origPixelBox3);
        QWidget::setTabOrder(origPixelBox3, origPixelBox4);
        QWidget::setTabOrder(origPixelBox4, newPixelBox1);
        QWidget::setTabOrder(newPixelBox1, newPixelBox2);
        QWidget::setTabOrder(newPixelBox2, newPixelBox3);
        QWidget::setTabOrder(newPixelBox3, newPixelBox4);
        QWidget::setTabOrder(newPixelBox4, okButton);

        retranslateUi(PixelTForm);

        QMetaObject::connectSlotsByName(PixelTForm);
    } // setupUi

    void retranslateUi(QDialog *PixelTForm)
    {
        PixelTForm->setWindowTitle(QApplication::translate("PixelTForm", "Set pixel transformation", 0));
        origLabel->setText(QApplication::translate("PixelTForm", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Original pixel value</span></p></body></html>", 0));
        origLabel_2->setText(QApplication::translate("PixelTForm", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">New pixel value</span></p></body></html>", 0));
        okButton->setText(QApplication::translate("PixelTForm", "Ok", 0));
    } // retranslateUi

};

namespace Ui {
    class PixelTForm: public Ui_PixelTForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PIXELTFORM_H
