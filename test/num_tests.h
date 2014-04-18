/*
 *  num_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef NUM_TESTS
#define NUM_TESTS

#include <cassert>
#include <iostream>

#include "../num.h"

namespace num_tests
{
	void test_abs()
	{
		std::cout << "test absolute_value..." << std::endl;
		assert(num::absolute_value(3) == 3);
		assert(num::absolute_value(0) == 0);
		assert(num::absolute_value(3.14) == 3.14);
		assert(num::absolute_value(-3) == 3);
		assert(num::absolute_value(-9e7) == 9e7);
		assert(num::absolute_value(-9e-7) == 9e-7);
		std::cout << "test absolute_value complete" << std::endl;
	}
	
	void test_square_root()
	{
		std::cout << "test square_root..." << std::endl;
		assert(num::square_root(9) == 3);
		assert(num::square_root(num::PI*num::PI) == num::PI);
		std::cout << "test square_root complete" << std::endl;
	}
	
	void test_round()
	{
		std::cout << "test round..." << std::endl;
		assert(num::round(9.0, 0.01) == 9);
		assert(num::round(num::PI, 0.01) == 3.14);
		assert(num::round(num::PI, 0.001) == 3.142);
		assert(num::round(num::PI, 0.001) != 3.1415);
		assert(num::round(num::PI, 0.0000001) == 3.1415927);
		std::cout << "test round complete" << std::endl;
	}

	void run_tests()
	{
		test_abs();
		test_square_root();
		test_round();
	}
};

#endif // NUM_TESTS
