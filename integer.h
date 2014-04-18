/*
 *  Integer.h
 *  num
 *
 *  Created by Joe Dumoulin on 9/5/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 * This header defines macros for numeric operations that 
 * provide important tools for more complex operations.
 */
#ifndef INTEGER_H
#define INTEGER_H

#include <cassert>
#include <stdexcept>

namespace num
{
	template<typename I>
	//	requires Integer(I)
	I successor(const I& i) { return i + I(1); }

	template<typename I>
	//	requires Integer(I)
	I predecessor(const I& i) { return i - I(1); }
	
	template<typename I>
	//	requires Integer(I)
	I twice(const I& i) { return i + i; }
	
	// unary predicates
	template<typename I>
	//	requires Integer(I)
	bool positive(const I& i) { return (i > I(0)); }
	
	template<typename I>
	//	requires Integer(I)
	I half_nonnegative(const I& i) { return positive(i) ? i >> 1 : I(0); }
	
	template<typename I>
	//	requires Integer(I)
	I binary_scale_down_nonnegative(const I& i, const I& k) 
	{ return (positive(i) && positive(k)) ? i >> k : I(0); }

	template<typename I>
	//	requires Integer(I)
	I binary_scale_up_nonnegative(const I& i, const I& k) 
	{ return (i >= I(0) && k >= I(0)) ? i << k : I(0); }
	
	template<typename I>
	//	requires Integer(I)
	bool negative(const I& i) { return (i < I(0)); }
	
	template<typename I>
	//	requires Integer(I)
	bool zero(const I& i) { return (i == I(0)); }
	
	template<typename I>
	//	requires Integer(I)
	bool one(const I& i) { return (i == I(1)); }
	
	// helper
	template<typename I>
	//	requires Integer(I) - two's complement
	bool even(const I& i) { return i == ((i >> 1) << 1); }
	
	template<typename I>
	//	requires Integer(I) - two's complement
	bool odd(const I& i) { return !even(i); }
	
	// simple binary operations on a field
	template<typename T>
	class add
	{
	public:
		typedef size_t DistanceType;
		typedef T domain;
		
		T operator()(const T& t1, const T& t2)
		{ return t1+t2; }
	};
	
	template<typename T>
	class sub
	{
	public:
		typedef size_t DistanceType;
		typedef T domain;
		
		T operator()(const T& t1, const T& t2)
		{ return t1-t2; }
	};
	
	template<typename T>
	class mult
	{
	public:
		typedef size_t DistanceType;
		typedef T domain;
		
		T operator()(T t1, T t2)
		{ return t1*t2; }
	};
	
	template<typename T>
	class div
	{
	public:
		typedef size_t DistanceType;
		typedef T domain;
		
		T operator()(T t1, T t2)
		{ 
			if (t2 == T(0))
			{
				throw std::runtime_error("num::div - division error (integer divide by zero?)");
			}

			try
			{
				T t = t1/t2;
				return t;
			}
			catch(...)
			{
				throw std::runtime_error("num::div - division error (integer divide by zero?)");
			}
		}
	};
	
	
	
	// **************************************************
	// efficient power computation for binary operations
	template<typename I, typename Op>
	//	requires Integer(I) && BinaryOperation(Op)
	typename Op::domain power_accumulate_positive(typename Op::domain r, typename Op::domain a, I n, Op op)
	{
		// precondition: associative(op) && positive(n)
		assert(positive(n));
		while (true)
		{
			if (odd(n))
			{
				r = op(r, a);
				if (one(n)) return r;
			}
			a = op(a, a);
			n = half_nonnegative(n);
		}
	}
	
	template<typename I, typename Op>
	//	requires Integer(I) && BinaryOperation(Op)
	typename Op::domain power_accumulate(typename Op::domain r, typename Op::domain a, I n, Op op)
	{
		// precondition: associative(op) && (positive(n) || zero(n))
		assert(positive(n) || zero(n));
		if (zero(n)) return r;
		return power_accumulate_positive(r, a, n, op);
	}
	
	template<typename I, typename Op>
	//	requires Integer(I) && BinaryOperation(Op)
	typename Op::domain power(typename Op::domain a, I n, Op op)
	{
		// precondition: associative(op) && positive(n)
		assert(positive(n) || zero(n));
		if (zero(n)) return typename Op::domain(1);
		while (even(n))
		{
			a = op(a,a);
			n = half_nonnegative(n);
		}
		n = half_nonnegative(n);
		if (zero(n)) return a;
		return power_accumulate_positive(a, op(a, a), n, op);
	}
}


#endif // INTEGER_H
