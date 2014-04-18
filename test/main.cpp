#include <iostream>

#include "num_tests.h"
#include "gcd_tests.h"
#include "prime_factor_tests.h"
#include "integer_square_root_test.h"
#include "integer_tests.h"
#include "rational_tests.h"
#include "finite_integral_type_tests.h"
#include "polynomial_tests.h"

int main (int argc, char * const argv[]) 
{
	num_tests::run_tests();
	gcd_tests::run_tests();
	prime_factor_tests::run_tests();
	integer_square_root_tests::run_tests();
	integer_tests::run_tests();
	rational_tests::run_tests();
	finite_integral_type_tests::run_tests();
	polynomial_tests::run_tests();
    return 0;
}
