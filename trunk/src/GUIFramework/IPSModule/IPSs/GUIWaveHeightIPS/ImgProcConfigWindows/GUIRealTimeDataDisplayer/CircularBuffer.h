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
    CircularBuffer(size_t numPoints = 1000);

    virtual size_t size() const;
    virtual QPointF sample(size_t i) const;

    virtual QRectF boundingRect() const;

    void addNewPoint(double x, double y);

    void clear(void);

private:
    int 				mIndex;
    QVector<QPointF> 	mValues;
    QRectF				mBoundingRect;
    size_t				mActualNumPoints;
};

#endif /* CIRCULARBUFFER_H_ */
