/*
 * Frame.h
 *
 *  Created on: 12/09/2011
 *      Author: agustin
 */

#ifndef FRAME_H_
#define FRAME_H_



#include <opencv2/core/core.hpp>
#include "Timestamp.h"

struct Frame {
	cv::Mat			data;
	Timestamp		timestamp;
};

#endif /* FRAME_H_ */
