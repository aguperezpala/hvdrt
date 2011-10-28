#ifndef REALTIMEDATADISPLAYER_H
#define REALTIMEDATADISPLAYER_H

#include <QtGui/QWidget>
#include <qgraphicsscene.h>
#include <qpainter.h>

#include <vector>

#include "ui_realtimedatadisplayer.h"


class RealTimeDataDisplayer : public QWidget
{
    Q_OBJECT

public:
    RealTimeDataDisplayer(QWidget *parent = 0);
    ~RealTimeDataDisplayer();

    /* Sets the maximum possible value of the Y axis in seconds*/
    void setMaxYValue(float max);

    /* Sets the maximum possible value of the X axis in cm */
    void setMaxXValue(float max);

    /* Set scale (pixel/cm relation), so, how much value a pixel in cm */
    void setMeasureScale(float s);

    /* Sets the Time scale (pixel/seconds) */
    void setTimeScale(float s);

    /* Builds the axis and init the GUI
     * RETURNS:
     * 	true		on success
     *	false		on error
     */
    bool init(void);

    /* Adds a new point to draw.
     * Params:
     * 	@time		the time in seconds
     * 	@height		the height in cm
     */
    void addNewPoint(float time, float height);

private:
    /* Creates the both axis */
    void createAxis(void);

    /* Creates a point */
    void createPoint(float x, float y);

private:
    Ui::RealTimeDataDisplayerClass ui;
    QGraphicsScene					mScene;

    // Axis max values
    float							mXMaxValue;
    float							mYMaxValue;
    // Scale in cm
    float							mMeasureScale;
    float							mTimeScale;
    // the points
    std::vector<QGraphicsEllipseItem *> mPoints;

};

#endif // REALTIMEDATADISPLAYER_H
