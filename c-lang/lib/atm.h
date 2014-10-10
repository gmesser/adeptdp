// atm.h - Adept Time

#ifndef ADATETIME_H
#define ADATETIME_H

#include <time.h>

/*
 * A date and time module with date and time elements with
 * a comparison mode which will determine how the instances
 * will compare to each other.
 * 
 * DATEANDTIME comparison mode: date and time components will influence comparisons.
 *   The atm instance will be created with all components set
 *   to their proper values.
 *   The atm instance will be compared with another atm instance
 *   using both the date and the time components.
 *	  Two atm instances will match if they were created on or set to the
 *   exact same date and time.
 * DATEONLY comparison mode: only date components will influence comparisons.
 *   The atm instance will be created with the time components set
 *   to zero (0).
 *   The atm instance will be compared with another atm instance
 *   using the date components only.  Time of day will not be considered.
 *	  Two atm instances will match if they were created on or set to the
 *   same day, regardless of the time of day.
 * TIMEONLY comparison mode: only time components will influence comparisons.
 *   The atm instance will be created with the date components set
 *   to zero (0).
 *   The atm instance will be compared with another atm instance
 *   using the time components only.  Date will not be considered.
 *	  Two atm instances will match if they were created on or set to the
 *   same time, regardless of the date.
 * The default comparison mode is DATEANDTIME if atm_set_comparison_mode()
 * is not called to change it.
 */

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum { DATEONLY, TIMEONLY, DATEANDTIME } atm_comparison_mode;

typedef struct {
	time_t original;	// The original calendar time that was set.
	time_t time;		// The calendar time - in seconds.
	struct tm *gm;		// The Greenwich mean time (UTC).
	struct tm *loc;		// The local time.
} atm;

atm *atm_allocate();
atm *atm_free(atm *at);
atm *atm_create_now();
atm *atm_create_from_time_t(time_t *t);
atm *atm_create_from_gmtime(struct tm *tm);
atm *atm_create_from_loctime(struct tm *tm);

atm *atm_copy(atm *dst, atm *src);

void atm_set_now(atm *at);
void atm_set_original(atm *at);
void atm_set_from_time_t(atm *at, time_t *t);
void atm_set_from_gmtime(atm *at, struct tm *tm);
void atm_set_from_localtime(atm *at, struct tm *tm);

int atm_compare(atm *left, atm *right, atm_comparison_mode cmp);
int atm_compare_date(atm *left, atm *right);
int atm_compare_time(atm *left, atm *right);

int atm_is_equal(atm *left, atm *right, atm_comparison_mode cmp);
int atm_is_before(atm *left, atm *right, atm_comparison_mode cmp);
int atm_is_after(atm *left, atm *right, atm_comparison_mode cmp);

atm *atm_earliest(atm *at1, atm *at2, atm_comparison_mode cmp);
atm *atm_latest(atm *at1, atm *at2, atm_comparison_mode cmp);

#ifdef	__cplusplus
}
#endif

#endif	/* ADATETIME_H */

