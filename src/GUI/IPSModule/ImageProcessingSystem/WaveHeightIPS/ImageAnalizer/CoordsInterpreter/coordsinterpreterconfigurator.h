#ifndef COORDSINTERPRETERCONFIGURATOR_H
#define COORDSINTERPRETERCONFIGURATOR_H

#include <QtGui/QWidget>
#include <qdialog.h>
#include <qscrollarea.h>

#include <opencv2/core/core.hpp>

#include "ui_coordsinterpreterconfigurator.h"
#include "MouseQLabel.h"

#include "GUIConfiguratorDialog.h"

class CoordsInterpreterConfigurator : public GUIConfiguratorDialog
{
    Q_OBJECT

public:
    CoordsInterpreterConfigurator(QWidget *parent = 0);
    ~CoordsInterpreterConfigurator();

    // Set the image where we want to get the center point
    void setImage(const cv::Mat &img);

    // Returns the point selected if it was selected
    const cv::Point &getPoint(void) const {return mPoint;}

public slots:
    void onClearClicked(void);
    void onDoneClicked(void);

private:
    Ui::CoordsInterpreterConfiguratorClass ui;
    MouseQLabel					*mMouseLabel;
    QScrollArea					mScrollArea;
    cv::Point					mPoint;

    cv::Mat						mImage;

};

#endif // COORDSINTERPRETERCONFIGURATOR_H
