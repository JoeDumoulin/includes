/*
 *  linear_congruence.h
 *  test
 *
 *  Created by Joe Dumoulin on 5/22/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  These functions generate uniform pseudo-random numbers using 
 *  linear congruential methods.
 */

#include "finite_integral_type.h"
#include "integer.h"


namespace num
{
    template <typename T>
    // requires finite_integral_type(T)
    T linear_congruential_increment(T seed, T multiplier, T increment)
    {
        return seed * multiplier + increment;
    }

    template <typename T>
    // requires finite_integral_type(T)
    // requires multiplier > T(2)
    T linear_congruential_step(T start, T multiplier, T increment, int step)
    {
        T a_k = num::power(multiplier, step, num::mult());
        return a_k * start + (a_k - T(1))*increment/(multiplier - T(1));
    }
    
};