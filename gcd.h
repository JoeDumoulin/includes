/*
 *  gcd.h
 *  rational
 *
 *  Created by Joe  Dumoulin on 1/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GCD_H
#define GCD_H

namespace num
{
	template<class T>
	// requires Integer(T)
	T gcd(T x, T y)
	{	// calculate the greatest common divisor of x and y
		// using Euclid's algorithm.
		T swap;
		while (y != T(0))
		{
			swap = y;
			y = x % y;
			x = swap;
		}
		return x;
	}
	

}

#endif // GCD_H
