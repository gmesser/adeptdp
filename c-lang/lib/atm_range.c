// atm.c - Adept Time Range

/*
 * Date and Time functions using the tm structure and time_t.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "atm.h"
#include "atm_range.h"

/*
 * Allocate the memory for an atm_range structure.
 *
 * Allocates the memory for the enclosing structure and for the components.
 */
atm_range *atm_range_allocate() {
	atm_range *range = calloc(1, sizeof(atm_range));
	range->begin = atm_allocate();
	range->end = atm_allocate();
	return range;
}

/*
 * Free the memory that was allocated for an atm_range structure.
 */
atm_range *atm_range_free(atm_range *range) {
	if(range != NULL) {
		if(range->begin != NULL) {
			range->begin = atm_free(range->begin);
		}
		if(range->end != NULL) {
			range->end = atm_free(range->end);
		}
		free(range);
	}
	return NULL;
}

/*
 * Create a time range with copies of the supplied begin and end times.
 * The times do not need to be in order.
 * This function sets the beginning of the range to the earliest time and
 * sets the end of the range to latest time.
 */
atm_range *atm_range_create(atm *at1, atm *at2) {
	atm_range *range = NULL;
	if(at1 != NULL && at2 != NULL) {
		range = atm_range_allocate();
		atm_range_set(range, at1, at2);
	}
	return range;
}

/*
 * Create a copy of a time range.
 */
atm_range *atm_range_copy(atm_range *dst, atm_range *src) {
	if(src != NULL) {
		if(dst != NULL) {
			dst = atm_range_free(dst);
			dst = atm_range_allocate();
		}
		dst->begin = atm_copy(dst->begin, atm_earliest(src->begin, src->end, DATEANDTIME));
		dst->end = atm_copy(dst->end, atm_latest(src->begin, src->end, DATEANDTIME));
	}
}

/*
 * Set the range with copies of the supplied begin and end times.
 * The times do not need to be in order.
 * This function sets the beginning of the range to the earliest time and
 * sets the end of the range to latest time.
 */
void atm_range_set(atm_range *range, atm *at1, atm *at2) {
	if(at1 != NULL && at2 != NULL) {
		if(range == NULL) {
			range = atm_range_allocate();
		}
		range->begin = atm_copy(range->begin, atm_earliest(at1, at2, DATEANDTIME));
		range->end = atm_copy(range->end, atm_latest(at1, at2, DATEANDTIME));
	}
}

/*
 * Determine if the begin and end times of two atm ranges are equal.
 *
 * Return 1 if true, 0 if false.
 */
int atm_range_is_equal(atm_range *left, atm_range *right, atm_comparison_mode cmp) {
	int result = 1;
	if(left != NULL && right != NULL) {
		result = atm_is_equal(left->begin, right->begin, cmp);
		if(result == 1) {
			result = atm_is_equal(left->end, right->end, cmp);
		}
	}

	return result;
}

/*
 * Determine if the atm is between the begin and end times of the atm range (inclusive).
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_in_range(atm *at, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && at != NULL) {
		result = !atm_is_before(at, range->begin, cmp);
		if(result == 1) {
			result = !atm_is_after(at, range->end, cmp);
		}
	}

	return result;
}

/*
 * Determine if the atm is before the begin time of the atm range.
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_before_range(atm *at, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && at != NULL) {
		result = atm_is_before(at, range->begin, cmp);
	}

	return result;
}

/*
 * Determine if the atm is after the end time of the atm range.
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_after_range(atm *at, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && at != NULL) {
		result = atm_is_after(at, range->end, cmp);
	}

	return result;
}
