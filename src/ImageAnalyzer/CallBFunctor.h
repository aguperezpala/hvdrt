/*
 * CallBFunctor.h
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

#ifndef CALLBFUNCTOR_H_
#define CALLBFUNCTOR_H_

typedef struct {
	double time;
	double height;
	double pixelPos;
} AnalyzedData;

class CallBFunctor {
public:
	CallBFunctor(){};
	virtual ~CallBFunctor(){};

	// function called with the analyzed data
	virtual void operator()(const AnalyzedData&) = 0;
};

#endif /* CALLBFUNCTOR_H_ */
