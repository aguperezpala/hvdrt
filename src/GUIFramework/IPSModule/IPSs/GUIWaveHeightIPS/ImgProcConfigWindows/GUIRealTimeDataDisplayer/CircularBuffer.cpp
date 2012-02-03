/*
 * CircularBuffer.cpp
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */


#include <cmath>

#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(size_t numPoints):
   mIndex(0),
   mActualNumPoints(0)
{
	// resize the vector
	mValues.resize(numPoints);
}

size_t CircularBuffer::size() const
{
    return mActualNumPoints;
}

QPointF CircularBuffer::sample(size_t i) const
{
    int index = mIndex + i;
    if(index > mActualNumPoints){
    	index -= mActualNumPoints;
    }
    return mValues[index];
}

QRectF CircularBuffer::boundingRect() const
{
    return mBoundingRect;
}

void CircularBuffer::addNewPoint(double x, double y)
{
	++mIndex;
	if(mIndex >= mValues.size()){
		mIndex = 0;
	}

	if(mActualNumPoints < mValues.size()){
		++mActualNumPoints;
	}

	mValues[mIndex].setX(x);
	mValues[mIndex].setY(y);

	// update the bounding rectangle
	if(x < mBoundingRect.left()) mBoundingRect.setLeft(x);
	else if (x > mBoundingRect.right()) mBoundingRect.setRight(x);

	if(y < mBoundingRect.bottom()) mBoundingRect.setBottom(y);
	else if(y > mBoundingRect.top()) mBoundingRect.setTop(y);
}

void CircularBuffer::clear(void)
{
	mIndex = 0;
	mActualNumPoints = 0;
}
