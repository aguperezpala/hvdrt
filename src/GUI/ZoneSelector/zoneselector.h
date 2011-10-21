#ifndef ZONESELECTOR_H
#define ZONESELECTOR_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

#include "ui_zoneselector.h"
#include "MouseQLabel.h"

class ZoneSelector : public QWidget
{
    Q_OBJECT

public:
    ZoneSelector(QWidget *parent = 0);
    ~ZoneSelector();

    /* Sets the image where the user have to select the points */
    void setImage(const cv::Mat &img);

    /* Set the vector where it will be pushed the points selected in the image */
    void setPointsVector(std::vector<cv::Point2i> *vec);


private:
    int showMessage(const QString &m);
    bool IplImage2QImage(const cv::Mat &iplImgMat, QImage &ref);

private:
    Ui::ZoneSelectorClass 		ui;
    // The image to analyze
    cv::Mat						mImage;
    // Vector of points
    std::vector<cv::Point2i> 	*mPointsVector;
    MouseQLabel					*mMouseLabel;


};

#endif // ZONESELECTOR_H
