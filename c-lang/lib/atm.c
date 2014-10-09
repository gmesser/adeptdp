// atm.c - Adept Time

/*
 * Date and Time functions using the tm structure and time_t.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "atm.h"

// Return the current system time.
time_t now();

/*
 * Compare two integers.
 * Return the result of subtracting right from left.
 * The return will either be a number less than, equal to, or greater than zero
 * indicating that left is less than, equal to, or greater than right.
 */
int compare_int(int left, int right);

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

// Create an atm instance initialized with the current time.
atm *atm_create_now() {
	time_t t = time(0);
	atm *adt = atm_create_from_time_t(&t);
	return adt;
}

// Create an atm instance initialized with the specified time.
atm *atm_create_from_time_t(time_t *t) {
	atm *adt = NULL;
	if (t != NULL) {
		adt = atm_allocate();
		atm_set_from_time_t(adt, t);
	}
	return adt;
}

// Create an atm instance initialized with the specified gm time.
atm *atm_create_from_gmtime(struct tm *gmtm) {
	atm *adt = NULL;
	if (gmtm != NULL) {
		adt = atm_allocate();
		atm_set_from_gmtime(adt, gmtm);
	}
	return adt;
}

// Create an atm instance initialized with the specified local time.
atm *atm_create_from_loctime(struct tm *loctm) {
	atm *adt = NULL;
	if (loctm != NULL) {
		adt = atm_allocate();
		atm_set_from_localtime(adt, loctm);
	}
	return adt;
}

// Create an atm instance that is copied from the specified atm instance.
atm *atm_copy(atm *dst, atm *src) {
	if (src != NULL) {
		if(dst == NULL) {
			dst = atm_allocate();
		}
		dst->time = src->time;
		dst->original = src->original;
		memcpy(dst->gm, src->gm, sizeof(struct tm));
		memcpy(dst->loc, src->loc, sizeof(struct tm));
	}
	return dst;
}

// Determine which of the two atm instances is the earliest time.
atm *atm_earliest(atm *adt1, atm *adt2, atm_comparison_mode cmp) {
	atm *adt = NULL;
	if (adt1 != NULL && adt2 != NULL) {
		if(atm_is_before(adt1, adt2, cmp)) {
			adt = adt1;
		}
		else {
			adt = adt2;
		}
	}

	return adt;
}

// Determine which of the two atm instances is the latest time.
atm *atm_latest(atm *adt1, atm *adt2, atm_comparison_mode cmp) {
	atm *adt = NULL;
	if (adt1 != NULL && adt2 != NULL) {
		if(atm_is_after(adt1, adt2, cmp)) {
			adt = adt1;
		}
		else {
			adt = adt2;
		}
	}

	return adt;
}

// Allocate the memory for an atm structure.
atm *atm_allocate() {
	atm *adt = calloc(1, sizeof(atm));
	adt->gm = calloc(1, sizeof(struct tm));
	adt->loc = calloc(1, sizeof(struct tm));
	return adt;
}

// Free the memory that was allocated for an atm structure.
atm *atm_free(atm *adt) {
	if(adt != NULL) {
		if(adt->gm != NULL) {
			free(adt->gm);
		}
		if(adt->loc != NULL) {
			free(adt->loc);
		}
		free(adt);
	}
	return NULL;
}

// Set or create an atm instance initialized with the current time.
void atm_set_now(atm *adt) {
	time_t t = time(0);
	if(adt == NULL) {
		atm_create_from_time_t(&t);
	}
	else {
		atm_set_from_time_t(adt, &t);
	}
}

// Set or create an atm instance initialized with the specified time.
void atm_set_from_time_t(atm *adt, time_t *t) {
	if (adt == NULL) {
		adt = atm_create_from_time_t(t);
	}
	else {
		if (t != NULL) {
			adt->time = *t;
			if (adt->original == 0) {
				adt->original = adt->time;
			}
			gmtime_r(&(adt->time), adt->gm);
			localtime_r(&(adt->time), adt->loc);
		}
	}
}

