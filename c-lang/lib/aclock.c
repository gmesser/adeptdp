// aclock.c

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "aclock.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#endif // #ifdef __MACH__

/*
 * aclock_diff
 *
 * Calculate the difference between two time values.
 *
 * Returns: the difference between the two times
 */
double aclock_diff(double start, double end) {
	return(end - start);
}

/*
 * aclock_current_time
 *
 * Get the current CPU clock in seconds.
 * 
 * This clock measures the number of CPU clock ticks used by the program.  If
 * the program calls sleep() or if the program is interrupted by the operating
 * system, those clock ticks are not included in the number of clock ticks
 * returned to the function by the clock() function.
 *
 * Returns: the current CPU clock (normally in milliseconds)
 */
double aclock_current_time() {

	clock_t ct = clock();
	return ct / (double) CLOCKS_PER_SEC;
}

/*
 * aclock_create
 *
 * Create an aclock structure and start it.
 *
 * Returns: a new, running, aclock instance
 */
aclock *aclock_create() {
	aclock *ac = malloc(sizeof(aclock));
	aclock_reset(ac);
	aclock_start(ac);
	
	return ac;
}

/*
 * aclock_free
 *
 * Free an aclock structure
 *
 * Returns: NULL pointer
 */
aclock *aclock_free(aclock *ac) {
	if(ac != NULL) {
		free(ac);
	}

	return NULL;
}

/*
 * aclock_init
 *
 * Initialize the members of a clock structure.
 */
double aclock_init(aclock *ac) {
	double ct = aclock_current_time();
	ac->lastloop = ct;
	ac->started = ct;
	ac->stopped = 0.0;
	ac->elapsed = 0.0;

	return ac->elapsed;
}

/*
 * aclock_restart
 *
 * Restart a clock.
 */
double aclock_restart(aclock *ac) {
	double ct = aclock_current_time();
	double sd = aclock_diff(ac->stopped, ct);
	ac->started += sd;
	ac->lastloop += sd;
	ac->stopped = 0.0;

	return ac->elapsed;
}

/*
 * aclock_start
 *
 * Either start the clock
 * at the current time or restarts the
 * clock at 0.0.
 *
 * Calling aclock_start() while the clock is
 * running restarts it at 0.0.
 *
 * Calling aclock_start() while the clock is
 * stopped effectively restarts it at the
 * current time by adjusting the start
 * and lastlap times by the amount of time
 * that the clock was stopped.
 *
 * Returns: current elapsed time
 */
double aclock_start(aclock *ac) {
	if (ac->stopped) {
		aclock_restart(ac);
	}
	else {
		aclock_init(ac);
	}

	return aclock_elapsed(ac);
}

/*
 * aclock_stop
 *
 * Record the time when the
 * clock is stopped.  When stopped,
 * the recorded value is used in the elapsed
 * time calculation.  The clock
 * remains stopped until the aclock_start()
 * function is called to restart it.
 *
 * Returns: current elapsed time
 */
double aclock_stop(aclock *ac) {
	if (!ac->stopped) {
		ac->stopped = aclock_current_time();
	}

	return aclock_elapsed(ac);
}

/*
 * aclock_reset
 *
 * Stop the clock and initialize all times to the
 * current time.
 * The aclock_start() function must be called to
 * restart it.
 *
 * Returns: current elapsed time
 */
double aclock_reset(aclock *ac) {
	ac->lastloop = ac->started = ac->stopped = aclock_current_time();
	ac->elapsed = 0.0;

	return aclock_elapsed(ac);
}

/*
 * aclock_loop
 *
 * Calculate the number of seconds
 * since loop() was last called
 * Reset the loop timer to zero.
 *
 * Returns: the number of seconds since loop() was last called
 */
double aclock_loop(aclock *ac) {
	double dl; // difference from last loop

	if (ac->stopped) {
		dl = aclock_diff(ac->lastloop, ac->stopped);
		ac->lastloop = ac->stopped;
	}
	else {
		double ct = aclock_current_time();
		dl = aclock_diff(ac->lastloop, ct);
		ac->lastloop = ct;
	}

	return dl;
}

/*
 * checkloop
 *
 * Calculate the number of seconds
 * since loop() was last called.
 * Does not reset the loop timer.
 *
 * Returns: the number of seconds since loop() was last called
 */
double aclock_check_loop(aclock *ac) {
	double dl; // difference from last loop

	if (ac->stopped) {
		dl = aclock_diff(ac->lastloop, ac->stopped);
	}
	else {
		double ct = aclock_current_time();
		dl = aclock_diff(ac->lastloop, ct);
	}

	return dl;
}

/*
 * aclock_elapsed
 *
 * Calculate the elapsed time in
 * seconds that the clock has been
 * running.
 *
 * Returns: current elapsed time
 */
double aclock_elapsed(aclock *ac) {
	if (ac->stopped) {
		ac->elapsed = aclock_diff(ac->started, ac->stopped);
	}
	else {
		ac->elapsed = aclock_diff(ac->started, aclock_current_time());
	}

	return ac->elapsed;
}

/*
 * aclock_isstopped
 *
 * Indicate if the clock is currently stopped.
 *
 * Return: non-zero if the clock is stopped, and zero if it is running
 */
int aclock_isstopped(aclock *ac) {
	if (ac->stopped)
		return 1;
	else
		return 0;
}

/*
 * aclock_elapsedDHMS
 *
 * Break out the elapsed time into days,
 * hours, minutes, and seconds.  Those
 * are returned in the passed parameters.
 *
 * Returns: current elapsed time
 */
double aclock_elapsedDHMS(double elapsed_time, double *days, double *hours, double *minutes, double *seconds) {
	double seconds_in_day = 60.0 * 60.0 * 24.0;
	double seconds_in_hour = 60.0 * 60.0;
	double seconds_in_minute = 60.0;

	double work_time = elapsed_time;

	if(work_time >= seconds_in_day) {
		double dd = fabs(work_time / seconds_in_day);
		work_time -= dd * seconds_in_day;
		if(days != NULL) {
			*days = dd;
		}
	}

	if(work_time >= seconds_in_hour) {
		double hh = fabs(work_time / seconds_in_hour);
		work_time -= hh * seconds_in_hour;
		if(hours != NULL) {
			*hours = hh;
		}
	}

	if(work_time >= seconds_in_minute) {
		double mm = fabs(work_time / seconds_in_minute);
		work_time -= mm * seconds_in_minute;
		if(minutes != NULL) {
			*minutes = mm;
		}
	}

	if(seconds != NULL) {
		*seconds = work_time;
	}

	return elapsed_time;
}
