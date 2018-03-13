/********************************************************************************
** Form generated from reading UI file 'operOrderForm.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPERORDERFORM_H
#define UI_OPERORDERFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_OperOrderForm
{
public:
    QComboBox *operationComboBox1;
    QComboBox *operationComboBox2;
    QComboBox *operationComboBox3;
    QComboBox *operationComboBox4;
    QCheckBox *firstOperCheckBox;
    QPushButton *okButton;
    QCheckBox *secondOperCheckBox;
    QCheckBox *thirdOperCheckBox;
    QCheckBox *fourthOperCheckBox;

    void setupUi(QDialog *OperOrderForm)
    {
        if (OperOrderForm->objectName().isEmpty())
            OperOrderForm->setObjectName(QStringLiteral("OperOrderForm"));
        OperOrderForm->resize(400, 289);
        operationComboBox1 = new QComboBox(OperOrderForm);
        operationComboBox1->setObjectName(QStringLiteral("operationComboBox1"));
        operationComboBox1->setEnabled(true);
        operationComboBox1->setGeometry(QRect(20, 30, 161, 27));
        operationComboBox2 = new QComboBox(OperOrderForm);
        operationComboBox2->setObjectName(QStringLiteral("operationComboBox2"));
        operationComboBox2->setEnabled(false);
        operationComboBox2->setGeometry(QRect(20, 80, 161, 27));
        operationComboBox3 = new QComboBox(OperOrderForm);
        operationComboBox3->setObjectName(QStringLiteral("operationComboBox3"));
        operationComboBox3->setEnabled(false);
        operationComboBox3->setGeometry(QRect(20, 130, 161, 27));
        operationComboBox4 = new QComboBox(OperOrderForm);
        operationComboBox4->setObjectName(QStringLiteral("operationComboBox4"));
        operationComboBox4->setEnabled(false);
        operationComboBox4->setGeometry(QRect(20, 180, 161, 27));
        firstOperCheckBox = new QCheckBox(OperOrderForm);
        firstOperCheckBox->setObjectName(QStringLiteral("firstOperCheckBox"));
        firstOperCheckBox->setGeometry(QRect(220, 30, 131, 22));
        firstOperCheckBox->setChecked(true);
        okButton = new QPushButton(OperOrderForm);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(140, 240, 98, 27));
        secondOperCheckBox = new QCheckBox(OperOrderForm);
        secondOperCheckBox->setObjectName(QStringLiteral("secondOperCheckBox"));
        secondOperCheckBox->setGeometry(QRect(220, 80, 151, 22));
        secondOperCheckBox->setChecked(false);
        thirdOperCheckBox = new QCheckBox(OperOrderForm);
        thirdOperCheckBox->setObjectName(QStringLiteral("thirdOperCheckBox"));
        thirdOperCheckBox->setGeometry(QRect(220, 130, 131, 22));
        thirdOperCheckBox->setChecked(false);
        fourthOperCheckBox = new QCheckBox(OperOrderForm);
        fourthOperCheckBox->setObjectName(QStringLiteral("fourthOperCheckBox"));
        fourthOperCheckBox->setGeometry(QRect(220, 180, 151, 22));
        fourthOperCheckBox->setChecked(false);
        QWidget::setTabOrder(operationComboBox1, firstOperCheckBox);
        QWidget::setTabOrder(firstOperCheckBox, operationComboBox2);
        QWidget::setTabOrder(operationComboBox2, secondOperCheckBox);
        QWidget::setTabOrder(secondOperCheckBox, operationComboBox3);
        QWidget::setTabOrder(operationComboBox3, thirdOperCheckBox);
        QWidget::setTabOrder(thirdOperCheckBox, operationComboBox4);
        QWidget::setTabOrder(operationComboBox4, fourthOperCheckBox);
        QWidget::setTabOrder(fourthOperCheckBox, okButton);

        retranslateUi(OperOrderForm);
        QObject::connect(firstOperCheckBox, SIGNAL(clicked(bool)), operationComboBox1, SLOT(setEnabled(bool)));
        QObject::connect(secondOperCheckBox, SIGNAL(clicked(bool)), operationComboBox2, SLOT(setEnabled(bool)));
        QObject::connect(thirdOperCheckBox, SIGNAL(clicked(bool)), operationComboBox3, SLOT(setEnabled(bool)));
        QObject::connect(fourthOperCheckBox, SIGNAL(clicked(bool)), operationComboBox4, SLOT(setEnabled(bool)));

        operationComboBox1->setCurrentIndex(0);
        operationComboBox2->setCurrentIndex(0);
        operationComboBox3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(OperOrderForm);
    } // setupUi

    void retranslateUi(QDialog *OperOrderForm)
    {
        OperOrderForm->setWindowTitle(QApplication::translate("OperOrderForm", "Set operation order", 0));
        operationComboBox1->clear();
        operationComboBox1->insertItems(0, QStringList()
         << QApplication::translate("OperOrderForm", "Transform pixel", 0)
         << QApplication::translate("OperOrderForm", "Thresholding", 0)
         << QApplication::translate("OperOrderForm", "Equalize", 0)
         << QApplication::translate("OperOrderForm", "Gaussian Blur", 0)
         << QApplication::translate("OperOrderForm", "Median Blur", 0)
         << QApplication::translate("OperOrderForm", "Linear Filter", 0)
         << QApplication::translate("OperOrderForm", "Dilate", 0)
         << QApplication::translate("OperOrderForm", "Erode", 0)
        );
        operationComboBox2->clear();
        operationComboBox2->insertItems(0, QStringList()
         << QApplication::translate("OperOrderForm", "Transform pixel", 0)
         << QApplication::translate("OperOrderForm", "Thresholding", 0)
         << QApplication::translate("OperOrderForm", "Equalize", 0)
         << QApplication::translate("OperOrderForm", "Gaussian Blur", 0)
         << QApplication::translate("OperOrderForm", "Median Blur", 0)
         << QApplication::translate("OperOrderForm", "Linear Filter", 0)
         << QApplication::translate("OperOrderForm", "Dilate", 0)
         << QApplication::translate("OperOrderForm", "Erode", 0)
        );
        operationComboBox3->clear();
        operationComboBox3->insertItems(0, QStringList()
         << QApplication::translate("OperOrderForm", "Transform pixel", 0)
         << QApplication::translate("OperOrderForm", "Thresholding", 0)
         << QApplication::translate("OperOrderForm", "Equalize", 0)
         << QApplication::translate("OperOrderForm", "Gaussian Blur", 0)
         << QApplication::translate("OperOrderForm", "Median Blur", 0)
         << QApplication::translate("OperOrderForm", "Linear Filter", 0)
         << QApplication::translate("OperOrderForm", "Dilate", 0)
         << QApplication::translate("OperOrderForm", "Erode", 0)
        );
        operationComboBox4->clear();
        operationComboBox4->insertItems(0, QStringList()
         << QApplication::translate("OperOrderForm", "Transform pixel", 0)
         << QApplication::translate("OperOrderForm", "Thresholding", 0)
         << QApplication::translate("OperOrderForm", "Equalize", 0)
         << QApplication::translate("OperOrderForm", "Gaussian Blur", 0)
         << QApplication::translate("OperOrderForm", "Median Blur", 0)
         << QApplication::translate("OperOrderForm", "Linear Filter", 0)
         << QApplication::translate("OperOrderForm", "Dilate", 0)
         << QApplication::translate("OperOrderForm", "Erode", 0)
        );
        firstOperCheckBox->setText(QApplication::translate("OperOrderForm", "First operation", 0));
        okButton->setText(QApplication::translate("OperOrderForm", "OK", 0));
        secondOperCheckBox->setText(QApplication::translate("OperOrderForm", "Second operation", 0));
        thirdOperCheckBox->setText(QApplication::translate("OperOrderForm", "Third operation", 0));
        fourthOperCheckBox->setText(QApplication::translate("OperOrderForm", "Fourth operation", 0));
    } // retranslateUi

};

namespace Ui {
    class OperOrderForm: public Ui_OperOrderForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPERORDERFORM_H
