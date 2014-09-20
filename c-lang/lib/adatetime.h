// adatetime.h - Adept Date and Time

#ifndef ADATETIME_H
#define	ADATETIME_H

#include <time.h>

/*
 * A date and time module with date and time elements initialized according to
 * a comparison mode which will determine how the instances will compare to each
 * other.
 * 
 * With the DATEONLY comparison mode, only date components will
 * influence comparisons.
 *   The adatetime instance will be created with the time components set
 *   to zero (0).
 *   The adatetime instance will be compared with another adatetime instance
 *   using the date components only.  Time of day will not be considered.
 *	  Two adatetime instances will match if they were created on or set to the
 *   same day, regardless of the time of day.
 * With the TIMEONLY comparison mode, only time components will
 * influence comparisons.
 *   The adatetime instance will be created with the date components set
 *   to zero (0).
 *   The adatetime instance will be compared with another adatetime instance
 *   using the time components only.  Date will not be considered.
 *	  Two adatetime instances will match if they were created on or set to the
 *   same time, regardless of the date.
 * With the DATEANDTIME comparison mode, both date and time components will
 * influence comparisons.
 *   The adatetime instance will be created with all components set
 *   to their proper values.
 *   The adatetime instance will be compared with another adatetime instance
 *   using both the date and the time components.
 *	  Two adatetime instances will match if they were created on or set to the
 *   exact same date and time.
 * The default comparison mode is DATEANDTIME if adatetime_set_comparison_mode()
 * is not called to change it.
 */

#ifdef	__cplusplus
extern "C" {
#endif

enum comparison_mode { DATEONLY, TIMEONLY, DATEANDTIME };

typedef struct {
	time_t original;	// The original time, before the comparison mode was applied.
	time_t time;		// The time, filtered.
	struct tm *gm;		// The Greenwich mean time, filtered.
	struct tm *loc;	// The local time, filtered.
	comparison_mode cmp;	// The comparison mode applied during initialization.
} adatetime;

adatetime *adatetime_create_from_now();
adatetime *adatetime_create_from_time_t(time_t t);
adatetime *adatetime_create_from_gmtime(struct tm *tm);
adatetime *adatetime_create_from_loctime(struct tm *tm);
adatetime *adatetime_copy(adatetime *adt);
adatetime *adatetime_free(adatetime *adt);
adatetime *adatetime_diff(adatetime *left, adatetime *right);

void adatetime_set_from_time_t(adatetime *adt, time_t t);
void adatetime_set_from_gmtime(adatetime *adt, struct tm *tm);
void adatetime_set_from_localtime(adatetime *adt, struct tm *tm);
void adatetime_set_comparison_mode(adatetime *adt, comparison_mode mode);

int adatetime_compare(adatetime *left, adatetime *right);
int adatetime_lessthan(adatetime *left, adatetime *right);

#ifdef	__cplusplus
}
#endif

#endif	/* ADATETIME_H */

