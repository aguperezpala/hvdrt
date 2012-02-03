/*
 * CircularBuffer.cpp
 *
 *  Created on: 02/02/2012
 *      Author: agustin
 *
 */


#include <math.h>

#include "CircularBuffer.h"
#include "DebugUtil.h"

CircularBuffer::CircularBuffer(size_t numPoints, double cantSeconds):
mCantSeconds(cantSeconds),
   mIndex(0),
   mActualNumPoints(0)
{
	// resize the vector
	mValues.resize(numPoints);
	mBoundingRect.setLeft(0.0);
	mBoundingRect.setRight(cantSeconds);
}

size_t CircularBuffer::size() const
{
    return mActualNumPoints;
}

QPointF CircularBuffer::sample(size_t i) const
{
//    int index = mIndex + i;
//    if(index > mValues.size()){
//    	index = index % mValues.size();
//    }
    return mValues[i];
}

QRectF CircularBuffer::boundingRect() const
{
    return mBoundingRect;
}

void CircularBuffer::addNewPoint(double x, double y)
{
	static double lastX = 99999.0;


	// check if we are overflowing the image
	const double realX = ::fmod(x,mCantSeconds);
	if(lastX > realX){
		// overflow, reinit the index
		mIndex = 0;
		mActualNumPoints = 0;
	} else {
		++mIndex;
		ASSERT(mIndex < mValues.size());
		++mActualNumPoints;
	}
	lastX = realX;

	mValues[mIndex].setX(realX);
	mValues[mIndex].setY(y);
//	debug("Adding new point: (%lf,%lf), index: %d, mActualNumPoints: %d\n",
//			::fmod(x,mCantSeconds),y, mIndex, mActualNumPoints);

	// update the bounding rectangle
//	if(x < mBoundingRect.left()) mBoundingRect.setLeft(x);
//	else if (x > mBoundingRect.right()) mBoundingRect.setRight(x);

	if(y < mBoundingRect.bottom()) mBoundingRect.setBottom(y);
	else if(y > mBoundingRect.top()) mBoundingRect.setTop(y);
}

void CircularBuffer::clear(void)
{
	mIndex = 0;
	mActualNumPoints = 0;
}
