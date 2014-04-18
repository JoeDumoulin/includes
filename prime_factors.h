/*
 *  prime_factors.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PRIME_FACTORS
#define PRIME_FACTORS


#include <vector>
#include <iterator>
#include <stdexcept>

#include "num.h"
#include "integer.h"
#include "integer_square_root.h"
#include "gcd.h"

namespace num
{
	template<typename T, typename Container>
	// requires Integer(T)
	void get_divisors(T t, std::back_insert_iterator<Container> out)
	{
		if (t == T(1)) return;
		T d = integer_square_root(t);
		
		while (t % d != T(0) && t > T(1)) --d;
		if (d == T(1)) 
			out++ = t; // t is prime
		else 
		{ // get the divisors of the multiples of t
			get_divisors(d, out);
			get_divisors(t/d, out);
		}
	}
	
	template<typename T>
	// requires Positive(T)
	bool is_prime(const T& t)
	{
		if (t == T(1)) return true; // 1 is prime
		T d = integer_square_root(t);
		
		while (t % d != T(0) && t > T(1)) --d;
		if (d == T(1)) 
			return true;
		return false;
	}

	// prime factors
	template<typename T, typename Container>
	// requires Integer(T)
	void prime_factors(T t, std::back_insert_iterator<Container> out)
	{
		if (t <= T(0))
		{
			out++ = T(-1);
			t /= T(-1);
		}
		if (t == T(1))
		{
			out++ = T(1);
			return;
		}
		while (even(t)) 
		{ 
			out++ = T(2);
			t /= T(2);
		}
		if (t != T(1))
		{
			get_divisors(t, out);
		}
	}
	
};

#endif // PRIME_FACTORS