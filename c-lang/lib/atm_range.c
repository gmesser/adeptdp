// atm.c - Adept Time Range

/*
 * Date and Time functions using the tm structure and time_t.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "atm.h"
#include "atm_range.h"

// Allocate the memory for an atm_range structure.
atm_range *atm_range_allocate() {
	atm_range *range = calloc(1, sizeof(atm_range));
	range->begin = atm_allocate();
	range->end = atm_allocate();
	return range;
}

// Free the memory that was allocated for an atm_range structure.
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

atm_range *atm_range_create(atm *adt1, atm *adt2) {
	atm_range *range = atm_range_allocate();
	range->begin = atm_copy(range->begin, atm_earliest(adt1, adt2, DATEANDTIME));
	range->end = atm_copy(range->end, atm_latest(adt1, adt2, DATEANDTIME));
	return range;
}

atm_range *atm_range_copy(atm_range *dst, atm_range *src) {
	if(src != NULL) {
		if(dst == NULL) {
			dst = atm_range_create(src->begin, src->end);
		}
		else {
			dst->begin = atm_copy(dst->begin, atm_earliest(src->begin, src->end, DATEANDTIME));
			dst->end = atm_copy(dst->end, atm_latest(src->begin, src->end, DATEANDTIME));
		}
	}
}

void atm_range_set(atm_range *range, atm *adt1, atm *adt2) {
	if(adt1 != NULL && adt2 != NULL) {
		if(range == NULL) {
			range = atm_range_create(adt1, adt2);
		}
		else {
			range->begin = atm_copy(range->begin, atm_earliest(adt1, adt2, DATEANDTIME));
			range->end = atm_copy(range->end, atm_latest(adt1, adt2, DATEANDTIME));
		}
	}
}

/*
 * Determine if the begin and end dates of two atm ranges are equal.
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

// Some useful atm range checking functions.

/*
 * Determine if the atm is within the begin and end dates of the atm range (inclusive).
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_in_range(atm *adt, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = !atm_is_before(adt, range->begin, cmp);
		if(result == 1) {
			result = !atm_is_after(adt, range->end, cmp);
		}
	}

	return result;
}

/*
 * Determine if the atm is before the begin date of the atm range.
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_before_range(atm *adt, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = atm_is_before(adt, range->begin, cmp);
	}

	return result;
}

/*
 * Determine if the atm is after the end date of the atm range.
 *
 * Return 1 if true, 0 if false.
 */
int atm_is_after_range(atm *adt, atm_range *range, atm_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = atm_is_after(adt, range->end, cmp);
	}

	return result;
}
