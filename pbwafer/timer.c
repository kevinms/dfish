#include "timer.h"

void TIMER_init(pwtimer_t *t,Uint32 interval)
{
	t->interval = interval;
	t->end_time = SDL_GetTicks() + interval;
}

char TIMER_is_timeout(pwtimer_t *t)
{
	if(t->end_time > SDL_GetTicks())
		return 0;
	return 1;
}

/*
double
getTime()
{
	//check and return time
	struct timeval curTime;
	(void) gettimeofday (&curTime, (struct timezone *) NULL);
	return (((((double) curTime.tv_sec) * 1000000.0) 
	        + (double) curTime.tv_usec) / 1000000.0); 
}

// Subtract the `struct timeval' values X and Y,
// storing the result in RESULT.
// Return 1 if the difference is negative, otherwise 0.

int
timeval_subtract (result, x, y)
	struct timeval *result, *x, *y;
{
	// Perform the carry for the later subtraction by updating y.
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	// Compute the time remaining to wait.
	// tv_usec is certainly positive.
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	// Return 1 if result is negative.
	return x->tv_sec < y->tv_sec;
}
*/
