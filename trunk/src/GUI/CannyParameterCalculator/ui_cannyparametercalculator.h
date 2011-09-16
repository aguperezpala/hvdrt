/********************************************************************************
** Form generated from reading UI file 'cannyparametercalculator.ui'
**
** Created: Fri Sep 16 16:17:40 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANNYPARAMETERCALCULATOR_H
#define UI_CANNYPARAMETERCALCULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CannyParameterCalculatorClass
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *image_label;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QCheckBox *l2Gradient_checkBox;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSlider *threshole1_slider;
    QLineEdit *threshole1_text;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QSlider *threshole2_slider;
    QLineEdit *threshole2_text;
    QPushButton *cannyBuild_button;
    QPushButton *loadImg_button;

    void setupUi(QWidget *CannyParameterCalculatorClass)
    {
        if (CannyParameterCalculatorClass->objectName().isEmpty())
            CannyParameterCalculatorClass->setObjectName(QString::fromUtf8("CannyParameterCalculatorClass"));
        CannyParameterCalculatorClass->setWindowModality(Qt::WindowModal);
        CannyParameterCalculatorClass->resize(835, 799);
        verticalLayout_2 = new QVBoxLayout(CannyParameterCalculatorClass);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        image_label = new QLabel(CannyParameterCalculatorClass);
        image_label->setObjectName(QString::fromUtf8("image_label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(image_label->sizePolicy().hasHeightForWidth());
        image_label->setSizePolicy(sizePolicy);
        image_label->setMinimumSize(QSize(800, 600));

        verticalLayout_2->addWidget(image_label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        l2Gradient_checkBox = new QCheckBox(CannyParameterCalculatorClass);
        l2Gradient_checkBox->setObjectName(QString::fromUtf8("l2Gradient_checkBox"));

        formLayout->setWidget(1, QFormLayout::LabelRole, l2Gradient_checkBox);

        label = new QLabel(CannyParameterCalculatorClass);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        threshole1_slider = new QSlider(CannyParameterCalculatorClass);
        threshole1_slider->setObjectName(QString::fromUtf8("threshole1_slider"));
        threshole1_slider->setMaximum(400);
        threshole1_slider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(threshole1_slider);

        threshole1_text = new QLineEdit(CannyParameterCalculatorClass);
        threshole1_text->setObjectName(QString::fromUtf8("threshole1_text"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(threshole1_text->sizePolicy().hasHeightForWidth());
        threshole1_text->setSizePolicy(sizePolicy1);
        threshole1_text->setMinimumSize(QSize(112, 0));
        threshole1_text->setAcceptDrops(false);
        threshole1_text->setReadOnly(true);

        horizontalLayout->addWidget(threshole1_text);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout);

        label_2 = new QLabel(CannyParameterCalculatorClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        threshole2_slider = new QSlider(CannyParameterCalculatorClass);
        threshole2_slider->setObjectName(QString::fromUtf8("threshole2_slider"));
        threshole2_slider->setMaximum(400);
        threshole2_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(threshole2_slider);

        threshole2_text = new QLineEdit(CannyParameterCalculatorClass);
        threshole2_text->setObjectName(QString::fromUtf8("threshole2_text"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(threshole2_text->sizePolicy().hasHeightForWidth());
        threshole2_text->setSizePolicy(sizePolicy2);
        threshole2_text->setMinimumSize(QSize(50, 0));
        threshole2_text->setReadOnly(true);

        horizontalLayout_2->addWidget(threshole2_text);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout_2);


        verticalLayout->addLayout(formLayout);

        cannyBuild_button = new QPushButton(CannyParameterCalculatorClass);
        cannyBuild_button->setObjectName(QString::fromUtf8("cannyBuild_button"));

        verticalLayout->addWidget(cannyBuild_button);

        loadImg_button = new QPushButton(CannyParameterCalculatorClass);
        loadImg_button->setObjectName(QString::fromUtf8("loadImg_button"));

        verticalLayout->addWidget(loadImg_button);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(CannyParameterCalculatorClass);

        QMetaObject::connectSlotsByName(CannyParameterCalculatorClass);
    } // setupUi

    void retranslateUi(QWidget *CannyParameterCalculatorClass)
    {
        CannyParameterCalculatorClass->setWindowTitle(QApplication::translate("CannyParameterCalculatorClass", "CannyParameterCalculator", 0, QApplication::UnicodeUTF8));
        image_label->setText(QString());
        l2Gradient_checkBox->setText(QApplication::translate("CannyParameterCalculatorClass", "L2Gradiente", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CannyParameterCalculatorClass", "Threshole1 (L)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CannyParameterCalculatorClass", "Threshole2 (H)", 0, QApplication::UnicodeUTF8));
        cannyBuild_button->setText(QApplication::translate("CannyParameterCalculatorClass", "Canny", 0, QApplication::UnicodeUTF8));
        loadImg_button->setText(QApplication::translate("CannyParameterCalculatorClass", "Load Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CannyParameterCalculatorClass: public Ui_CannyParameterCalculatorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANNYPARAMETERCALCULATOR_H
