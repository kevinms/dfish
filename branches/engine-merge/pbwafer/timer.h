#ifndef __TIMER_H
#define __TIMER_H

#include <SDL.h>

typedef struct pwtimer_s {
	Uint32 interval;
	Uint32 end_time;
} pwtimer_t;

void TIMER_init(pwtimer_t *t,Uint32 interval);
char TIMER_is_timeout(pwtimer_t *t);

#endif /* !__TIMER_H */
