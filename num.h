/*
 *  num.h
 *  
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NUM_H
#define NUM_H

#include <cmath>
#include <stdexcept>

namespace num
{
	const double PI=3.1415926535;
	const double E=2.718281828;
	
	template<typename T>
	T absolute_value(const T& value)	
	{
		return (value < 0) ? -value : value;
	}
	
	template<class T>
	T square_root(const T& value)
	{
		typedef T value_type;
		
		if (value < value_type(0))
		{	
			// hm.  no square root of negative number.
			throw std::runtime_error("num::square_root() : cannot calculate a negative square root");
		}
		
		value_type start = value_type(2);
		value_type next = (start + value/start)/value_type(2);
		while (absolute_value(start-next) > value_type(9e-7))
		{
			start = next;
			next = (start + value/start)/value_type(2);
		}
		return next;
	}
	
	template<class T>
	T round(const T& v, const double tol)
	{
		typedef T value_type;
		value_type t = v;
		t /= tol;
		double rnd = t < 0.0 ? ceil((double) t - 0.5): floor((double) t + 0.5);
		
		if (rnd == -0.0) rnd = 0.0;
		return (value_type) rnd*tol;
		
	}
	
	// some helpers for printing containers of numbers
	template<typename Container>
	std::ostream& print_container_as_set(std::ostream& out, Container& c)
	{
		return print_container_as_set<typename Container::iterator>(out, c.begin(), c.end());
	}
	
	template<typename I>
	std::ostream& print_container_as_set(std::ostream& out, I begin, I end)
	{
		out << "{";
		I p = begin;
		if (p != end)
		{
			out << *p;
			for (++p; p != end; ++p)
			{
				out << ", " << *p;
			}
		}
		out << "}";
		
		return out;
	}
}

#endif // NUM_H
