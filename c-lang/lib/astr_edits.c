// astr_edits.c - Adept String

/*
 * These functions edit the contents of an astr instance.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include "astr.h"

/*
 * astr_to_upper_case
 *
 * Convert all of the characters in the astr string to upper case.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_to_upper_case(astr *as) {
	int i;
	if (as != NULL && as->string != NULL && as->length > 0) {
		for (i = 0; i < as->length && (as->string)[i] != '\0'; i++) {
			(as->string)[i] = toupper((as->string)[i]);
		}
		astr_update(as);
	}
	return as;
}

/*
 * astr_to_lower_case
 *
 * Convert all of the characters in the astr string to lower case.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_to_lower_case(astr *as) {
	int i;
	if (as != NULL && as->string != NULL && as->length > 0) {
		for (i = 0; i < as->length && (as->string)[i] != '\0'; i++) {
			(as->string)[i] = tolower((as->string)[i]);
		}
		astr_update(as);
	}
	return as;
}

/*
 * astr_to_mixed_case
 *
 * Convert all of the characters in the astr string to mixed case.
 * The initial letter of each word will be upper case,
 * the rest of the characters in each word will be lower case.
 * A word is considered to be a series of characters where each character
 * satisfies the standard isalnum() test;
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_to_mixed_case(astr *as) {
	int i;
	char last = ' ';
	if (as != NULL && as->string != NULL && as->length > 0) {
		for (i = 0; i < as->length && (as->string)[i] != '\0'; i++) {
			if(islower((as->string)[i]) && !isalnum(last)) {
				(as->string)[i] = toupper((as->string)[i]);
			}
			else if(isupper((as->string)[i]) && isalnum(last)) {
				(as->string)[i] = tolower((as->string)[i]);
			}
			last = (as->string)[i];
		}
		astr_update(as);
	}
	return as;
}

/*
 * astr_left_trim
 *
 * Trim the left (leading) spaces of the astr string.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_left_trim(astr *as) {
	char *d;
	char *s;
	char *end;
	if (as != NULL && as->string != NULL && as->length > 0) {
		d = s = as->string;
		end = as->string + as->length;
		while(s < end && isspace(*s)) {
			s++;
		}
		if (d < s) {
			as->length -= (s - d);
			while(s < end && *s != '\0' ) {
				*d++ = *s++;
			}
			while (d < end) {
				*d++ = '\0';
			}
		}
		if(s > as->string) {
			astr_update(as);
		}
	}
	return as;
}

/*
 * astr_right_trim
 *
 * Trim the right (trailing) spaces of the astr string.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_right_trim(astr *as) {
	char *s;
	char *beg;
	char *end;
	if (as != NULL && as->string != NULL && as->length > 0) {
		s = as->string + as->length - 1;
		beg = as->string;
		end = as->string + as->length;
		while(s >= beg && *s == '\0') {
			s--;
		}
		while(s >= beg && isspace(*s)) {
			*s-- = '\0';
		}
		if (s < (end - 1)) {
			astr_update(as);
		}
	}
	return as;
}

/*
 * astr_trim
 *
 * Trim the left and right spaces of the astr string.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_trim(astr *as) {
	as = astr_left_trim(as);
	as = astr_right_trim(as);
	return as;
}

/*
 * astr_pack
 *
 * Replace multiple space characters in the astr string with one space character.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_pack(astr *as) {
	char *d;
	char *s;
	char *end;
	if (as != NULL && as->string != NULL && as->length > 0) {
		d = s = as->string;
		end = as->string + as->length;
		while (*s != '\0' && s < end) {
			if (isspace(*s)) {
				*d++ = *s++;
				while(isspace(*s) && s < end) {
					s++;
				}
			}
			else {
				*d++ = *s++;
			}
		}
		*d = '\0';
		if (s < (end - 1)) {
			astr_update(as);
		}
	}
	return as;
}

/*
 * astr_clean
 *
 * Trim and pack the spaces in the astr string.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_clean(astr *as) {
	as = astr_trim(as);
	as = astr_pack(as);
	return as;
}

/*
 * astr_not_empty
 *
 * Make sure the astr string is not empty (or NULL), use space for the single character.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_not_empty(astr *as) {
	as = astr_not_empty_char(as, ' ');
	return as;
}

/*
 * astr_not_empty_char
 * 
 * Make sure the astr string is not empty (or NULL), use the specified single character.
 *
 * Parameter: The astr instance to be edited
 * Parameter: The character to use as the sole character if necessary
 * Returns:   Pointer to the astr instance
 */
astr *astr_not_empty_char(astr *as, const char sole) {
	char sole_str[2] = {' ', '\0'};
	if (as != NULL) {
		if (as->string == NULL || as->length <= 0) {
			sole_str[0] = sole;
			as = astr_set(as, sole_str);
		}
		astr_update(as);
	}
	return as;
}

/*
 * astr_reverse
 *
 * Reverse the characters in an astr string.
 *
 * Parameter: The astr instance to be edited
 * Returns:   Pointer to the astr instance
 */
astr *astr_reverse(astr *as) {
	char *b, *e;
	char x;
	if (as != NULL && as->length > 1) {
		b = as->string;
		e = as->string + as->length - 1;
		while (b < e) {
			x = *b;
			*b = *e;
			*e = x;
			b++;
			e--;
		}
	}
	return as;
}
