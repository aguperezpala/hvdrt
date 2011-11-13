/*
 * DataProcessor.h
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

#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include <string>
#include <fstream>

#include "DebugUtil.h"
#include "CallBFunctor.h"
#include "CoordsInterpreter.h"
#include "Timestamp.h"

class DataProcessor : public CallBFunctor {
public:
	DataProcessor();
	virtual ~DataProcessor();

	/* Set the file name where we will put the data */
	void setFileName(const std::string &fname);

	/* Set the relation between pixels and millimeters.
	 * Requires:
	 * 	@rel		Relation pixels/mm
	 */
	void setRelation(float rel) {mRelation = rel;}

	/* Set the data to analyze */
	void setData(CoordsInterpreter::Data *data);

	virtual void operator()(void);


private:
	CoordsInterpreter::Data			*mData;
	std::ofstream					mOutFile;
	float							mRelation;
};

#endif /* DATAPROCESSOR_H_ */
