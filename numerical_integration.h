/*
 *  numerical_integration.h
 *  integrate
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NUMERICAL_INTEGRATION
#define NUMERICAL_INTEGRATION

namespace num
{
	//numerical integration by retangular method
	template<typename UnaryFunction, typename T>
	T integrateSquare(UnaryFunction f, const T& low, const T& high, const T& interval = 100)
	{
		//divide into interval segments
		T ret = 0;
		T inc = (high-low)/interval;
		for (T level = low; level < high; level += inc)
		{
			ret += f(level)*inc;
		}
		return ret;
	}
	
	//numerical integration by triangular method
	template<typename UnaryFunction, typename T>
	T integrateTriangle(const UnaryFunction& f, const T& low, const T& high, const T& interval = 100)
	{
		//divide into interval segments
		T ret = 0;
		T inc = (high-low)/interval;
		T left = low;
		while(left < high)
		{
			T right = left+inc;
			ret += f(left)*inc + (f(right)-f(left))*inc/2;
			left = right;
		}
		return ret;
	}
	
	//numerical integration by simpson's method
	template<typename UnaryFunction, typename T>
	T integrateSimpsons(const UnaryFunction& f, const T& low, const T& high, const T& interval = 100)
	{
		//divide into interval segments
		T ret = 0;
		T inc = (high-low)/interval;
		T left = low;
		while(left < high)
		{
			T center = left+inc;
			T right = center+inc;
			ret += inc*(f(left) + 4*f(center) + f(right))/3;
			left = right;
		}
		return ret;
	}
};

#endif // NUMERICAL_INTEGRATION