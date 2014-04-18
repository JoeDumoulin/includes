/*
 *  integer_square_root.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/11/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INTEGER_SQUARE_ROOT
#define INTEGER_SQUARE_ROOT

#include "num.h"
#include "rational.h"
#include "integer.h"

namespace num
{
	template<typename T>
	// requires Integer(T) && Nonnegative(T)
	T integer_square_root(const T& t)
	{	// Use Newton's method to solve n^2 - 4 = 0

		if (num::negative(t))
		{
			throw std::runtime_error("num::integer_square_root - positive only.");
		}
		if (t == T(1)) return T(1);

		T n = t;
		T nn = t/2;
	
		while (absolute_value(nn-n) > T(1))
		{
			n = nn;
			nn = (n+t/n)/T(2);
		}
		return floor(nn);
	}
}

#endif // INTEGER_SQUARE_ROOT
