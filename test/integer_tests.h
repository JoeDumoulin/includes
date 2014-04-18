/*
 *  integer_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INTEGER_TESTS
#define INTEGER_TESTS

#include <cassert>

#include "integer.h"

namespace integer_tests
{
	template<class T>
	class test
	{
	public:
		test()
		{
			test_successor();
			test_predecessor();
			test_twice();
			test_half_nonnegative();
			test_binary_scale_down_nonnegative();
			test_binary_scale_up_nonnegative();
			test_positive();
			test_negative();
			test_zero();
			test_one();
			test_even();
			test_odd();
			test_add();
			test_sub();
			test_mult();
			test_div();
		}

		void test_successor()
		{
			assert(num::successor(T(0)) == T(1));
			assert(num::successor(T(-1)) == T(0));
			assert(num::successor(T(10)) == T(11));
		}
	
		void test_predecessor()
		{
			assert(num::predecessor(T(1)) == T(0));
			assert(num::predecessor(T(0)) == T(-1));
			assert(num::predecessor(T(11)) == T(10));
		}
	
		void test_twice()
		{
			assert(num::twice(T(0)) == T(0));
			assert(num::twice(T(-1)) == T(-2));
			assert(num::twice(T(10)) == T(20));
		}
	
		void test_half_nonnegative()
		{
			assert(num::half_nonnegative(T(0)) == T(0));
			assert(num::half_nonnegative(T(-1)) == T(0));
			assert(num::half_nonnegative(T(2)) == T(1));
			assert(num::half_nonnegative(T(10)) == T(5));
			assert(num::half_nonnegative(T(11)) == T(5));
		}
	
		void test_binary_scale_down_nonnegative()
		{
			assert(num::binary_scale_down_nonnegative(T(0), T(5)) == T(0));
			assert(num::binary_scale_down_nonnegative(T(-1), T(2)) == T(0));
			assert(num::binary_scale_down_nonnegative(T(2), T(1)) == T(1));
			assert(num::binary_scale_down_nonnegative(T(10), T(2)) == T(2));
			assert(num::binary_scale_down_nonnegative(T(11), T(3)) == T(1));
		}
	
		void test_binary_scale_up_nonnegative()
		{
			assert(num::binary_scale_up_nonnegative(T(0), T(5)) == T(0));
			assert(num::binary_scale_up_nonnegative(T(-1), T(2)) == T(0));
			assert(num::binary_scale_up_nonnegative(T(2), T(1)) == T(4));
			assert(num::binary_scale_up_nonnegative(T(10), T(2)) == T(40));
			assert(num::binary_scale_up_nonnegative(T(11), T(3)) == T(88));
		}
	
		void test_positive()
		{
			assert(num::positive(T(1)));
			assert(!num::positive(T(-1)));
			assert(!num::positive(T(0)));
			assert(num::positive(T(10)));
			assert(!num::positive(T(-10)));
		}
	
		void test_negative()
		{
			assert(!num::negative(T(1)));
			assert(num::negative(T(-1)));
			assert(!num::negative(T(0)));
			assert(!num::negative(T(10)));
			assert(num::negative(T(-10)));
		}
	
		void test_zero()
		{
			assert(num::zero(T(0)));
			assert(!num::zero(T(-1)));
			assert(!num::zero(T(1)));
			assert(num::zero(T(1) - T(1)));
		}
	
		void test_one()
		{
			assert(!num::one(T(0)));
			assert(!num::one(T(-1)));
			assert(num::one(T(1)));
			assert(!num::one(T(1) - T(1)));
		}
	
		void test_even()
		{
			assert(num::even(T(0)));
			assert(!num::even(T(1)));
			assert(num::even(T(2)));
			assert(num::even(T(-2)));
			assert(!num::even(T(3)));
		}
	
		void test_odd()
		{
			assert(!num::odd(T(0)));
			assert(num::odd(T(1)));
			assert(!num::odd(T(2)));
			assert(!num::odd(T(-2)));
			assert(num::odd(T(3)));
		}
	
	void test_add()
	{
		assert(num::add<T>()(T(3), T(0)) == (3));
		assert(num::add<T>()(T(3), T(-3)) == T(0));
		assert(num::add<T>()(T(0), T(1)) == T(1));
		assert(num::add<T>()(T(-4), T(-6)) == T(-10));
		assert(num::add<T>()(T(4), T(6)) == T(10));
	}
	
	void test_sub()
	{
		assert(num::sub<T>()(T(1), T(1)) == T(0));
		assert(num::sub<T>()(T(-1), T(1)) == T(-2));
		assert(num::sub<T>()(T(1), T(-1)) == T(2));
		assert(num::sub<T>()(T(0), T(1)) == T(-1));
		assert(num::sub<T>()(T(1), T(0)) == T(1));
	}
	
	void test_mult()
	{
		assert(num::mult<T>()(T(1), T(1)) == T(1));
		assert(num::mult<T>()(T(-1), T(1)) == T(-1));
		assert(num::mult<T>()(T(1), T(-1)) == T(-1));
		assert(num::mult<T>()(T(0), T(1)) == T(0));
		assert(num::mult<T>()(T(1), T(0)) == T(0));
	}
	
	bool catch_div_error()
	{
		try
		{
			num::div<T>()(T(1), T(0));
			return false;
		}
		catch(std::runtime_error e)
		{
			return true;
		}
	}
	
	void test_div()
	{
		assert(num::div<T>()(T(1), T(1)) == T(1));
		assert(num::div<T>()(T(-1), T(1)) == T(-1));
		assert(num::div<T>()(T(1), T(-1)) == T(-1));
		assert(num::div<T>()(T(0), T(1)) == T(0));
		assert(catch_div_error());
	}
	};
	void run_tests()
	{
		std::cout << "test integer functions... " << std::endl;
		test<int>();
		std::cout << "test integer functions complete." << std::endl;
	}
};

#endif //INTEGER_TESTS
