#ifndef MOUSEQLABEL_H
#define MOUSEQLABEL_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qevent.h>
#include <qpoint.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>


class MouseQLabel : public QLabel
{
    Q_OBJECT

    static const int REMARK_POINT_PIXELS	=	4;

    static const int ZOOM_IMAGE_SIZE		=	100;
    static const int ZOOM_FACTOR			=	250;
public:
    MouseQLabel(QWidget *parent = 0);
    ~MouseQLabel();

    // sets the labels where it will show the positions
    void setLabelX(QLabel *x) {mLabelX = x;}
    void setLabelY(QLabel *y) {mLabelY = y;}

    inline int getX(void) const {return mX;}
    inline int getY(void) const {return mX;}

    /* Sets the image to work with */
    void setImage(const QImage &img);

    /* Set the Label where it will be put the zoomed image */
    void setZoomLabel(QLabel *zoom);

    /* clears the points */
    void clearPoints(void);

    /* Returns selected points */
    const std::vector<QPoint> &getPoints(void) const {return mPoints;}

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    /* fills the label with the img */
    void showImage(const QImage &img);

    /* Remarks the point */
    void remarkPoint(QImage &img, int x, int y);
    void remarkListPoints(int num, QImage &img);

    /* Remark a rectangle */
    void remarkRectangle(QImage &img, int topLeftX, int topLeftY, int bottomRX, int bottomRY);


private:
    int							mX;
    int							mY;
    QLabel						*mLabelX;
    QLabel						*mLabelY;
    QLabel						*mZoomLabel;
    QImage						mImg;
    QImage						mOrigImg;
    // the list of points
    std::vector<QPoint>			mPoints;
    QRgb						mPaintColor;
};

#endif // MOUSEQLABEL_H
