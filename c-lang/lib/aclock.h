// aclock.h

#ifndef ACLOCK_H
#define ACLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct aclock {
	// The current time when the clock was started.
	double started;

	// The current time when the clock was stopped.
	double stopped;

	// The current time when loop was last called.
	double lastloop;

	// The last calculated elapsed time.
	double elapsed;
} aclock;

double aclock_diff(double start, double end);
double aclock_current_time();
double aclock_elapsedDHMS(double elapsed_time, double *days, double *hours, double *minutes, double *seconds);

aclock *aclock_create();
aclock *aclock_free(aclock *ac);
double aclock_init(aclock *ac);
double aclock_restart(aclock *ac);
double aclock_start(aclock *ac);
double aclock_stop(aclock *ac);
double aclock_reset(aclock *ac);
double aclock_loop(aclock *ac);
double aclock_check_loop(aclock *ac);
double aclock_elapsed(aclock *ac);
int aclock_isstopped(aclock *ac);

#ifdef	__cplusplus
}
#endif

#endif	/* ACLOCK_H */
