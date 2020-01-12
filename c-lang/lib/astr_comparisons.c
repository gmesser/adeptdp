// astr_compare.c - Adept String Comparisons

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"

#ifndef NULL
#define NULL 0
#endif

/*
 * astr_equals
 *
 * Determine if two astr instances are equal.
 *
 * Quickly determines if they are NOT equal by comparing the checksums.
 * If the checksums do not match, the strings are not equal.  One integer
 * comparison quickly detects if the strings are not equal.
 * If the checksums match, do the full comparison to confirm equality.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Returns:   1 if equal, 0 if not
 */
int astr_equals(const astr *as1, const astr *as2) {
	if (as1 != NULL && as2 != NULL) {
		if (as1->checksum != as2->checksum) {
			return 0;
		}
	}
	return (astr_compare(as1, as2) == 0 ? 1 : 0);
}

/*
 * astr_prefix_equals
 *
 * Determine if the prefixes of two astr instances are equal.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Parameter: The number of prefix characters to compare
 * Returns:   1 if equal, 0 if not
 */
int astr_prefix_equals(const astr *as1, const astr *as2, const int num_prefix_chars) {
	if (as1 != NULL && as2 != NULL) {
		if (as1->string != NULL && as2->string != NULL) {
			return (strncmp(as1->string, as2->string, num_prefix_chars) == 0 ? 1 : 0);
		}
	}
	return 0;
}

/*
 * astr_less_than
 *
 * Determine if one astr instance is less than another.
 *
 * Convenience method.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Returns:   1 if as1<as2, 0 if not
 */
int astr_less_than(const astr *as1, const astr *as2) {
	return (astr_compare(as1, as2) < 0 ? 1 : 0);
}

/*
 * astr_greater_than
 *
 * Determine if one astr instance is greater than another.
 *
 * Convenience method.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Returns:   1 if as1>as2, 0 if not
 */
int astr_greater_than(const astr *as1, const astr *as2) {
	return (astr_compare(as1, as2) > 0 ? 1 : 0);
}

/*
 * astr_compare
 *
 * Compare two astr instances.
 * A simple wrapper around strcmp();
 *
 * If one instance pointer is null and the other isn't, or
 * if the string in one instance is null and the other isn't,
 * the null instance or string is reported to be less than the non-null.
 * Two null instance pointers, or two instances with null strings, are equal.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Returns:   <0 if as1<as2, 0 if as1==as2, >0 if as1>as2
 * See also:  strcmp()
 */
int astr_compare(const astr *as1, const astr *as2) {
	int result = 0;

	if (as1 != NULL && as2 != NULL) {
		if (as1->string != NULL && as2->string != NULL) {
			result = strcmp(as1->string, as2->string);
		}
		else if (as1->string == NULL && as2->string != NULL) {
			result = -1;
		}
		else if (as1->string != NULL && as2->string == NULL) {
			result = 1;
		}
		else {
			result = 0;
		}
	}
	else if (as1 == NULL && as2 != NULL) {
		result = -1;
	}
	else if (as1 != NULL && as2 == NULL) {
		result = 1;
	}
	else {
		result = 0;
	}
	return result;
}

/*
 * astr_prefix_compare
 *
 * Compare the first n characters of two astr instances.
 * A simple wrapper around strncmp();
 *
 * If one instance pointer is null and the other isn't, or
 * if the string in one instance is null and the other isn't,
 * the null instance or string is reported to be less than the non-null.
 * Two null instance pointers, or two instances with null strings, are equal.
 *
 * Parameter: The first astr instance
 * Parameter: The second astr instance
 * Parameter: The number of prefix characters to compare
 * Returns:   <0 if as1<as2, 0 if as1==as2, >0 if as1>as2
 * See also:  strncmp()
 */
int astr_prefix_compare(const astr *as1, const astr *as2, const int num_prefix_chars) {
	int result = 0;

	if (as1 != NULL && as2 != NULL) {
		if (as1->string != NULL && as2->string != NULL) {
			result = strncmp(as1->string, as2->string, num_prefix_chars);
		}
		else if (as1->string == NULL && as2->string != NULL) {
			result = -1;
		}
		else if (as1->string != NULL && as2->string == NULL) {
			result = 1;
		}
		else {
			result = 0;
		}
	}
	else if (as1 == NULL && as2 != NULL) {
		result = -1;
	}
	else if (as1 != NULL && as2 == NULL) {
		result = 1;
	}
	else {
		result = 0;
	}
	return result;
}
