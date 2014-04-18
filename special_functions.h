/*
 *  special_functions.h
 *  integrate
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SPECIAL_FUNCTIONS
#define SPECIAL_FUNCTIONS

#include <functional>
#include <iostream>
#include <cmath>
#include <cassert>

#include "timing.h"

namespace num
{
	template<typename UnaryFunction, typename T>
	T exponent(const UnaryFunction& f, const T& x)
	{
		return exp(f(x));
	}
	
	
	namespace epic_fail
	{
		// gamma Function stuff.  Lots of cruft to curry the gamma Function 
		//	to look like a unary function for integration.
		template<typename T>
		T gammaBaseFunction(const T& y, const T& c)
		{
			return pow(y, c - 1.0)*exp(-y);
		}
	
		template<typename T>
		struct GammaBaseFunctionStruct : public std::binary_function<T, T, T>
		{
			T operator()(const T& y, const T& c) const
			{ return gammaBaseFunction<T>(y, c); }
		};
	
		// this approximation is very bad and quite slow.
		template<typename T>
		T gammaFunction(const T& c, const T& limit = T(1e5))
		{
			// calculate between 1 and 2, otherwise recurse
			assert(c >= 0.0);
			if (c < 1.0)
				return gammaFunction(c + 1.0)/c;
		
			if (c > 2.0)
			{
				return gammaFunction(c - 1.0)*(c-1.0);
			}
			return integrateSimpsons(std::bind2nd(GammaBaseFunctionStruct<T>(), c), 0.0, limit, 1000000.0);
		}
	};
	
	// this approximation is pretty good and fast..  12 decimal 
	//	digit of accuracy on T => double.
	// accuracy can probably be improved.
	// limit gives the ability to calculate the lower incomplete 
	//	gamma function using this routine.
	template<typename T>
	T gamma(const T& c, const T& limit = T(40))
	{
		// calculate between 1 and 2, otherwise spin a little
		assert(c >= T(0));
		if (c < T(1))
			return gamma(c + T(1))/c;
		
		if (c > T(2))
		{   // this can be improved using a smarter factorial reduction
			return gamma(c - T(1))*(c-T(1));
		}
		T x = pow(limit, c)*exp(-limit);
		T sum = 0;
		for (int n = 0; n < 100; ++n)
		{
			T fact = T(1)/c;
			for (int i = 1; i <= n; ++i)
			{
				fact *= limit/(c + i);
			}
			sum += fact;
		}
		return x*sum;
	}
	
	template<typename T>
	T beta(const T& x, const T& y)
	{
		assert(x >= T(0));
		assert(y >= T(0));
		
		return gamma(x)*gamma(y)/gamma(x+y);
	}
	
	template<typename T>
	T binomialCoefficient(const T& n, const T& k)
	{
		assert(n >= k);
		assert(n >= T(0));
		assert(k >= T(0));
		
		return T(1)/((n + T(1))*beta(n - k + T(1), k + T(1)));
	}
	
	template<typename T>
	T riemannZeta(const T& s, int limit = 20)
	{
		assert(s > T(1));
		T ret = T(0);
		for (int i = 1; i < limit; ++i)
		{
			ret += T(1)/pow(i, s);
		}
		return ret;
	}
	
	const double EulerMascheroni = 0.5772156649015328606065;
	
	template<typename T>
	T digammaSmall(const T& z)
	{
		return -EulerMascheroni - T(1)/z;
	}
	
	template<typename T>
	T digammaBig(const T& z)
	{
		// coefficients for the polynomial expansion
		const double c1 = 0.5;
		const double c2 = 0.083333333333333333333333;
		const double c3 = 0.008333333333333333333333;
		const double c4 = 0.003968253968253968253968;
		
		return (log(z) - c1 * pow(z, -1) - c2 * pow(z, -2) + c3 * pow(z, -4) + c4 * pow(z, -6));
	}
	
	template<typename T>
	T digamma(const T& z)
	{
		if (z == T(0)) { return 0; }
		
		// limits used to pick the calculation to use
		const T lower = 0.00005;
		const T
		upper = 8.5;
		
		if (z < lower) { return digammaSmall(z); }
		
		if (z > upper) { return digammaBig(z); }
		
		return digamma(z - T(1)) + T(1)/z;
		
	}
	
};



#endif // SPECIAL_FUNCTIONS
