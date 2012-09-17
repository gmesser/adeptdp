// astr_conversions.c - Adept String

/*
 * Functions to convert the contents of an astr instance to a numeric value in
 * the safest way, insofar as they detect when there are characters in the
 * value that are not numeric.  
 * Note that these functions set the global errno if there is a problem
 * converting the value.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"

/*
 * astr_to_long
 *
 * Convert the astr instance string to a long.
 *
 * Parameter: The astr instance to be evaluated
 * Errno:     EINVAL if the string is an invalid number
 *            EDOM if the string is NULL or empty
 *            0 if the string was successfully evaluated
 * Returns:   The evaluated long value of the string
 */
long astr_to_long(const astr *as) {
   long num;
   char *end;

	if (as != NULL) {
      if(as->string != NULL && strlen(as->string) > 0) {
         num = strtol(as->string, &end, 10);
         if(strlen(end) == 0) {
            errno = 0;
         } else {
            num = 0L;
            errno = EINVAL;
         }
      }
		else {
         num = 0L;
         errno = EDOM;
      }
   }

   return num;
}

/*
 * astr_to_double
 *
 * Convert the astr instance string to a double.
 *
 * Parameter: The astr instance to be evaluated
 * Errno:     EINVAL if the string is an invalid number
 *            EDOM if the string is NULL or empty
 *            0 if the string was successfully evaluated
 * Returns:   The evaluated long value of the string
 */
double astr_to_double(const astr *as) {
   double num;
   char *end;

	if (as != NULL) {
      if(as->string != NULL && strlen(as->string) > 0) {
         num = strtod(as->string, &end);
         if(strlen(end) == 0) {
            errno = 0;
         }
			else {
            num = 0.0;
            errno = EINVAL;
         }
      }
		else {
         num = 0.0;
         errno = EDOM;
      }
   }

   return num;
}
