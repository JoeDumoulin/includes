/*
 *  prime_factor_tests.h
 *  test
 *
 *  Created by Joe Dumoulin on 10/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PRIME_FACTOR_TESTS
#define PRIME_FACTOR_TESTS

#include <vector>
#include <iterator>
#include <numeric>
#include <stdexcept>

#include "../integer.h"

namespace prime_factor_tests
{
	// print results (for testing the test)
	void print_prime_factors(int number)
	{
		try 
		{
			std::vector<int> factors;
			num::prime_factors(number, back_inserter(factors));
			
			std::cout << "factors of " << number << std::endl;
			num::print_container_as_set(std::cout, factors.begin(), factors.end()) << std::endl;
		}
		catch (std::runtime_error e) 
		{
			return;
		}
	}
	
	template<typename T>
	bool verify_prime_factor_result(const T& t)
	{
		try
		{
			std::vector<int> factors;
			num::prime_factors(t, back_inserter(factors));

			// we multiply out each element to determine if we got the right numbers
			T result = T(1);
			return (t == std::accumulate(factors.begin(), factors.end(), result, num::mult<T>()));
		}
		catch (std::runtime_error e)
		{
			return false;
		}
	}
	
	void test_is_prime()
	{
		assert(num::is_prime(52579));
		assert(num::is_prime(1));
		assert(!num::is_prime(1001));
	}

	void test_prime_factors()
	{
		std::cout << "test prime_factors..." << std::endl;
	
		assert(verify_prime_factor_result(-1));
		assert(verify_prime_factor_result(1));
		assert(verify_prime_factor_result(24));
		assert(verify_prime_factor_result(24*27));
		assert(verify_prime_factor_result(31752));
		assert(verify_prime_factor_result(41*71*1001));
		assert(verify_prime_factor_result(1000000001));
		assert(verify_prime_factor_result(52579));
		
		test_is_prime();
	
		std::cout << "test prime_factors complete" << std::endl;
	
	}


	void run_tests()
	{
		test_prime_factors();
	}
};


#endif // PRIME_FACTOR_TESTS