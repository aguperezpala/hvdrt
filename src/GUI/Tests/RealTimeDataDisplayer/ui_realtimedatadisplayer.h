/********************************************************************************
** Form generated from reading UI file 'realtimedatadisplayer.ui'
**
** Created: Fri Oct 28 16:18:49 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REALTIMEDATADISPLAYER_H
#define UI_REALTIMEDATADISPLAYER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RealTimeDataDisplayerClass
{
public:
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *RealTimeDataDisplayerClass)
    {
        if (RealTimeDataDisplayerClass->objectName().isEmpty())
            RealTimeDataDisplayerClass->setObjectName(QString::fromUtf8("RealTimeDataDisplayerClass"));
        RealTimeDataDisplayerClass->resize(461, 379);
        horizontalLayout = new QHBoxLayout(RealTimeDataDisplayerClass);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(RealTimeDataDisplayerClass);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);


        retranslateUi(RealTimeDataDisplayerClass);

        QMetaObject::connectSlotsByName(RealTimeDataDisplayerClass);
    } // setupUi

    void retranslateUi(QWidget *RealTimeDataDisplayerClass)
    {
        RealTimeDataDisplayerClass->setWindowTitle(QApplication::translate("RealTimeDataDisplayerClass", "RealTimeDataDisplayer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RealTimeDataDisplayerClass: public Ui_RealTimeDataDisplayerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REALTIMEDATADISPLAYER_H
