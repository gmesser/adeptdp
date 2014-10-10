// atm.c - Adept Time

/*
 * Date and Time functions using the tm structure and time_t.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "atm.h"

int compare_int(int left, int right);

// ---------------------------------------------------------------------------

/*
 * Compare two integers.
 * Returns the result of subtracting right from left.
 *
 * Return a number less than, equal to, or greater than zero if
 * left is less than, equal to, or greater than right, respectively.
 */
int compare_int(int left, int right) {
	return left - right;
}

/*
 * Allocate the memory for an atm structure.
 *
 * Allocates the memory for the enclosing structure and for the components.
 */
atm *atm_allocate() {
	atm *at = calloc(1, sizeof(atm));
	at->gm = calloc(1, sizeof(struct tm));
	at->loc = calloc(1, sizeof(struct tm));
	return at;
}

/*
 * Free the memory that was allocated for an atm structure.
 */
atm *atm_free(atm *at) {
	if(at != NULL) {
		if(at->gm != NULL) {
			free(at->gm);
		}
		if(at->loc != NULL) {
			free(at->loc);
		}
		free(at);
	}
	return NULL;
}

/*
 * Create an atm instance initialized with the current time.
 */
atm *atm_create_now() {
	time_t t = time(0);
	atm *at = atm_create_from_time_t(&t);
	return at;
}

/*
 * Create an atm instance initialized with the specified time.
 */
atm *atm_create_from_time_t(time_t *t) {
	atm *at = NULL;
	if (t != NULL) {
		at = atm_allocate();
		atm_set_from_time_t(at, t);
	}
	return at;
}

/*
 * Create an atm instance initialized with the specified gm time.
 */
atm *atm_create_from_gmtime(struct tm *gmtm) {
	atm *at = NULL;
	if (gmtm != NULL) {
		at = atm_allocate();
		atm_set_from_gmtime(at, gmtm);
	}
	return at;
}

/*
 * Create an atm instance initialized with the specified local time.
 */
atm *atm_create_from_loctime(struct tm *loctm) {
	atm *at = NULL;
	if (loctm != NULL) {
		at = atm_allocate();
		atm_set_from_localtime(at, loctm);
	}
	return at;
}

/*
 * Create a copy of an atm instance.
 */
atm *atm_copy(atm *dst, atm *src) {
	if (src != NULL) {
		if(dst != NULL) {
			dst = atm_free(dst);
			dst = atm_allocate();
		}
		dst->time = src->time;
		dst->original = src->original;
		memcpy(dst->gm, src->gm, sizeof(struct tm));
		memcpy(dst->loc, src->loc, sizeof(struct tm));
	}
	return dst;
}

/*
 * Set or create an atm instance initialized with the current time.
 */
void atm_set_now(atm *at) {
	time_t t = time(0);
	atm_set_from_time_t(at, &t);
}

/*
 * Set an atm instance to its original time.
 * The original time is the time that was set when the instance was allocated 
 * and initialized.  It is retained over the lifetime of the instance, so the 
 * instance can be returned to its originally set time.
 */
void atm_set_original(atm *at) {
	if(at != NULL) {
		time_t t = at->original;
		atm_set_from_time_t(at, &t);
	}
}

/*
 * Set or create an atm instance initialized with the specified time.
 */
void atm_set_from_time_t(atm *at, time_t *t) {
	if (t != NULL) {
		if (at == NULL) {
			at = atm_allocate();
		}
		at->time = *t;
		if (at->original == 0) {
			at->original = at->time;
		}
		gmtime_r(&(at->time), at->gm);
		localtime_r(&(at->time), at->loc);
	}
}

/*
 * Set or create an atm instance initialized with the specified gm time.
 */
void atm_set_from_gmtime(atm *at, struct tm *tm) {
	if (tm != NULL) {
		if (at == NULL) {
			at = atm_allocate();
		}
		memcpy(at->gm, tm, sizeof (struct tm));
		at->time = timegm(at->gm);
		if (at->original == 0) {
			at->original = at->time;
		}
		localtime_r(&(at->time), at->loc);
	}
}

