/*
 *  confidence_intervals.h
 *  integrate
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CONFIDENCE_INTERVALS
#define CONFIDENCE_INTERVALS

namespace stat
{
	template<typename T>
	T f(const T& x)
	{
		return -x*x/2;
	}
	
	// N(0.0, 1.0)
	template<typename T>
	const T gaussPDF(const T& x)
	{
		return num::exponent(f<T>, x)/sqrt(2.0*3.1415926);
	}
	
	template<typename C, typename T>
	// requires Integer(C) & Number(T)
	C twoPointConfidence1(const T& interval, const T& alpha)
	{
		T b = alpha*interval*interval;
		return 1.0/(4.0*b); // truncating
	}
	
	template<typename T>
	T twoPointGaussResult(const T& alpha)
	{
		return (1.0-alpha)/2.0;
	}
	
	template<typename C, typename T>
	// requires Integer(C) & Number(T)
	C uniformConfidence1(const T& interval, const T& alpha)
	{
		return 1.0/(12.0*alpha*interval*interval); // truncating
	}
	
	template<typename T>
	T uniformGaussResult(const T& alpha)
	{
		return 1.0-alpha/2.0;
	}
	
	// search the interval for the integral upper limit
	//	corresponding to the passed in alpha
	template<typename UnaryFunction, typename T>
	T invertedGaussSum(const UnaryFunction& f, const T& alpha, const T& tol = 0.0001)
	{
		T low = 0.0;
		T high = 50.0;
		T resultNeeded = f(alpha); // this is the final result we want the parameter for.
		T value = 25.0;
		T increment = (high-value)/2.0;
		T result = num::integrateSimpsons(gaussPDF<double>, low, value);
		while (fabs(result - resultNeeded) > tol && value < high && value > low)
		{ // binary search since this is monotone
			if (result < resultNeeded) value = value + increment;
			else value  = value - increment;
			result = num::integrateSimpsons(gaussPDF<double>, low, value);
			increment /= 2.0;
		}
		
		return value;
	}
	template<typename C, typename T>
	// requires Integer(C) & Number(T)
	C twoPointConfidence2(const T& interval, const T& alpha)
	{
		T b = stat::invertedGaussSum(twoPointGaussResult<T>, alpha);
		
		return b*b/(16.0*interval*interval); // truncating
	}
	
	template<typename C, typename T>
	// requires Integer(C) & Number(T)
	C uniformConfidence2(const T& interval, const T& alpha)
	{
		T b = stat::invertedGaussSum(twoPointGaussResult<T>, alpha)/interval;
		return b*b/12.0; // truncating
	}
};

#endif // CONFIDENCE_INTERVALS
