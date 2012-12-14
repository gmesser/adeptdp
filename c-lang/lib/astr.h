// astr.h - Adept String

#ifndef ASTR_H_INCLUDED
#define ASTR_H_INCLUDED

#include <stdio.h>

/*
 * The astr object is optimized for retrieving the length of the string
 * in the astr instance and for detecting inequality of the strings in
 * two astr instances.  It also provides a mechanism for constructing
 * a null-terminated string from a character buffer of known length.
 * 
 * Getting the length is a constant-time operations, regardless of the
 * length of the strings in the structure.
 * 
 * Comparing, when the strings in the two astr instances are unequal,
 * is a constant-time operation, regardless of the length of the
 * strings. 
 * 
 * Comparing, when the strings in the two astr instances are equal, or
 * when the strings are unequal but the two checksums happen to match,
 * takes the same time as strcmp(), plus one integer comnparison of the
 * checksums.
 *
 * An astr instance can be constructed either from a buffer of a
 * specified length, or from a null-terminated string whose length will
 * be determined during construction.  
 *
 * Construction takes longer than strcpy() or memcpy() because the
 * checksum is calculated during construction.
 *
 * The standard C library string handling functions are not redefined here.
 * Use the standard C library functions to manipulate the string inside the
 * astr object.
 *
 * No effort has been made to make this module thread-safe.
 */

typedef struct astr {
	char *string;
	int checksum;
	int length;
	int allocated_length;
} astr;

#ifdef	__cplusplus
extern "C" {
#endif

// ----------------------
// Creation and
// Memory Management

// Allocate a new, empty astr.
astr *astr_create_empty();

// Allocate a new astr that is a copy of an existing astr.
astr *astr_copy(const astr *src);

// Allocate a new astr initialized with a string.
astr *astr_create(const char *string);

// Allocate a new astr initialized with a buffer of specified length.
astr *astr_create_from_buffer(const char *buffer, const int length);

// Allocate a new astr initialized by a call to vsnprintf.
astr *astr_printf(const char *fmt, ...);

// Reinitialize an astr with a string.
astr *astr_set(astr *as, const char *string);

// Reinitialize an astr with a buffer of specified length.
astr *astr_set_from_buffer(astr *as, const char *buffer, const int length);

// Append a string to an astr instance.
astr *astr_append(astr *as, const char *string);

// Append a buffer to an astr instance.
astr *astr_append_buffer(astr *as, const char *buffer, const int length);

// Free an astr instance.
astr *astr_free(astr *as);

// ----------------------
// Classifications

// Determine if the astr instance is empty.
int astr_is_empty(const astr *as);

// Determine if the astr instance is blank.
int astr_is_blank(const astr *as);

// Determine if the astr instance is space.
int astr_is_space(const astr *as);

// Determine if the astr instance matches 'y', 'n', "yes", or "no", case insensitive.
int astr_is_yn(const astr *as);

// Determine if the astr instance matches '1' or '0'.
int astr_is_10(const astr *as);

// Determine if the astr instance matches 't', 'f', "true", or "false", case insensitive.
int astr_is_tf(const astr *as);

// Determine if the astr instance matches :alpha:.
int astr_is_alphabetic(const astr *as);

// Determine if the astr instance matches :alnum:.
int astr_is_alphanumeric(const astr *as);

// Determine if the astr matches the specified regular expression.
int astr_match(const astr *as, const astr *expression, int posix_cflags);

// ----------------------
// Comparisons

// Quickly determine if two astr instances are equal.
int astr_equals(const astr *as1, const astr *as2);

// Determine if the prefixes of two astr instances are equal.
int astr_prefix_equals(const astr *as1, const astr *as2, const int num_prefix_chars);

// Determine if one astr instance is less than another.
int astr_less_than(const astr *as1, const astr *as2);

// Determine if one astr instance is greater than another.
int astr_greater_than(const astr *as1, const astr *as2);

// Compare two astr instances.
int astr_compare(const astr *as1, const astr *as2);

// Compare the prefixes of two astr instances.
int astr_prefix_compare(const astr *as1, const astr *as2, const int num_prefix_chars);

// ----------------------
// Conversions

// Convert the astr to a long.
long astr_to_long(const astr *as);

// Convert the astr to a double.
double astr_to_double(const astr *as);

// ----------------------
// Edits

// Convert the astr to upper case.
astr *astr_to_upper_case(astr *as);

// Convert the astr to lower case.
astr *astr_to_lower_case(astr *as);

// Convert the astr to mixed case (Like This Example).
astr *astr_to_mixed_case(astr *as);

// Trim whitespace from left side of an astr.
astr *astr_left_trim(astr *as);

// Trim whitespace from right side of an astr.
astr *astr_right_trim(astr *as);

// Trim whitespace from left and right sides of an astr.
astr *astr_trim(astr *as);

// Replace multiple space characters in the astr with one space character.
astr *astr_pack(astr *as);

// Pack and trim the astr.
astr *astr_clean(astr *as);

// Make sure the astr string is not empty (or NULL), use space for sole character.
astr *astr_not_empty(astr *as);

// Make sure the astr string is not empty (or NULL), use specified sole character.
astr *astr_not_empty_char(astr *as, const char sole);

// ----------------------
// Utility

// Calculate the checksum of an astr instance.
int astr_calc_checksum(const astr *as);

// Dump an astr instance in hex dump format.
char *astr_hexdump(const astr *as);

// Dump the string in an astr instance.
char *astr_hexdump_string(const astr *as);

// Dump the astr structure.
char *astr_hexdump_struct(const astr *as);
	
// Print an astr instance in labeled string format.
char *astr_print(const astr *as);
	
// Print the string in an astr instance.
char *astr_print_string(const astr *as);
	
// Print an astr structure.
char *astr_print_struct(const astr *as);
	
#ifdef	__cplusplus
}
#endif

#endif // ASTR_H_INCLUDED
