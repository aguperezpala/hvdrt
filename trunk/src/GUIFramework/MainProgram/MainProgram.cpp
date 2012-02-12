/*
 * MainProgram.cpp
 *
 *  Created on: 04/11/2011
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

#include "MainProgram.h"


// remove all the actual mips
void MainProgram::removeIPS(void)
{
	for(int i = mIPS.size() - 1; i >= 0; --i){
		delete mIPS[i];
	}
}

MainProgram::MainProgram()
{


}

MainProgram::~MainProgram()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode MainProgram::loadProcessingSystems(IPSFactory *factory)
{
	ASSERT(factory);
	factory->getIPS(mIPS);

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void MainProgram::getProcessingSystems(std::list<const GUIImageProcessingSystem *> &ips)
{
	ips.clear();
	for(int i = mIPS.size() - 1; i >= 0; --i){
		ips.push_back(mIPS[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
errCode MainProgram::execute(const std::string &ipsName)
{
	GUIImageProcessingSystem *system = 0;

	for(int i = mIPS.size() - 1; i >= 0; --i){
		if(mIPS[i]->getName() == ipsName){
			system = mIPS[i];
		}
	}

	if(system == 0){
		debug("Error: No Image Processing System with name %s\n", ipsName.c_str());
		return INVALID_PARAM;
	}

	// execute the system and return the errcode
	errCode err = system->initialize();

	if(err != NO_ERROR){
		return err;
	}

//	while(err == NO_ERROR){
	// TODO: arreglar esto, para que no se cierre la ventana principal a no ser
	// que nosotros querramos.
		err = system->execute();
//	}

	return err;
}
