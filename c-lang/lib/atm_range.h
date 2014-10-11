// atm_range.h - Adept Time Range

#ifndef ADATETIME_RANGE_H
#define ADATETIME_RANGE_H

#include <time.h>
#include "atm.h"

/*
 * A range module with begin and end atm elements.
 */

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct atm_range {
	atm *begin; // The beginning of the range.
	atm *end;   // The end of the range.
} atm_range;

atm_range *atm_range_allocate();
atm_range *atm_range_free(atm_range *range);
atm_range *atm_range_create(atm *begin, atm *end);

atm_range *atm_range_copy(atm_range *src);

void atm_range_set(atm_range *range, atm *begin, atm *end);

int atm_range_is_equal(atm_range *left, atm_range *right, atm_comparison_mode cmp);

int atm_is_in_range(atm *adt, atm_range *range, atm_comparison_mode cmp);
int atm_is_before_range(atm *adt, atm_range *range, atm_comparison_mode cmp);
int atm_is_after_range(atm *adt, atm_range *range, atm_comparison_mode cmp);

#ifdef	__cplusplus
}
#endif

#endif	/* ADATETIME_RANGE_H */

