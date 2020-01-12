// astr_classifications.c - Adept String Classifications

/*
 * Functions to determine if the contents of an astr match various
 * classifications.
 * Most of these functions use the regex library.
 */

#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "astr.h"

/*
 * astr_is_empty
 *
 * Determine if the astr instance is empty.
 * Empty is defined as having zero length or possibly containing only '\0' characters.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if empty and 0 if not empty.
 */
int astr_is_empty(const astr *as) {
   int result = 1;
   if (as != NULL && as->string != NULL && (as->length > 0 || strlen(as->string) > 0)) {
		result = 0;
   }
   return result;
}

/*
 * astr_is_blank
 *
 * Determine if the astr instance is blank.
 * Blank is defined as having length > 0 and containing only spaces and '\t'.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if blank and 0 if not blank.
 */
int astr_is_blank(const astr *as) {
	int result;
	astr *expression = astr_create("^[[:blank:]]+$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

/*
 * astr_is_space
 *
 * Determine if the astr instance is space.
 * Space is defined as having length > 0 and containing only spaces, or '\t', '\r', '\n', '\v', or '\f' characters.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if blank and 0 if not space.
 */
int astr_is_space(const astr *as) {
	int result;
	astr *expression = astr_create("^[[:space:]]+$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

/*
 * astr_is_alphabetic
 *
 * Determine if the astr instance contains alphabetic characters only.
 * Example: AbcdEfg
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if the string contains alphabetic characters only, and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_is_alphabetic(const astr *as) {
	int result;
	astr *expression = astr_create("^[[:alpha:]]+$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

/*
 * astr_is_alphanumeric
 *
 * Determine if the astr instance contains alphanumeric characters only.
 * Example: 1A2B3C
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if the string contains alphanumeric characters only, and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_is_alphanumeric(const astr *as) {
	int result;
	astr *expression = astr_create("^[[:alnum:]]+$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

/*
 * astr_is_yn
 *
 * Determine if the astr instance contains Y or N.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if the string equals "y", "yes", "n", or "no" (case-insensitive), and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_is_yn(const astr *as) {
	int result;
	astr *expression = astr_create("^([yn]|yes|no)$");
	result = astr_match(as, expression, REG_EXTENDED | REG_ICASE);
	astr_free(expression);
	return result;
}

/*
 * astr_is_tf
 *
 * Determine if the astr instance contains true or false.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if the string equals "t", "true", "f", or "false" (case-insensitive), and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_is_tf(const astr *as) {
	int result;
	astr *expression = astr_create("^([tf]|true|false)$");
	result = astr_match(as, expression, REG_EXTENDED | REG_ICASE);
	astr_free(expression);
	return result;
}

/*
 * astr_is_10
 *
 * Determine if the astr instance contains 1 or 0.
 *
 * Parameter: The astr instance to be checked
 * Returns    1 if the string equals "1" or "0", and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_is_10(const astr *as) {
	int result;
	astr *expression = astr_create("^[10]$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

/*
 * astr_match
 *
 * Determine if the astr instance matches a regular expression.
 * Call the regex engine.
 *
 * Parameter: The astr instance to be checked
 * Parameter: The astr containing the regex string
 * Parameter: The POSIX cflags to be used to control the matching
 * Returns    1 if the astr matches the expression, and 0 if not.
 *				  -1 if there was an error with the regular expression.
 */
int astr_match(const astr *as, const astr *expression, int posix_cflags) {
	regex_t regex;
	int errc;
	int match;
	errc= regcomp(&regex, expression->string, posix_cflags);
	if (errc) {
		return -1;
	}
	match = regexec(&regex, as->string, 0, NULL, 0);
	regfree(&regex);
	return !match;
}

