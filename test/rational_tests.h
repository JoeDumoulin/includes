/*
 *  rational_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef RATIONAL_TESTS_H
#define RATIONAL_TESTS_H

#include <iostream>
#include <stdexcept>

#include "../rational.h"

namespace rational_tests
{
	template<class T>
	class test
	{
	public:
		test()
		{
			test_create_assign();
			test_unary_negate();
			test_add();
			test_subtract();
			test_multiply();
			test_divide();
			test_invert();
			test_ordering_operators();
			test_floor_ceiling();
		}
		
		bool catch_create_zero_denom_error()
		{
			try
			{
				num::rational<T>(T(1), T(0));
				return false;
			}
			catch (std::runtime_error e)
			{
				return true;
			}
		}
	
		void test_create_assign()
		{
			assert(num::rational<T>() == T(0));
		
			num::rational<T> r(T(2));
			assert(r == T(2));
			assert(r == num::rational<T>(T(2)));
			assert(r == num::rational<T>(T(2),T(1)));
			assert(catch_create_zero_denom_error());
		
			num::rational<T> s(T(2),T(5));
			num::rational<T> t = s;
			assert(t == s);
			assert(r.numerator() == T(2) && r.denominator() == T(1));
		}
	
		void test_unary_negate()
		{
			num::rational<T> s(T(2), T(5));
			num::rational<T> t = -s;
			assert(t != s);
			assert(t.numerator() == T(-2) && t.denominator() == T(5));
		}
	
		void test_add()
		{
			num::rational<T> p(T(-3), T(17));
			num::rational<T> q(T(5), T(17));
			assert(p+q == num::rational<T>(T(2), T(17)));
			assert((p+=num::rational<T>(T(2), T(17))) == num::rational<T>(T(-1), T(17)));
		}
		
		void test_subtract()
		{
			num::rational<T> p(T(-3), T(17));
			num::rational<T> q(T(5), T(17));
			
			assert(p-q == num::rational<T>(T(-8), T(17)));
			assert(q-p == num::rational<T>(T(8), T(17)));
			assert(q-(-p) == num::rational<T>(T(2), T(17)));
		}
		
		void test_multiply()
		{
			num::rational<T> p(T(0), T(17));
			num::rational<T> q(T(1), T(17));
			num::rational<T> r(T(-1), T(17));
			num::rational<T> s(T(5), T(17));
			num::rational<T> t(T(3), T(5));
			num::rational<T> u(T(3), T(17));
			
			assert(p*q == T(0));
			assert(s*t == u);
			assert(q*r == num::rational<T>(T(-1), T(17)*T(17)));
			assert((p *= q) == num::rational<T>(T(0)));
			assert((s *= num::rational<T>(T(17))) == T(5));
		}
		
		bool catch_zero_denominator(const num::rational<T>& p, const num::rational<T>& q)
		{
			try
			{
				p/q;
				return false;
			}
			catch(std::runtime_error e)
			{
				return true;
			}
		}
		
		void test_divide()
		{
			num::rational<T> p(T(0), T(17));
			num::rational<T> q(T(1), T(17));
			num::rational<T> r(T(-1), T(17));
			num::rational<T> s(T(5), T(17));
			num::rational<T> u(T(3), T(17));
			
			assert(p/q == T(0));
			assert(catch_zero_denominator(q, p));
			assert(q/r == T(-1));
			assert(s/u == num::rational<T>(5, 3));
			assert((s /= u) == num::rational<T>(5, 3));			
		}
		bool catch_zero_denominator_inverse(num::rational<T> p)
		{
			try
			{
				p.invert();
				return false;
			}
			catch(std::runtime_error e)
			{
				return true;
			}
		}
		
		void test_invert()
		{
			num::rational<T> p(T(0), T(17));
			num::rational<T> q(T(1), T(17));
			num::rational<T> r(T(-1), T(17));
			
			assert(catch_zero_denominator_inverse(p));
			assert(num::rational<T>(5, 3).invert() == num::rational<T>(3, 5));
			
			num::rational<T> s(q);
			assert(s.invert() == T(17));
			assert(num::rational<T>(q).invert() == T(17));
			assert(q.invert() * r == T(-1));
		}
		
		void test_ordering_operators()
		{
			num::rational<T> p(T(1), T(2));
			num::rational<T> q(T(2), T(3));
			num::rational<T> r(T(2), T(5));
			num::rational<T> s(T(3), T(5));
			
			assert(p == num::rational<T>(T(1), T(2)));
			assert(p <= num::rational<T>(T(1), T(2)));
			assert(p >= num::rational<T>(T(1), T(2)));
			assert(p != q);
			assert(p < q);
			assert(p <= q);
			assert(s > p);
			assert(s > r);
			assert(s >= p);
		}
		
		void test_floor_ceiling()
		{
			assert(floor(num::rational<T>(T(1), T(2))) == T(0));
			assert(ceil(num::rational<T>(T(1), T(2))) == T(1));
			assert(floor(num::rational<T>(T(10))) == T(10));
			assert(floor(num::rational<T>(T(50), T(3))) == T(16));
			assert(ceil(num::rational<T>(T(50), T(3))) == T(17));			
		}
	};
	
	void run_tests()
	{
		std::cout << "test rational class..." << std::endl;
		test<int>();
		test<long>();
		test<long long>();
		// test<num::rational<int> >();  // this one has issues (doh!)
		std::cout << "test rational class completed." << std::endl;
	}
};

#endif // RATIONAL_TESTS_H
