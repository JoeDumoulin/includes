/*
 *  finite_integral_type_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FINITE_INTEGRAL_TYPE_TESTS_H
#define FINITE_INTEGRAL_TYPE_TESTS_H

#include <iostream>

#include "../finite_integral_type.h"
#include "../prime_factors.h"

namespace finite_integral_type_tests
{
	template<class T, T BASE>
	class test
	{
	public:
		typedef typename num::finite_integral_type<T, BASE> FIT;
		test()
		{
			test_create_assign();
			test_unary_negate();
			test_add();
			test_subtract();
			test_multiply();
			test_divide();
			test_invert();
			test_modulus();
		}
		
		void test_create_assign()
		{
			assert(FIT() == T(0));
						
			assert(FIT(T(2)) == FIT(BASE+T(2)));
			FIT a(T(2));
			assert(a == FIT(T(2)));
			FIT b = a;
			assert(a == b);
			assert(a == T(2));
		}
		
		void test_unary_negate()
		{
			FIT a(BASE + T(5));
			assert(-a.normalize() == FIT(BASE - T(5)).normalize());
		}
		
		void test_add()
		{
			FIT a(T(4));
			FIT b(T(5));

			assert((a + b) == FIT(T(4) + T(5)));
			FIT c(T(101));
			assert((c += b) == FIT(T(101) + T(5)));
		}
		
		void test_subtract()
		{
			FIT a(T(4));
			FIT b(T(5));
			
			assert((a - b) == FIT(T(4) - T(5)));
			FIT c(T(101));
			assert((c -= b) == FIT(T(101) - T(5)));
		}
		
		void test_multiply()
		{
			FIT a(T(4));
			FIT b(T(5));
			
			assert((a * b) == FIT(T(4) * T(5)));
			FIT c(T(101));
			assert((c *= b) == FIT(T(101) * T(5)));
		}
		
		bool test_divide_by_zero_error(FIT a = T(2), const FIT& b = T(0))
		{
			try
			{
				a /= b;
				return false;
			}
			catch (std::runtime_error e)
			{
				return true;
			}
		}

		void test_divide()
		{
			FIT a(T(4));
			FIT b(T(5));
			FIT c(T(20));

			// since we are testing the value of division, 
			//	and for certain BASE values we will accidentally 
			//	divide by zero, we test before the assert.
			if (!test_divide_by_zero_error(T(20), T(5)))
				assert((c / b) == FIT(T(20) / T(5)));
			if (!test_divide_by_zero_error(T(20), T(4)))
				assert((c /= a) == FIT(T(20) / T(4)));
			assert(test_divide_by_zero_error());
		}
		
		bool test_no_inverse_error(const FIT& a = T(0))
		{
			try 
			{
				a.invert();
				return false;
			}
			catch (std::runtime_error e)
			{
				return true;
			}
		}
		
		void test_invert()
		{
			FIT a(T(4));
			
			assert(test_no_inverse_error());
			if (!test_no_inverse_error())
				assert(a.invert() != FIT(T(0)));
		}
		
		

		// % is not homomorphic for this class.  makes it a little difficult to test.
		bool test_modulus_of_zero_error()
		{
			try
			{
				FIT a(T(2));
				a %= FIT(T(0));
				return false;
			}
			catch (std::runtime_error e)
			{
				return true;
			}
		}
		
		// this test does not work if BASE is not prime.  
		//	I need a way to verify prime-ness.
		
		void test_modulus()
		{ 
			FIT a(T(4));
			FIT b(T(5));
			FIT c(T(20));
			
			if (BASE > T(1) && num::is_prime(BASE))
			{
				assert((c % b) == FIT((T(20) % BASE) % (T(5) % BASE)));
				assert((c %= a) == FIT((T(20) % BASE) % (T(4) % BASE)));
			}
			assert(test_modulus_of_zero_error());
		}
	};
	
	void run_tests()
	{
		std::cout << "test finite integral type classes..." << std::endl;
		
		test<int, 19>();
		test<int, 3>();
		test<int, 6>();
		
		// this fails in division.  really it is a trivial example, but interesting!
		test<int, 1>();
		
		std::cout << "test finite integral type classes completed" << std::endl;
	}
};

#endif // FINITE_INTEGRAL_TYPE_TESTS_H
