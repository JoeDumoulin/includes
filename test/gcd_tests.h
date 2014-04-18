/*
 *  gcd_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/10/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GCD_TESTS
#define GCD_TESTS

#include <cassert>
#include <vector>

#include "../gcd.h"
#include "../num.h"
#include "../prime_factors.h"

namespace gcd_tests
{
	void test_gcd()
	{
		std::cout << "test gcd..." << std::endl;
		assert(num::gcd(5, 7) == 1);
		assert(num::gcd(91, 28) == 7);
		assert(num::gcd(91, 52) == 13);
		std::cout << "test gcd complete" << std::endl;
	}
	
	void run_tests()
	{
		test_gcd();
	}
};

#endif // GCD_TESTS