// Set or create an atm instance initialized with the specified gm time.
void atm_set_from_gmtime(atm *adt, struct tm *tm) {
	if (adt == NULL) {
		adt = atm_create_from_gmtime(tm);
	}
	else {
		if (tm != NULL) {
			memcpy(adt->gm, tm, sizeof (struct tm));
			adt->time = timegm(adt->gm);
			if (adt->original == 0) {
				adt->original = adt->time;
			}
			localtime_r(&(adt->time), adt->loc);
		}
	}
}

// Set or create an atm instance initialized with the specified local time.
void atm_set_from_localtime(atm *adt, struct tm *tm) {
	if (adt == NULL) {
		adt = atm_create_from_loctime(tm);
	}
	else {
		if (tm != NULL) {
			memcpy(adt->loc, tm, sizeof (struct tm));
			adt->time = timelocal(adt->loc);
			if (adt->original == 0) {
				adt->original = adt->time;
			}
			gmtime_r(&(adt->time), adt->gm);
		}
	}
}

// Compare two atm instances.
int atm_compare(atm *left, atm *right, atm_comparison_mode cmp) {
	int result = 0;
	if (left != NULL && right != NULL) {
		if (left != right) {
			if (cmp == DATEONLY) {
				result = atm_compare_date(left, right);
			}
			else if (cmp == TIMEONLY) {
				result = atm_compare_time(left, right);
			}
			else {
				result = atm_compare_date(left, right);
				if (result == 0) {
					result = atm_compare_time(left, right);
				}
			}
		}
	}

	return result;
}

/*
 * Determine if left is equal right.
 * Return 1 is true, 0 if false;
 */
int atm_is_equal(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) == 0 ? 1 : 0;
}

/*
 * Determine if left is before right.
 * Return 1 is true, 0 if false;
 */
int atm_is_before(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) < 0 ? 1 : 0;
}

/*
 * Determine if left is after right.
 * Return 1 is true, 0 if false;
 */
int atm_is_after(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) > 0 ? 1 : 0;
}

//  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
//  int tm_min;			/* Minutes.	[0-59] */
//  int tm_hour;			/* Hours.	[0-23] */
//  int tm_mday;			/* Day.		[1-31] */
//  int tm_mon;			/* Month.	[0-11] */
//  int tm_year;			/* Year	- 1900.  */
//  int tm_wday;			/* Day of week.	[0-6] */
//  int tm_yday;			/* Days in year.[0-365]	*/
//  int tm_isdst;			/* DST.		[-1/0/1]*/
//  long int tm_gmtoff;		/* Seconds east of UTC.  */
//  const char *tm_zone;		/* Timezone abbreviation.  */

// Compare the date components of two atm instances.
int atm_compare_date(atm *left, atm *right) {
	int result = 0;
	if (left != right) {
		if (left != NULL && right != NULL) {
			if ((result = compare_int(left->loc->tm_year, right->loc->tm_year)) != 0) {
				return result;
			}
			if ((result = compare_int(left->loc->tm_mon, right->loc->tm_mon)) != 0) {
				return result;
			}
			if ((result = compare_int(left->loc->tm_mday, right->loc->tm_mday)) != 0) {
				return result;
			}
		}
	}
	return 0;
}

// Compare the time components of two atm instances.
int atm_compare_time(atm *left, atm *right) {
	int result = 0;
	if (left != right) {
		if (left != NULL && right != NULL) {
			if ((result = compare_int(left->loc->tm_hour, right->loc->tm_hour)) != 0) {
				return result;
			}
			if ((result = compare_int(left->loc->tm_min, right->loc->tm_min)) != 0) {
				return result;
			}
			if ((result = compare_int(left->loc->tm_sec, right->loc->tm_sec)) != 0) {
				return result;
			}
		}
	}
	return 0;
}

// Compare two integers.
int compare_int(int left, int right) {
	return left - right;
}