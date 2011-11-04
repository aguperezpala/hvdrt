/*
 * MainProgram.h
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

#ifndef MAINPROGRAM_H_
#define MAINPROGRAM_H_


#include <QtGui/QWidget>

#include <list>
#include <vector>
#include <algorithm>
#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GUIIPSFactory.h"
#include "guiimageprocessingsystem.h"



class MainProgram {
public:
	MainProgram();
	virtual ~MainProgram();

	/* Load all the Image processing systems.
	 * Params:
	 * 	@factory		The IPSFactory used to load all the ImageProcessingSystems
	 * Returns errCode
	 * TODO: load all the processing system using plugins
	 */
	errCode loadProcessingSystems(const IPSFactory *factory);

	/* Returns all the processing systems */
	void getProcessingSystems(std::list<const GUIImageProcessingSystem *> &ips);

	/* Execute one of the processing systems loaded. This is a blocking function
	 * Returns once the IPS had finish and returns the corresponding error code
	 * Params:
	 * 	@ipsName		The IPS name to be executed, it must be loaded before
	 */
	errCode execute(const std::string &ipsName);

private:
	std::vector<std::auto_ptr<GUIImageProcessingSystem> >	mIPS;

};

#endif /* MAINPROGRAM_H_ */
