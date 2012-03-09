/*
 * IntegralCalculator.cpp
 *
 *  Created on: 08/03/2012
 *      Author: agustin
 *
 */

#include "IntegralCalculator.h"


// Returns the area of a trapezoid of a given height where sideA
// and sideB are the uneven sides
double IntegralCalculator::CalculateTrapezoidArea(double sideA, double sideB, double height)
{
	// Area of a trapezoid: A = (a+b)h / 2 (Wikipedia)
	return ( ( (sideA+sideB) * height  ) / 2.0 );
}

// Approximates the area under the curve of function pointed to by pFunc
// (which must take and return a float) in the range x=start to x=end.
// Accuracy is determined by number of steps used between these values.
double IntegralCalculator::ApproximateIntegral(const FunctionFunctr &func,
		double start, double end, unsigned steps)
{
	// pFunc is a pointer to a function that takes and returns a float, which we will use as f(x).
	// The general method is to calculate points at step intervals and calculate area of
	// the trapezoid underneath then add areas together

	int   i;	// counter
	double diff; // the difference between steps - used as trapezoid height later
	double* yValues = new double[steps + 2]; // for calculated results

	// set start and end x values

	// Interpolate x values for number of steps
	// Loop from second element to penultimate element
	diff = (end - start) / (static_cast<double>(steps + 1));
	double accumDiff = start;
	// now we have all the x values, calculate all corresponding y values (or f(x))
	for (i = 0; i < steps + 2; ++i)
	{
		accumDiff = accumDiff + diff;
		yValues[i] = func(accumDiff);
	}

	// now calculate the area under each trapezoid
	// a will be first y of pair, b will be second y, h will be diff from earlier
	double finalArea = 0.0;

	// we always need to do n-1 traps where n is the number of points we have
	for (i = 0; i < steps + 1; ++i)
	{
		finalArea += CalculateTrapezoidArea(yValues[i], yValues[i+1], diff);
	}

	delete[] yValues;

	return finalArea;
}
