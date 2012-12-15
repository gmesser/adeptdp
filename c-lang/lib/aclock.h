// aclock.h

#ifndef ACLOCK_H
#define ACLOCK_H

#include "astr.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct aclock {
	// The current time when the clock was started.
	double started;

	// The current time when the clock was stopped.
	double stopped;

	// The last current time that was retrieved from the system.
	double lasttime;

	// The current time when loop was last called.
	double lastloop;

	// The last time difference that was calculated.
	double lastdiff;

	// The last elapsed time that was calculated .
	double elapsed;
} aclock;

// Calculate the difference between the start and end times.
double aclock_diff(double start, double end);

// Get the current CPU time used by the program in seconds.
double aclock_current_time();

// Split the elapsed time into days, hours, minutes, and seconds.
double aclock_elapsedDHMS(double elapsed_time, double *days, double *hours, double *minutes, double *seconds);

// Create a clock.  The clock is running after creation.
aclock *aclock_create();

// Free the memory used by a clock.
aclock *aclock_free(aclock *ac);

// Initialize a clock.  The clock is running after initialization.
double aclock_init(aclock *ac);

// Restart a clock.
double aclock_restart(aclock *ac);

// Start a clock.
double aclock_start(aclock *ac);

// Stop a clock.
double aclock_stop(aclock *ac);

// Reset a clock.  The clock is stopped after reset.
double aclock_reset(aclock *ac);

// Register the completion of a loop and start timing the next loop.
double aclock_loop(aclock *ac);

// Check the loop time but do not start timing a new loop.
double aclock_check_loop(aclock *ac);

// Calculate the elapsed time the clock has been running.
double aclock_elapsed(aclock *ac);

// Check to see if the clock is stopped.
int aclock_isstopped(aclock *ac);

// Print the aclock structure to an astr instance.
astr *aclock_to_astr(aclock *ac);

#ifdef	__cplusplus
}
#endif

#endif	/* ACLOCK_H */
