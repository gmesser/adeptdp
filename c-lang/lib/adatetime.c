// adatetime.c - Adept Date and Time

/*
 * Date and Time functions.
 */

#include <stdlib.h>
#include <time.h>
#include "adatetime.h"

// Edit the tm structures as specified by the comparison mode.
void edit_tm(adatetime *adt);

/*
 * For reference, here are the standard C time functions (and POSIX versions):
 *
 * char *asctime(const struct tm *timeptr);
 *		Returns a pointer to a string which represents the day and time of the structure timeptr. The string is in the following format:
 *		DDD MMM dd hh:mm:ss YYYY
 *			DDD	Day of the week (Sun, Mon, Tue, Wed, Thu, Fri, Sat)
 *			MMM	Month of the year (Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec)
 *			dd	Day of the month (1,...,31)
 *			hh	Hour (0,...,23)
 *			mm 	Minute (0,...,59)
 *			ss	Second (0,...,59)
 *			YYYY	Year
 *    The string is terminated with a newline character and a null character.
 *		The string is always 26 characters long (including the terminating newline and null characters).
 *
 * clock_t clock(void);
 *		Returns the processor clock time used since the beginning of an implementation-defined era (normally the beginning of the program).
 *		The returned value divided by CLOCKS_PER_SEC results in the number of seconds. If the value is unavailable, then -1 is returned.
 *
 * char *ctime(const time_t *timer);
 *     Returns a string representing the localtime based on the argument timer. This is equivalent to:
 *     asctime(locatime(timer));
 *
 * double difftime(time_t time1, time_t time2);
 *		Calculates the difference of seconds between time1 and time2 (time1-time2).
 *
 * struct tm *gmtime(const time_t *timer);
 * struct tm *gmtime_r (const time_t *time, struct tm *resultp);
 *		The value of timer is broken up into the structure tm and expressed in Coordinated Universal Time (UTC) also known as Greenwich Mean Time (GMT).
 *		A null pointer is returned if UTC is not available.
 *
 * struct tm *localtime(const time_t *timer);
 * struct tm *localtime_r (const time_t *time, struct tm *resultp);
 *		The value of timer is broken up into the structure tm and expressed in the local time zone.
 *
 * time_t mktime(struct tm *timeptr);
 * time_t timelocal (struct tm *brokentime);
 * time_t timegm (struct tm *brokentime);
 *		Converts the structure pointed to by timeptr into a time_t value according to the local time zone.
 *		The values in the structure are not limited to their constraints. If they exceed their bounds,
 *		then they are adjusted accordingly so that they fit within their bounds.
 *		The original values of tm_wday (day of the week) and tm_yday (day of the year) are ignored,
 *		but are set correctly after the other values have been constrained.
 *		tm_mday (day of the month) is not corrected until after tm_mon and tm_year are corrected.
 *		After adjustment the structure still represents the same time.
 *		If the calendar time cannot be represented, then -1 is returned.
 *
 * size_t strftime(char *str, size_t maxsize, const char *format, const struct tm *timeptr);
 * 
 * time_t time(time_t *timer);
 *		Calculates the current calender time and encodes it into time_t format.
 *		The time_t value is returned. If timer is not a null pointer, then the value is also stored into the object it points to.
 *		If the time is unavailable, then -1 is returned.
 */

void edit_tm(adatetime *adt) {
	if(adt->cmp == DATEONLY) {
		adt->loc->tm_hour = 0;
		adt->loc->tm_min = 0;
		adt->loc->tm_sec = 0;
		adt->gm->tm_hour = 0;
		adt->gm->tm_min = 0;
		adt->gm->tm_sec = 0;
		adt->time = timelocal(adt->loc);
	}
	else if(adt->cmp == TIMEONLY) {
		adt->loc->tm_mday = 0;
		adt->loc->tm_mon = 0;
		adt->loc->tm_wday = 0;
		adt->loc->tm_yday = 0;
		adt->loc->tm_year = 0;
		adt->gm->tm_mday = 0;
		adt->gm->tm_mon = 0;
		adt->gm->tm_wday = 0;
		adt->gm->tm_yday = 0;
		adt->gm->tm_year = 0;
		adt->time = timelocal(adt->loc);
	}
}

/*
 * adatetime_create_now()
 *
 * Create an adatetime instance initialized with the current time.
 */
adatetime *adatetime_create_now(comparison_mode cmp) {
	time_t t = time(0);
	adatetime *adt = adatetime_create_from_time_t(t, cmp);
	return adt;
}

adatetime *adatetime_create_from_time_t(time_t t, comparison_mode cmp) {
	adatetime *adt = malloc(sizeof(adatetime));
	adt->time = t;
	adt->original = t;
	adt->cmp = cmp;
	adt->gm = malloc(sizeof(struct tm));
	gmtime_r(&t, adt->gm);
	adt->loc = malloc(sizeof(struct tm));
	localtime_r(&t, adt->loc);
	edit_tm(adt);
	return adt;
}

adatetime *adatetime_create_from_gmtime(struct tm *tm, comparison_mode cmp) {
	return 0;
}

adatetime *adatetime_create_from_loctime(struct tm *tm, comparison_mode cmp) {
	return 0;
}

adatetime *adatetime_copy(adatetime *adt) {
	return 0;
}

adatetime *adatetime_free(adatetime *adt) {
	return 0;
}

adatetime *adatetime_diff(adatetime *left, adatetime *right) {
	return 0;
}

void adatetime_set_now(adatetime *adt) {

}

void adatetime_set_from_time_t(adatetime *adt, time_t t) {

}

void adatetime_set_from_gmtime(adatetime *adt, struct tm *tm) {

}

void adatetime_set_from_localtime(adatetime *adt, struct tm *tm) {

}

void adatetime_set_comparison_mode(adatetime *adt, comparison_mode mode) {

}

int adatetime_compare(adatetime *left, adatetime *right) {
	return 0;
}

int adatetime_lessthan(adatetime *left, adatetime *right) {
	return 0;
}

