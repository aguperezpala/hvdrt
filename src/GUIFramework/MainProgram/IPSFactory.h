/*
 * IPSFactory.h
 *
 *  Created on: 12/02/2012
 *      Author: agustin
 *
 */

#ifndef IPSFACTORY_H_
#define IPSFACTORY_H_

#include <auto_ptr.h>
#include <vector>

#include "guiimageprocessingsystem.h"

class IPSFactory {
public:
	typedef std::vector<GUIImageProcessingSystem*> GUIIPSVec;
public:
	IPSFactory(int winW, int winH);
	~IPSFactory();

	/* Get all the GUIImageProcessingSystem that we found or we have set to
	 * return
	 */
	void getIPS(GUIIPSVec &result);


private:
	int		mWinH;
	int		mWinW;
};

#endif /* IPSFACTORY_H_ */
