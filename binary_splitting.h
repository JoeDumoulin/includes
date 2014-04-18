/*
 *  binary_splitting.h.h
 *  integrate
 *
 *  Created by Joe Dumoulin on 7/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <cassert>

// this file contains some example implementations of binary splitting that will hopefully lead to 
// a generic implementation based on http://www.ginac.de/CLN/binsplit.pdf
template<typename T>
//requires Integer(T)
T obvious_factorial(const T& t)
{
	if (t == T(2))
	{
		return t;
	}
	
	return t*obvious_factorial(t-1);
	
}


// the following is neded for binary splitting type recursion
template<typename T>
//requires Interger(T)
T rational_factorial(const T& a, const T& b)
{
//	assert(a>=b);
//	assert(b>=0);
	
	switch(a-b)
	{
		case 0:
			return T(1);
		case 1:
			return a;
		case 2:
			return a*(a-1);
		default:
			return rational_factorial(a, (a+b)/2)
			*rational_factorial((a+b)/2, b);			
	}
}