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

	// The last calculated elapsed time.
	double elapsed;

	// The last calculated loop time;
	double lastloop;
} aclock;

double ac_diff(double start, double end);
double ac_current_time();
double ac_elapsedDHMS(double elapsed_time, double *days, double *hours, double *minutes, double *seconds);

void ac_init(aclock *ac);
void ac_restart(aclock *ac);
double ac_start(aclock *ac);
double ac_stop(aclock *ac);
double ac_reset(aclock *ac);
double ac_loop(aclock *ac);
double ac_check_loop(aclock *ac);
double ac_elapsed(aclock *ac);
int ac_isstopped(aclock *ac);

#ifdef	__cplusplus
}
#endif

#endif	/* ACLOCK_H */
