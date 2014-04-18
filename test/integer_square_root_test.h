/*
 *  integer_square_root_test.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef INTEGER_SQUARE_ROOT_TEST
#define INTEGER_SQUARE_ROOT_TEST

#include <cassert>
#include <stdexcept>
#include <iostream>

#include "../integer_square_root.h"

namespace integer_square_root_tests
{
	bool catch_negative_except(int i)
	{
		try
		{
			num::integer_square_root(i);
			return true;
		}
		catch(std::runtime_error e)
		{
			return false;
		}
	}
	void test_integer_square_root()
	{
		std::cout << "test integer square root..." << std::endl;
		assert(!catch_negative_except(-1));
		
		assert(num::integer_square_root(0) == 0);
		assert(num::integer_square_root(1) == 1);
		assert(num::integer_square_root(2) == 1);
		assert(num::integer_square_root(3) == 2);
		assert(num::integer_square_root(7) == 2);
		assert(num::integer_square_root(9) == 3);
		assert(num::integer_square_root(30) == 5);
		assert(num::integer_square_root(33) == 5);
		assert(num::integer_square_root(36) == 6);
		assert(num::integer_square_root(10001) == 100);
		assert(num::integer_square_root(100000003) == 10000);
		std::cout << "test integer square root complete." << std::endl;
	}
	
	void run_tests()
	{
		test_integer_square_root();
	}
};

#endif // INTEGER_SQUARE_ROOT_TEST
