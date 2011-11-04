/*
 * FrameListener.h
 *
 *  Created on: 12/09/2011
 *      Author: agustin
 */

#ifndef FRAMELISTENER_H_
#define FRAMELISTENER_H_

#include "GlobalDefines.h"
#include "Frame.h"

class FrameEmitter;

class FrameListener {
public:
	FrameListener(){};
	virtual ~FrameListener(){};

	/* Process a frame (blocking function) */
	virtual errCode processFrame(Frame &frame) = 0;

};

#endif /* FRAMELISTENER_H_ */
