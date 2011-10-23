#ifndef MOUSEQLABEL_H
#define MOUSEQLABEL_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlabel.h>
#include <qwidget.h>
#include <qpainter.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>


class MouseQLabel : public QLabel
{
    Q_OBJECT

    static const int REMARK_POINT_PIXELS	=	4;
    static const int REMARK_POINT_COLOR		=	255;
public:
    MouseQLabel(QWidget *parent = 0);
    ~MouseQLabel();

    // sets the labels where it will show the positions
    void setLabelY(QLabel *x) {mLabelX = x;}
    void setLabelX(QLabel *y) {mLabelY = y;}

    inline int getX(void) const {return mX;}
    inline int getY(void) const {return mX;}

    /* Sets the image to work with */
    void setImage(const QImage &img);


protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

private:
    /* fills the label with the img */
    void showImage(const QImage &img);

    /* Remarks the point */
    void remarkPoint(QImage &img, int x, int y);

    /* Remark a rectangle */
    void remarkRectangle(QImage &img, int topLeftX, int topLeftY, int bottomRX, int bottomRY);


private:
    int			mX;
    int			mY;
    QLabel		*mLabelX;
    QLabel		*mLabelY;

    QImage		mImg;
};

#endif // MOUSEQLABEL_H
