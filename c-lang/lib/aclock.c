// aclock.c

#include <time.h>
#include <math.h>
#include "aclock.h"

#ifdef __MACH__
#include <mach/mach_time.h>
#endif // #ifdef __MACH__

/*
 * ac_diff
 *
 * Calculate the difference between two time values.
 *
 * Returns: the difference between the two times
 */
double ac_diff(double start, double end) {
	return(end - start);
}

/*
 * ac_current_time
 *
 * Get the current time.
 *
 * Returns: the current system time in nanoseconds
 */
double ac_current_time() {

	double ct;

#ifndef __MACH__

	// Linux
	struct timespec tp;
	int result = clock_gettime(CLOCK_MONOTONIC, &tp);
	if (result == 0) {
		ct = tp.tv_sec + (1.0e-9 * tp.tv_nsec);
	}
	else {
		// An error occurred while getting the time.
		ct = 0.0;
	}

#endif // #ifndef __MACH__

#ifdef __MACH__

	// Darwin - Apple OS X
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);

	double conv_factor = (static_cast<double>(info.numer)) / (static_cast<double>(info.denom));
	ct = mach_absolute_time() * conv_factor * 1.0e-9;

#endif // #ifdef __MACH__

	return ct;
}

/*
 * ac_init
 *
 * Initialize the members of a clock structure.
 */
void ac_init(aclock *ac) {
	double ct = ac_current_time();
	ac->lastloop = ct;
	ac->started = ct;
	ac->elapsed = 0.0;
}

/*
 * ac_restart
 *
 * Restart a clock.
 */
void ac_restart(aclock *ac) {
	double ct = ac_current_time();
	double sd = ac_diff(ac->stopped, ct);
	ac->started += sd;
	ac->lastloop += sd;
	ac->stopped = 0.0;
}

/*
 * ac_start
 *
 * Either start the clock
 * at the current time or restarts the
 * clock at 0.0.
 *
 * Calling ac_start() while the clock is
 * running restarts it at 0.0.
 *
 * Calling ac_start() while the clock is
 * stopped effectively restarts it at the
 * current time by adjusting the start
 * and lastlap times by the amount of time
 * that the clock was stopped.
 *
 * Returns: current elapsed time
 */
double ac_start(aclock *ac) {
	if (ac->stopped) {
		ac_restart(ac);
	}
	else {
		ac_init(ac);
	}

	return ac_elapsed(ac);
}

/*
 * ac_stop
 *
 * Record the time when the
 * clock is stopped.  When stopped,
 * the recorded value is used in the elapsed
 * time calculation.  The clock
 * remains stopped until the ac_start()
 * function is called to restart it.
 *
 * Returns: current elapsed time
 */
double ac_stop(aclock *ac) {
	if (!ac->stopped) {
		ac->stopped = ac_current_time();
	}

	return ac_elapsed(ac);
}

/*
 * ac_reset
 *
 * Stop the clock and initialize all times to the
 * current time.
 * The ac_start() function must be called to
 * restart it.
 *
 * Returns: current elapsed time
 */
double ac_reset(aclock *ac) {
	ac->lastloop = ac->started = ac->stopped = ac_current_time();
	ac->elapsed = 0.0;

	return ac_elapsed(ac);
}

/*
 * ac_loop
 *
 * Calculate the number of seconds
 * since loop() was last called
 * Reset the loop timer to zero.
 *
 * Returns: the number of seconds since loop() was last called
 */
double ac_loop(aclock *ac) {
	double dl; // difference from last loop

	if (ac->stopped) {
		dl = ac_diff(ac->lastloop, ac->stopped);
		ac->lastloop = ac->stopped;
	}
	else {
		double ct = ac_current_time();
		dl = ac_diff(ac->lastloop, ct);
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
double ac_check_loop(aclock *ac) {
	double dl; // difference from last loop

	if (ac->stopped) {
		dl = ac_diff(ac->lastloop, ac->stopped);
	}
	else {
		double ct = ac_current_time();
		dl = ac_diff(ac->lastloop, ct);
	}

	return dl;
}

/*
 * ac_elapsed
 *
 * Calculate the elapsed time in
 * seconds that the clock has been
 * running.
 *
 * Returns: current elapsed time
 */
double ac_elapsed(aclock *ac) {
	if (ac->stopped) {
		ac->elapsed = ac_diff(ac->started, ac->stopped);
	}
	else {
		ac->elapsed = ac_diff(ac->started, ac_current_time());
	}

	return ac->elapsed;
}

/*
 * ac_isstopped
 *
 * Indicate if the clock is currently stopped.
 *
 * Return: non-zero if the clock is stopped, and zero if it is running
 */
int ac_isstopped(aclock *ac) {
	if (ac->stopped)
		return 1;
	else
		return 0;
}

/*
 * ac_elapsedDHMS
 *
 * Break out the elapsed time into days,
 * hours, minutes, and seconds.  Those
 * are returned in the passed parameters.
 *
 * Returns: current elapsed time
 */
double ac_elapsedDHMS(double elapsed_time, double *days, double *hours, double *minutes, double *seconds) {
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
