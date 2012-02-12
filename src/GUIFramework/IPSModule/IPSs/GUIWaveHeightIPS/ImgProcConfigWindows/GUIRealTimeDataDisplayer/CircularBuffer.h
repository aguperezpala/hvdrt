/*
 * CircularBuffer.h
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <qwt_series_data.h>
#include <qvector.h>

class CircularBuffer: public QwtSeriesData<QPointF>
{
public:
    CircularBuffer(size_t numPoints = 1000, double cantSeconds = 15.0);
    virtual ~CircularBuffer(){};

    virtual size_t size() const;
    virtual QPointF sample(size_t i) const;

    virtual QRectF boundingRect() const;

    int getNumPoints(void) const {return mValues.size();}

    double getCantSeconds(void) const {return mCantSeconds;}

    void addNewPoint(double x, double y);

    void clear(void);

private:
    double				mCantSeconds;
    int 				mIndex;
    QVector<QPointF> 	mValues;
    QRectF				mBoundingRect;
    size_t				mActualNumPoints;
};

#endif /* CIRCULARBUFFER_H_ */
