/********************************************************************************
** Form generated from reading UI file 'zoneselector.ui'
**
** Created: Fri Oct 21 20:11:40 2011
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZONESELECTOR_H
#define UI_ZONESELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZoneSelectorClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLabel *x_label;
    QLabel *label_2;
    QLabel *y_label;

    void setupUi(QWidget *ZoneSelectorClass)
    {
        if (ZoneSelectorClass->objectName().isEmpty())
            ZoneSelectorClass->setObjectName(QString::fromUtf8("ZoneSelectorClass"));
        ZoneSelectorClass->resize(770, 685);
        verticalLayout = new QVBoxLayout(ZoneSelectorClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        pushButton = new QPushButton(ZoneSelectorClass);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(ZoneSelectorClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        label_3 = new QLabel(ZoneSelectorClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(label_3);

        x_label = new QLabel(ZoneSelectorClass);
        x_label->setObjectName(QString::fromUtf8("x_label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(x_label->sizePolicy().hasHeightForWidth());
        x_label->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(x_label);

        label_2 = new QLabel(ZoneSelectorClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label_2);

        y_label = new QLabel(ZoneSelectorClass);
        y_label->setObjectName(QString::fromUtf8("y_label"));
        sizePolicy1.setHeightForWidth(y_label->sizePolicy().hasHeightForWidth());
        y_label->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(y_label);


        horizontalLayout->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ZoneSelectorClass);

        QMetaObject::connectSlotsByName(ZoneSelectorClass);
    } // setupUi

    void retranslateUi(QWidget *ZoneSelectorClass)
    {
        ZoneSelectorClass->setWindowTitle(QApplication::translate("ZoneSelectorClass", "ZoneSelector", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("ZoneSelectorClass", "Clear Points", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("ZoneSelectorClass", "Done", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ZoneSelectorClass", "X:", 0, QApplication::UnicodeUTF8));
        x_label->setText(QApplication::translate("ZoneSelectorClass", "0", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ZoneSelectorClass", "Y:", 0, QApplication::UnicodeUTF8));
        y_label->setText(QApplication::translate("ZoneSelectorClass", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ZoneSelectorClass: public Ui_ZoneSelectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZONESELECTOR_H
