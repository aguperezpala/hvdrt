/*
 * DataProcessor.cpp
 *
 *  Created on: 13/11/2011
 *      Author: agustin
 *
 *
 * OpenVC License
 *
 * Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
 * Copyright (C) 2009, Willow Garage Inc., all rights reserved.
 * Third party copyrights are property of their respective owners.
 *
 * This software is provided by the copyright holders and contributors "as is" and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall the Intel Corporation or contributors be liable for any direct,
 * indirect, incidental, special, exemplary, or consequential damages
 * (including, but not limited to, procurement of substitute goods or services;
 * loss of use, data, or profits; or business interruption) however caused
 * and on any theory of liability, whether in contract, strict liability,
 * or tort (including negligence or otherwise) arising in any way out of
 * the use of this software, even if advised of the possibility of such damage.
 */

#include <cmath>

#include "DataProcessor.h"



////////////////////////////////////////////////////////////////////////////////
float DataProcessor::getNeighborsHeight(void)
{
	int neighborHeight = 0;
	// we have not detected the new size, try getting the average from the
	// pixels of the sides
	std::vector<int> &l = mData->at(mMiddlePoint-1);
	std::vector<int> &r = mData->at(mMiddlePoint+1);
	int lh = 0;
	int rh = 0;
	if(l.size() != 0) {
		// no left pixel
		lh = l[l.size()-1];
	}
	if(r.size() != 0){
		rh = r[r.size()-1];
	}

	// get the neighbor height
	if(rh != 0 && lh != 0){
		neighborHeight = (rh+lh)/2;
	} else if (rh != 0){
		neighborHeight = rh;
	} else if (lh != 0){
		neighborHeight = lh;
	}

	// now check if the neighborHeight is possible
	float mmh = neighborHeight / mRelation;
	if(mLastHeight > 0.0f){
		if(std::abs(mmh-mLastHeight) < MAX_HEIGHT_VARIANCE){
			// we have a correct value, return it
			return mmh;
		} else {
			// we cannot detect the value, return the last height
			debug("Warning: Coludn't detect the height, returning the last"
					" calculated Height\n");
			return mLastHeight;
		}
	} else {
		// we can see if we have some "normal value"
		if(mmh == 0){
			debug("Warning: Problem detecting the height\n");
			return mLastHeight;
		} else {
			return mmh;
		}
	}
}

/* Function used to get the height of the wave using the middlePoint and
 * middlepoint-1 & middlePoint+1 positions. We will get the first value from
 * bottom to top and check if is a possible value (using the last value
 * and the left and right values)
 * Returns:
 * 	height			The height of the wave.
 */
////////////////////////////////////////////////////////////////////////////////
float DataProcessor::getWaveHeight(void)
{
	ASSERT(mMiddlePoint > 0);

	//TODO: Aca no estamos trabajando con mas de un valor, lo que deberiamos
	// hacer es tener en cuenta el valor anterior y hacia donde estaba yendo la ola
	// (vector de direccion) para poder estimar aproximadamente la nueva posicion
	// y de esta forma obtener el valor que se encuentra lo mas proximo a
	// lastPosition + moveDirectionVector (simplemente con std::abs() con cada
	// valor para obtener el mas cercano).
	//


	// now get the first point from bottom to top
	std::vector<int> &p = mData->at(mMiddlePoint);
	if(p.size() == 0){
		return getNeighborsHeight();
	}
	// else we have some value, try to get the height
	int h = p[p.size()-1];
	if(mLastHeight > 0){
		float mmh = h / mRelation;// TODO: save 1/Relation and multiply here (is faster)
		if(std::abs(h - mLastHeight) < MAX_HEIGHT_VARIANCE){
			// we have a good value
			return mmh;
		} else {
			// to much variance, try to approximate using the neighbors
			return getNeighborsHeight();
		}
	} else {
		float mmh = h / mRelation;
		if(mmh == 0.0f){
			return getNeighborsHeight();
		} else {
			return mmh;
		}
	}

	ASSERT(false);
}

////////////////////////////////////////////////////////////////////////////////
DataProcessor::DataProcessor() :
mTimeStamp(0),
mMiddlePoint(-1),
mLastHeight(-1.0f)
{
	// TODO Auto-generated constructor stub

}

////////////////////////////////////////////////////////////////////////////////
DataProcessor::~DataProcessor()
{
	mOutFile.close();
	delete mTimeStamp;
}


////////////////////////////////////////////////////////////////////////////////
void  DataProcessor::setFileName(const std::string &fname)
{
	if(mOutFile.is_open()){
		mOutFile.close();
	}
	mOutFile.open(fname.c_str(), std::ios::out);

	if(!mOutFile.is_open()){
		debug("Error while opening the output file %s\n", fname.c_str());
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////
void DataProcessor::setData(CoordsInterpreter::Data *data)
{
	ASSERT(data);

	if(mData){
		debug("Warning: Previous data was set\n");
	}

	mData = data;

	if(mData->size() < 3){
		debug("Error: The actual method need at almost 3 points to work correctly\n");
		ASSERT(false);
		return;
	}

	// set the point that we will analyze
	mMiddlePoint = mData->size()/2;
	ASSERT(mMiddlePoint > 0);
}

////////////////////////////////////////////////////////////////////////////////
void DataProcessor::operator()(void)
{
	ASSERT(mData);

	if(!mTimeStamp){
		// we have to create the timestamp
		mTimeStamp = new Timestamp();
	}

	// now process the data
	mLastHeight = getWaveHeight();

	// now transform the high using the middle point
	mRealLastHeight = mLastHeight - (mPoint.y / mRelation);

	// print the data into the file
	mLastTime = mTimeStamp->getDiffTimestamp();
	mOutFile << mLastTime << "\t\t" << mRealLastHeight <<  std::endl;


}
