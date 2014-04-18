/*
 *  timing.h
 *  integrate
 *
 *  Created by Joe Dumoulin on 8/1/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef TIMING_H
#define TIMING_H

#  include <stdint.h>
#  include <mach/mach_time.h>

// Internal time and clock access functions
unsigned int convertTime(unsigned long long sysTime)
{
    static double conversion = 0.0;
    
    if( conversion == 0.0 )
    {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info( &info );
        
		//Convert the timebase into milliseconds
		if( err == 0  )
			conversion = 1e-3 * (double) info.numer / (double) info.denom;
    }
    
    return (unsigned int) (conversion * sysTime);
}

unsigned systemTime()
{
	return convertTime(mach_absolute_time());
}

unsigned long systemClock()
{
	return (unsigned long) mach_absolute_time();
}

// Sets up the timing system and registers the performance timer.
// note that this is here for runtime compatibility on windows.
void initTime()
{
	//This initializes the system timer for some OS implementations
}


#endif //TIMING_H
