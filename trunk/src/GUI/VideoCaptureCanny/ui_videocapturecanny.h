/********************************************************************************
** Form generated from reading UI file 'videocapturecanny.ui'
**
** Created: Fri Oct 21 18:14:11 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOCAPTURECANNY_H
#define UI_VIDEOCAPTURECANNY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoCaptureCannyClass
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *loadVideo_button;
    QPushButton *camera_button;
    QHBoxLayout *horizontalLayout;
    QPushButton *canny_button;
    QLineEdit *cannyA_text;
    QLineEdit *cannyB_text;

    void setupUi(QWidget *VideoCaptureCannyClass)
    {
        if (VideoCaptureCannyClass->objectName().isEmpty())
            VideoCaptureCannyClass->setObjectName(QString::fromUtf8("VideoCaptureCannyClass"));
        VideoCaptureCannyClass->resize(438, 113);
        verticalLayout = new QVBoxLayout(VideoCaptureCannyClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        loadVideo_button = new QPushButton(VideoCaptureCannyClass);
        loadVideo_button->setObjectName(QString::fromUtf8("loadVideo_button"));

        verticalLayout->addWidget(loadVideo_button);

        camera_button = new QPushButton(VideoCaptureCannyClass);
        camera_button->setObjectName(QString::fromUtf8("camera_button"));

        verticalLayout->addWidget(camera_button);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        canny_button = new QPushButton(VideoCaptureCannyClass);
        canny_button->setObjectName(QString::fromUtf8("canny_button"));

        horizontalLayout->addWidget(canny_button);

        cannyA_text = new QLineEdit(VideoCaptureCannyClass);
        cannyA_text->setObjectName(QString::fromUtf8("cannyA_text"));

        horizontalLayout->addWidget(cannyA_text);

        cannyB_text = new QLineEdit(VideoCaptureCannyClass);
        cannyB_text->setObjectName(QString::fromUtf8("cannyB_text"));

        horizontalLayout->addWidget(cannyB_text);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(VideoCaptureCannyClass);

        QMetaObject::connectSlotsByName(VideoCaptureCannyClass);
    } // setupUi

    void retranslateUi(QWidget *VideoCaptureCannyClass)
    {
        VideoCaptureCannyClass->setWindowTitle(QApplication::translate("VideoCaptureCannyClass", "VideoCaptureCanny", 0, QApplication::UnicodeUTF8));
        loadVideo_button->setText(QApplication::translate("VideoCaptureCannyClass", "Load Video", 0, QApplication::UnicodeUTF8));
        camera_button->setText(QApplication::translate("VideoCaptureCannyClass", "Camera Capture", 0, QApplication::UnicodeUTF8));
        canny_button->setText(QApplication::translate("VideoCaptureCannyClass", "Canny", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VideoCaptureCannyClass: public Ui_VideoCaptureCannyClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOCAPTURECANNY_H