/*
 * Set or create an atm instance initialized with the specified local time.
 */
void atm_set_from_localtime(atm *at, struct tm *tm) {
	if (tm != NULL) {
		if (at == NULL) {
			at = atm_allocate();
		}
		memcpy(at->loc, tm, sizeof (struct tm));
		at->time = timelocal(at->loc);
		if (at->original == 0) {
			at->original = at->time;
		}
		gmtime_r(&(at->time), at->gm);
	}
}

/*
 * Compare two atm instances.
 *
 * Return a number less than, equal to, or greater than zero if
 * left is less than, equal to, or greater than right, respectively.
 */
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
 * Determine if left is a time that is equal to right.
 *
 * Return 1 if true, 0 if false;
 */
int atm_is_equal(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) == 0 ? 1 : 0;
}

/*
 * Determine if left is a time that is before (less than) right.
 *
 * Return 1 if true, 0 if false;
 */
int atm_is_before(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) < 0 ? 1 : 0;
}

/*
 * Determine if left is a time that is after (greater than) right.
 *
 * Return 1 if true, 0 if false;
 */
int atm_is_after(atm *left, atm *right, atm_comparison_mode cmp) {
	return atm_compare(left, right, cmp) > 0 ? 1 : 0;
}

// ---------------------------------------------------------------------------
// struct tm elements and usages:
//
// ---- USED IN TIMEONLY AND DATEANDTIME COMPARISONS
//  int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
//  int tm_min;			/* Minutes.	[0-59] */
//  int tm_hour;		/* Hours.	[0-23] */
//
// ---- USED IN DATEONLY AND DATEANDTIME COMPARISONS
//  int tm_mday;		/* Day.		[1-31] */
//  int tm_mon;			/* Month.	[0-11] */
//  int tm_year;		/* Year	- 1900. */
//
// ---- NOT USED IN COMPARISONS
//  int tm_wday;				/* Day of week.	 [0-6] */
//  int tm_yday;				/* Days in year. [0-365]	*/
//  int tm_isdst;				/* DST. [-1/0/1] */
//  long int tm_gmtoff;		/* Seconds east of UTC. */
//  const char *tm_zone;		/* Timezone abbreviation. */
// ---------------------------------------------------------------------------

/*
 * Compare the date components of two atm instances.
 *
 * Called when comparing two atm instances with
 * the atm_comparison_mode of DATEONLY or DATEANDTIME.
 *
 * Return a number less than, equal to, or greater than zero if
 * left is less than, equal to, or greater than right, respectively.
 */
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

/*
 * Compare the time components of two atm instances.
 *
 * Called when comparing two atm instances with
 * the atm_comparison_mode of TIMEONLY or DATEANDTIME.
 *
 * Return a number less than, equal to, or greater than zero if
 * left is less than, equal to, or greater than right, respectively.
 */
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

/*
 * Determine the earliest of two atm instances.
 *
 * Return the earliest of the two atm instances.
 * If the two are equal, return the second parameter.
 */
atm *atm_earliest(atm *at1, atm *at2, atm_comparison_mode cmp) {
	atm *at = NULL;
	if (at1 != NULL && at2 != NULL) {
		if(atm_is_before(at1, at2, cmp)) {
			at = at1;
		}
		else {
			at = at2;
		}
	}

	return at;
}

/*
 * Determine the latest of two atm instances.
 *
 * Return the latest of the two atm instances.
 * If the two are equal, return the second parameter.
 */
atm *atm_latest(atm *at1, atm *at2, atm_comparison_mode cmp) {
	atm *at = NULL;
	if (at1 != NULL && at2 != NULL) {
		if(atm_is_after(at1, at2, cmp)) {
			at = at1;
		}
		else {
			at = at2;
		}
	}

	return at;
}
