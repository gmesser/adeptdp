// aclock.c - Adept Clock

/*
 * Clock stopwatch functions.
 */

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "astr.h"
#include "aclock.h"

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
 * Returns: The current CPU clock (system-dependent, normally in milliseconds)
 */
double aclock_current_time() {
	clock_t ct = clock(); // current time
	double ct_secs = (double) (ct / (double) CLOCKS_PER_SEC);
	return ct_secs;
}

/*
 * aclock_diff
 *
 * Calculate the difference between two time values.
 *
 * Parameter: The start time in seconds
 * Parameter: The end time in seconds
 * Returns: The difference in seconds between the two times
 */
double aclock_diff(double start, double end) {
	double d = (end - start); // difference in times
	return d;
}

/*
 * aclock_create
 *
 * Create an aclock structure and start it.
 *
 * Returns: A new, running, aclock instance
 */
aclock *aclock_create() {
	aclock *ac = malloc(sizeof(aclock));

	assert(ac != NULL);
	
	if (ac != NULL) {
		aclock_init(ac);
	}
	
	return ac;
}

/*
 * aclock_free
 *
 * Free an aclock structure
 *
 * Parameter: The aclock instance
 * Returns: NULL pointer
 */
aclock *aclock_free(aclock *ac) {
	if(ac != NULL) {
		free(ac);
		ac = NULL;
	}

	return ac;
}

/*
 * aclock_init
 *
 * Initialize the members of a clock structure.
 *
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_init(aclock *ac) {
	double et = 0.0; // elapsed time

	assert(ac != NULL);

	if (ac != NULL) {
		ac->lasttime = aclock_current_time();
		ac->lastloop = ac->lasttime;
		ac->started = ac->lasttime;
		ac->stopped = 0.0;
		ac->elapsed = 0.0;
		et = 0.0;
	}

	return et;
}

/*
 * aclock_restart
 *
 * Restart a clock.
 *
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_restart(aclock *ac) {
	double et = 0.0; // elapsed time

	assert(ac != NULL);

	if (ac != NULL) {
		ac->lasttime = aclock_current_time();
		ac->lastdiff = aclock_diff(ac->stopped, ac->lasttime);
		ac->started += ac->lastdiff;
		ac->lastloop += ac->lastdiff;
		ac->stopped = 0.0;
		et = aclock_elapsed(ac);
	}

	return et;
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
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_start(aclock *ac) {
	double et = 0.0; // elapsed time

	assert(ac != NULL);

	if (ac != NULL) {
		if (aclock_isstopped(ac)) {
			aclock_restart(ac);
		}
		else {
			aclock_init(ac);
		}
		et = ac->elapsed;
	}

	return et;
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
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_stop(aclock *ac) {
	double et = 0.0; // elapsed time

	assert(ac != NULL);

	if (ac != NULL) {
		if (!aclock_isstopped(ac)) {
			ac->stopped = ac->lasttime = aclock_current_time();
		}
		et = aclock_elapsed(ac);
	}

	return et;
}

/*
 * aclock_reset
 *
 * Stop the clock.  
 * Initialize all times to the current time.
 * Initialize all calculated values to 0.0.
 * The aclock_start() function must be called to restart the clock.
 *
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_reset(aclock *ac) {
	double et = 0.0; // elapsed time

	assert(ac != NULL);

	if (ac != NULL) {
		ac->lasttime = ac->lastloop = ac->started = ac->stopped = aclock_current_time();
		et = ac->lastdiff = ac->elapsed = 0.0;
	}

	return et;
}

/*
 * aclock_loop
 *
 * Calculate the number of seconds
 * since loop() was last called
 * Reset the loop timer to zero.
 *
 * Parameter: The aclock instance
 * Returns: The number of seconds since loop() was last called
 */
double aclock_loop(aclock *ac) {
	double lt = 0.0; // loop time

	assert(ac != NULL);

	if (ac != NULL) {
		if (aclock_isstopped(ac)) {
			lt = aclock_diff(ac->lastloop, ac->stopped);
			ac->lastloop = ac->stopped;
		}
		else {
			ac->lasttime = aclock_current_time();
			lt = aclock_diff(ac->lastloop, ac->lasttime);
			ac->lastloop = ac->lasttime;
		}
	}

	return lt;
}

/*
 * checkloop
 *
 * Calculate the number of seconds
 * since loop() was last called.
 * Does not reset the loop timer.
 *
 * Parameter: The aclock instance
 * Returns: The number of seconds since loop() was last called
 */
double aclock_check_loop(aclock *ac) {
	double lt = 0.0; // loop time

	assert(ac != NULL);

	if (ac != NULL) {
		if (aclock_isstopped(ac)) {
			lt = aclock_diff(ac->lastloop, ac->stopped);
		}
		else {
			ac->lasttime = aclock_current_time();
			lt = aclock_diff(ac->lastloop, ac->lasttime);
		}
	}

	return lt;
}

/*
 * aclock_elapsed
 *
 * Calculate the elapsed time in
 * seconds that the clock has been
 * running.
 *
 * Parameter: The aclock instance
 * Returns: The current elapsed time
 */
double aclock_elapsed(aclock *ac) {
	double et; // elapsed time
	
	assert(ac != NULL);

	if (ac != NULL) {
		if (aclock_isstopped(ac)) {
			ac->elapsed = ac->lastdiff = aclock_diff(ac->started, ac->stopped);
		}
		else {
			ac->lasttime = aclock_current_time();
			ac->elapsed = aclock_diff(ac->started, ac->lasttime);
		}

		et = ac->elapsed;
	}

	return et;
}

/*
 * aclock_isstopped
 *
 * Indicate if the clock is currently stopped.
 *
 * Parameter: The aclock instance
 * Return: Non-zero if the clock is stopped, and zero if it is running
 */
int aclock_isstopped(aclock *ac) {
	assert(ac != NULL);

	if (ac != NULL && ac->stopped > 0.0) {
		return 1;
	}
	else {
		return 0;
	}
}

/*
 * aclock_elapsedDHMS
 *
 * Break out the elapsed time into days,
 * hours, minutes, and seconds.  Those
 * are returned in the passed parameters.
 *
 * Parameter: The elapsed time
 * Parameter: A pointer to a double to store the number of days of elapsed time
 * Parameter: A pointer to a double to store the number of hours of elapsed time in the days
 * Parameter: A pointer to a double to store the number of minutes of elapsed time in the hour
 * Parameter: A pointer to a double to store the number of seconds of elapsed time in the minute
 * Returns: The passed-in elapsed time
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

astr *aclock_to_astr(aclock *ac) {
	char *lbl_started   = "    Started: ";
	char *lbl_stopped   = "    Stopped: ";
	char *lbl_isstopped = "isstopped(): ";
	char *lbl_lasttime  = "  Last Time: ";
	char *lbl_lastloop  = "  Last Loop: ";
	char *lbl_lastdiff  = "  Last Diff: ";
	char *lbl_elapsed   = "    Elapsed: ";
	astr *as = NULL;

	if (ac != NULL) {
		int stopped = aclock_isstopped(ac);
		as = astr_printf("%s%.10g\n%s%.10g\n%s%d\n%s%.10g\n%s%.10g\n%s%.10g\n%s%.10g",
			lbl_started, ac->started,
			lbl_stopped, ac->stopped,
			lbl_isstopped, stopped,
			lbl_lasttime, ac->lasttime,
			lbl_lastloop, ac->lastloop,
			lbl_lastdiff, ac->lastdiff,
			lbl_elapsed, ac->elapsed);
	}
	return as;
}
