#ifndef MOUSEQLABEL_H
#define MOUSEQLABEL_H

#include <QtGui/QWidget>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qlabel.h>
#include <qwidget.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>


class MouseQLabel : public QLabel
{
    Q_OBJECT

public:
    MouseQLabel(QWidget *parent = 0);
    ~MouseQLabel();

    // sets the labels where it will show the positions
    void setLabelY(QLabel *x) {mLabelX = x;}
    void setLabelX(QLabel *y) {mLabelY = y;}

    inline int getX(void) const {return mX;}
    inline int getY(void) const {return mX;}

protected:
    void mousePressEvent(QMouseEvent* event);

private:


private:
    int			mX;
    int			mY;
    QLabel		*mLabelX;
    QLabel		*mLabelY;

};

#endif // MOUSEQLABEL_H
