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

#include "DataProcessor.h"



////////////////////////////////////////////////////////////////////////////////
DataProcessor::DataProcessor() {
	// TODO Auto-generated constructor stub

}

////////////////////////////////////////////////////////////////////////////////
DataProcessor::~DataProcessor()
{
	mOutFile.close();
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
}

////////////////////////////////////////////////////////////////////////////////
void DataProcessor::operator()(void)
{
	ASSERT(data);


}
