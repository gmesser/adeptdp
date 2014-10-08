// adatetime_range.h - Adept Date and Time

#ifndef ADATETIME_RANGE_H
#define ADATETIME_RANGE_H

#include <time.h>
#include "adatetime.h"

/*
 * A range module with begin and end adatetime elements.
 */

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
	adatetime *begin;	// The beginning of the range.
	adatetime *end;	// The end of the range.
} adatetime_range;

adatetime_range *adatetime_range_allocate();
adatetime_range *adatetime_range_free(adatetime_range *range);
adatetime_range *adatetime_range_create(adatetime *begin, adatetime *end);

adatetime_range *adatetime_range_copy(adatetime_range *dst, adatetime_range *src);

void adatetime_range_set(adatetime_range *range, adatetime *begin, adatetime *end);

int adatetime_range_is_equal(adatetime_range *left, adatetime_range *right, adatetime_comparison_mode cmp);

int adatetime_is_in_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp);
int adatetime_is_before_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp);
int adatetime_is_after_range(adatetime *adt, adatetime_range *range, adatetime_comparison_mode cmp);

#ifdef	__cplusplus
}
#endif

#endif	/* ADATETIME_RANGE_H */

