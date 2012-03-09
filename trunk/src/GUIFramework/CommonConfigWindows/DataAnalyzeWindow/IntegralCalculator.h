/*
 * IntegralCalculator.h
 *
 *  Created on: 08/03/2012
 *      Author: agustin
 *
 */

#ifndef INTEGRALCALCULATOR_H_
#define INTEGRALCALCULATOR_H_

#include <qvector.h>

class FunctionFunctr {
public:
	virtual double operator()(double d) const = 0;
};

class IntegralCalculator {
public:

	// Approximates the area under the curve of function pointed to by ys
	// NOTE: we assume that the ys has a size
	// (which must take and return a float) in the range x=start to x=end.
	// Accuracy is determined by number of steps used between these values.
	//
	static double ApproximateIntegral(const FunctionFunctr &func, double start,
			double end, unsigned steps);


private:

	// Returns the area of a trapezoid of a given height where sideA
	// and sideB are the uneven sides
	static double CalculateTrapezoidArea(double sideA, double sideB, double height);


};

#endif /* INTEGRALCALCULATOR_H_ */
