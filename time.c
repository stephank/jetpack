/*	time.c : routines that use the realtime process timer to synchronize
	the game.
*/

#include "copyright.h"
#include "defs.h"

struct itimerval	val;

/*	begin_timer starts the realtime process timer
*/
void	begin_timer()
{
	val.it_value.tv_sec = 30000;
	val.it_value.tv_usec = 0;
	val.it_interval.tv_sec = 30000;
	val.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &val, NULL);
}

/*	stop_timer stops the realtime process timer
*/
void	stop_timer()
{
	val.it_value.tv_sec = 0;
	val.it_value.tv_usec = 0;
	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &val, NULL);
}

/*	get_timer returns the elapsed real timer time in microseconds
*/
unsigned long	get_timer()
{
	register long	ds, dus, dt;

	getitimer(ITIMER_REAL, &val);
	ds = 30000 - val.it_value.tv_sec - 1;
	dus = 1000000 - val.it_value.tv_usec;
	dt = dus + 1000000 * ds;
	return(dt);
}
