/*
 * IPSFactory.cpp
 *
 *  Created on: 12/02/2012
 *      Author: agustin
 *
 */

#include "guiwaveheightips.h"
#include "IPSFactory.h"

IPSFactory::IPSFactory(int winW, int winH) :
mWinW(winW),
mWinH(winH)
{
	// TODO Auto-generated constructor stub

}

IPSFactory::~IPSFactory() {
	// TODO Auto-generated destructor stub
}

void IPSFactory::getIPS(GUIIPSVec &result)
{
	result.clear();

	// by now we only return the WaveHeightIPS
	GUIImageProcessingSystem *ips(new GUIWaveHeightIPS(0, mWinW, mWinH));
	result.push_back(ips);

	return;
}
