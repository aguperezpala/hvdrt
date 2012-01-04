#ifndef FRAMELABELDISPLAYER_H
#define FRAMELABELDISPLAYER_H

#include <QtGui/QWidget>
#include <qlabel.h>

#include <opencv2/core/core.hpp>

#include "ui_framelabeldisplayer.h"



class FrameLabelDisplayer : public QLabel
{
    Q_OBJECT

public:
    FrameLabelDisplayer(QWidget *parent = 0);
    ~FrameLabelDisplayer();

    /* Set the image to be shown */
    void setImage(const cv::Mat &img);

private:
    Ui::FrameLabelDisplayerClass ui;

    QImage						mShowImage;
};

#endif // FRAMELABELDISPLAYER_H
