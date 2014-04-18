/*
 *  polynomial_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/31/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef POLYNOMIAL_TESTS_H
#define POLYNOMIAL_TESTS_H

#include <vector>

#include "polynomial.h"
#include "finite_integral_type.h"
#include "num.h"
#include "rational.h"

namespace polynomial_tests
{
	template<typename T>
	class test
	{
	public:
		test()
		{ // each test is named here
			test_create_assign();
			test_add();
			test_subtract();
			test_multiplication();
			test_divide();
			test_evaluate();
			test_derivative();
			test_integral();
			test_definite_integral();
		}
		
		void test_create_assign()
		{
			assert(num::polynomial<T>() == num::polynomial<T>(std::vector<T>()));
			
			T a[] = {T(5), T(7), T(9), T(30), T(25)};
			std::vector<T> va(a, a+4);
			num::polynomial<T> p(va);
			num::polynomial<T> q(a, 4);
			assert(p == q);
			
			num::polynomial<T> r(p);
			assert(p == r);
			num::polynomial<T> s = q;
			assert(q == s);
		}
		
		void test_add()
		{
			T a[] = {T(1), T(0), T(1)};
			T b[] = {T(2), T(0), T(1)};
			num::polynomial<T> p(a, 2);
			num::polynomial<T> q(a, 1);
			assert(p + q == num::polynomial<T>(b, 1));
			
			num::polynomial<T> r(a, 3);
			assert(r + q == num::polynomial<T>(b, 3));
			
			// the invariant is that summing on indices equals summing polynomials.
			T c[] = {T(15), T(13), T(11), T(9), T(7), T(5), T(3), T(1)};
			T d[] = {T(17), T(19), T(21), T(23), T(25), T(27), T(29), T(31)};
			std::vector<T> vsum(8, T(0));
			
			std::transform(c, c+8, d, vsum.begin(), num::add<T>());
			
			num::polynomial<T> s(c, 8);
			num::polynomial<T> t(d, 8);
			assert(s + t == num::polynomial<T>(vsum));
			
		}
		
		void test_subtract()
		{
			T a[] = {T(1), T(0), T(1)};
			T b[] = {T(0), T(0), T(1)};
			num::polynomial<T> p(a, 2);
			num::polynomial<T> q(a, 1);

			assert(p - q == num::polynomial<T>(b, 1));
			
			num::polynomial<T> r(a, 3);
			assert(r - q == num::polynomial<T>(b, 3));
			
			// the invariant is that summing on indices equals summing polynomials.
			T c[] = {T(15), T(13), T(11), T(9), T(7), T(5), T(3), T(1)};
			T d[] = {T(17), T(19), T(21), T(23), T(25), T(27), T(29), T(31)};
			std::vector<T> vsub(8, T(0));
			
			std::transform(c, c+8, d, vsub.begin(), num::sub<T>());
			
			num::polynomial<T> s(c, 8);
			num::polynomial<T> t(d, 8);
			//std::cout << "s - t = " << s-t << std::endl;
			assert(s - t == num::polynomial<T>(vsub));
		}
		
		void test_multiplication()
		{
			T a[] = {T(1), T(0), T(1)};
			T b[] = {T(1), T(0), T(-1)};
			T c[] = {0};
			
			// invariant 1 - any poly multiplied by 0 is zero
			num::polynomial<T> p(a, 3);
			num::polynomial<T> q(c, 1); //  = 0
			assert(p * q == q);
			assert(p * T(0) == q);
			assert(T(0) * p == q);
			
			// invariant 2 - poly<T>(1) is unit.
			num::polynomial<T> r(b, 1); // 1
			assert(p * r == p);
			assert(p * T(1) == p);
			assert(T(1) * p == p);

			// invariant 3 - multiplication by a scalar scales all elements.
			T d[3];
			for (int i = 0; i < 3; ++i)
			{
				d[i] = a[i] * T(2);
			}
			assert(p * T(2) == num::polynomial<T>(d, 3));
			
			// invariant 4 - multiplicaton of two polys scales the order of 
			//			the result by the sum of the orders of the two polys.
			num::polynomial<T> s(b, 3);
			assert((p * s).order() == p.order() + s.order());
			
			// invariant 5 multiplcation of two polys is convolution of their 
			//	coefficients.
			std::vector<T> e(3 + 3, T(0));
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					e[i+j] += a[j]*b[i];
				}
			}
			assert(p * s == num::polynomial<T>(e));
		}
		
		bool test_divide_by_zero_poly_error()
		{
			try 
			{
				T a[] = {1, 2, 3};
				num::polynomial<T> p(a, 3);
				num::polynomial<T> q;
				p /= q;
				return false;
			}
			catch (std::runtime_error e) 
			{
				return true;
			}
		}

		bool test_divide_by_zero_scalar_error()
		{
			try 
			{
				T a[] = {1, 2, 3};
				num::polynomial<T> p(a, 3);
				p /= T(0);
				return false;
			}
			catch (std::runtime_error e) 
			{
				return true;
			}
		}
		
		void test_divide()
		{
			// invariant 1 - division by zero is an error
			assert(test_divide_by_zero_poly_error());
			assert(test_divide_by_zero_scalar_error());
			
			// invariant 2 - division by 1 is unity
			T a[] = {T(1), T(-2), T(3), T(-4), T(5)};
			T b[] = {1};
			num::polynomial<T> p(a, 5);
			num::polynomial<T> q(b, 1);
			assert(p / q == p);
			
			// invariant 3 - a poly divided by itself is 1.
			assert(p / p == q);
			
			// invariant 4 - one divided by a poly is the inverse of the poly
			// (with remainder).
			num::polynomial<T> r = q / p;
			num::polynomial<T> rr = q % p;
			assert(p * r + rr == q);
			
			// invariant 5 - recover the value of a poly by multiplying the 
			//	divisor by the quotient of two polys (with remainder)
			T c[] = {-1, 1};
			num::polynomial<T> s(c, 2);
			num::polynomial<T> t = p / s;
			num::polynomial<T> tt = p % s;
			assert(s * t + tt == p);
		}
		
		void test_evaluate()
		{
			T a[] = {T(12), T(-4), T(6), T(-1)};
			num::polynomial<T> p(a, 4);
			assert(p(T(0)) == T(12));
			assert(p(T(1)) == T(13));
			
			assert(p(2) == T(20));
			assert(p(4) == T(28));
			assert(p(6) == T(-12));
			assert(p(8) == T(-148));
			assert(p(10) == T(-428));
			assert(p(12) == T(-900));
		}
		
		void test_derivative()
		{
			T a[] = {T(2), T(8), T(4), T(5)};
			T Da[] = {T(8), T(8), T(15)};
			T D2a[] = {T(8), T(30)};
			T D3a[] = {T(30)};
			assert(num::polynomial<T>(a, 4).derivative() == num::polynomial<T>(Da, 3));
			assert(num::polynomial<T>(a, 4).derivative().derivative() == num::polynomial<T>(D2a, 2));
			assert(num::polynomial<T>(a, 4).derivative().derivative().derivative() == num::polynomial<T>(D3a, 1));
		}
		
		void test_integral()
		{
			T a[] = {T(2), T(8), T(4), T(5)};
			num::polynomial<T> p(a, 4);
			assert(p.order() + 1 == p.integral().order());
		}
		
		void test_definite_integral()
		{
			T a[] = {T(2), T(8), T(4), T(5)};
			num::polynomial<T> p(a, 4);
			std::cout << "p = " << p << std::endl;
			std::cout << "I(p((T(0), T(1)) = " << p.definite_integral(T(0), T(1)) << std::endl;
			assert(p.order() + 1 == p.integral().order());
		}
	};
	
	void run_tests()
	{
		std::cout << "test polynomial class..." << std::endl;
		test<int>();
		test<num::finite_integral_type<int, 7> >();
		test<num::rational<int> >();
		test<double>();
		std::cout << "test polynomial class completed." << std::endl;
	}
};


#endif // POLYNOMIAL_TESTS_H