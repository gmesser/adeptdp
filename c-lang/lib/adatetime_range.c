// adatetime.c - Adept Date and Time

/*
 * Date and Time functions.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "adatetime.h"
#include "adatetime_range.h"

// Allocate the memory for an adatetime_range structure.
adatetime_range *adatetime_range_allocate() {
	adatetime_range *range = calloc(1, sizeof(adatetime_range));
	range->begin = adatetime_allocate();
	range->end = adatetime_allocate();
	return range;
}

// Free the memory that was allocated for an adatetime_range structure.
adatetime_range *adatetime_range_free(adatetime_range *range) {
	if(range != NULL) {
		if(range->begin != NULL) {
			range->begin = adatetime_free(range->begin);
		}
		if(range->end != NULL) {
			range->end = adatetime_free(range->end);
		}
		free(range);
	}
	return NULL;
}

adatetime_range *adatetime_range_create(adatetime *adt1, adatetime *adt2) {
	adatetime_range *range = adatetime_range_allocate();
	range->begin = adatetime_copy(range->begin, adatetime_earliest(adt1, adt2, DATEANDTIME));
	range->end = adatetime_copy(range->end, adatetime_latest(adt1, adt2, DATEANDTIME));
	return range;
}

adatetime_range *adatetime_range_copy(adatetime_range *dst, adatetime_range *src) {
	if(src != NULL) {
		if(dst == NULL) {
			dst = adatetime_range_create(src->begin, src->end);
		}
		else {
			dst->begin = adatetime_copy(dst->begin, adatetime_earliest(src->begin, src->end, DATEANDTIME));
			dst->end = adatetime_copy(dst->end, adatetime_latest(src->begin, src->end, DATEANDTIME));
		}
	}
}

void adatetime_range_set(adatetime_range *range, adatetime *adt1, adatetime *adt2) {
	if(adt1 != NULL && adt2 != NULL) {
		if(range == NULL) {
			range = adatetime_range_create(adt1, adt2);
		}
		else {
			range->begin = adatetime_copy(range->begin, adatetime_earliest(adt1, adt2, DATEANDTIME));
			range->end = adatetime_copy(range->end, adatetime_latest(adt1, adt2, DATEANDTIME));
		}
	}
}

/*
 * Determine if the begin and end dates of two adatetime ranges are equal.
 *
 * Return 1 if true, 0 if false.
 */
int adatetime_range_is_equal(adatetime_range *left, adatetime_range *right, adatetime_comparison_mode cmp) {
	int result = 1;
	if(left != NULL && right != NULL) {
		result = adatetime_is_equal(left->begin, right->begin, cmp);
		if(result == 1) {
			result = adatetime_is_equal(left->end, right->end, cmp);
		}
	}

	return result;
}

// Some useful adatetime range checking functions.

/*
 * Determine if the adatetime is within the begin and end dates of the adatetime range (inclusive).
 *
 * Return 1 if true, 0 if false.
 */
int adatetime_is_in_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = !adatetime_is_before(adt, range->begin, cmp);
		if(result == 1) {
			result = !adatetime_is_after(adt, range->end, cmp);
		}
	}

	return result;
}

/*
 * Determine if the adatetime is before the begin date of the adatetime range.
 *
 * Return 1 if true, 0 if false.
 */
int adatetime_is_before_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = adatetime_is_before(adt, range->begin, cmp);
	}

	return result;
}

/*
 * Determine if the adatetime is after the end date of the adatetime range.
 *
 * Return 1 if true, 0 if false.
 */
int adatetime_is_after_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp) {
	int result = 1;
	if(range != NULL && adt != NULL) {
		result = adatetime_is_after(adt, range->end, cmp);
	}

	return result;
}
