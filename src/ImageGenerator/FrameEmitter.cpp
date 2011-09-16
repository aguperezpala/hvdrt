/*
 * FrameEmitter.cpp
 *
 *  Created on: 16/09/2011
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

#include <algorithm>

#include "FrameEmitter.h"

FrameEmitter::FrameEmitter()
{

}

FrameEmitter::~FrameEmitter()
{

}

/* Add a new FrameListener */
bool FrameEmitter::addNewListener(FrameListener *fl)
{
	ASSERT(fl);
	std::list<FrameListener *>::iterator it = std::find(mListeners.begin(), mListeners.end(), fl);
	if(it != mListeners.end()){
		return false;
	}

	mListeners.push_back(fl);

	return true;
}

/* Removes a frame listener */
bool FrameEmitter::removeFrameListener(FrameListener *fl)
{
	ASSERT(fl);
	std::list<FrameListener *>::iterator it = std::find(mListeners.begin(), mListeners.end(), fl);
	if(it == mListeners.end()){
		return false;
	}

	mListeners.erase(it);

	return true;
}


void FrameEmitter::emmitFrame(Frame &frame)
{
	// TODO: Here we have to see if we will create a clone of the frame for
	// every FrameListener to avoid problems but lost performance :(

	for(std::list<FrameListener *>::iterator it = mListeners.begin(); it != mListeners.end();
			++it) {
		(*it)->processFrame(frame);
	}
}


