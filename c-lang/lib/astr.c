// astr.c - Adept String

/*
 * An astr instance is optimized for comparisons.  It maintains a simple
 * integer checksum that it uses to quickly detect when strings do not match.
 * It accumulates the integer checksum while the instance is being created.
 * This is fast, and accumulating the checksum does not slow down creation too
 * much.  A hash would have been even better at detecting non-matches, with
 * less false matches, but it takes longer to calculate the hash during
 * creation and would still require a full comparison to verify that the
 * strings are a real match when the hashes match.
 */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "astr.h"

static const char space_char = ' ';
static astr *astr_allocate_string(astr *as, size_t length);
static astr *astr_reallocate_string(astr *as, size_t length);
static astr *astr_clear(astr *as);
static astr *astr_free_string(astr *as);

/*
 * astr_create_empty
 *
 * Create an empty new astr instance.
 *
 * Returns:   Pointer to the astr instance
 */
astr *astr_create_empty() {
	astr *as = (astr *)calloc(1, sizeof(astr));
	return as;
}

/*
 * astr_copy
 *
 * Create a new astr instance that is copied from an existing astr instance.
 *
 * Parameter: The source astr instance
 * Returns:   Pointer to the astr instance
 */
astr *astr_copy(const astr *src) {
	astr *as = NULL;
	if (src != NULL) {
		as = astr_create(src->string);
	}
	return as;
}

/*
 * astr_create
 *
 * Create a new astr instance with contents from a string.
 * 
 * Parameter: The source null-terminated string
 * Returns:   Pointer to the astr instance
 */
astr *astr_create(const char *string) {
	astr *as = astr_create_empty();
	return astr_set(as, string);
}

/*
 * astr_create_from_buffer
 *
 * Create a new astr instance with contents from a buffer.
 *
 * Parameter: The source buffer
 * Parameter: The length of the source buffer
 * Returns:   Pointer to the astr instance
 */
astr *astr_create_from_buffer(const char *buffer, const int length) {
	astr *as = astr_create_empty();
	return astr_set_from_buffer(as, buffer, length);
}

/*
 * astr_printf
 *
 * Create a new astr with contents from a call to vsnprintf with the specified format and arguments.
 *
 * Parameter: The printf format string
 * Parameter: The argument list for printf
 * Returns:   Pointer to the astr instance
 */
astr *astr_printf(const char *fmt, ...) {
	char buff[256];
	char *newbuff = NULL;
	int count, count2;
	astr *as = NULL;
	va_list ap;
	va_list ap2;

	va_start(ap, fmt);
	count = vsnprintf(buff, sizeof(buff), fmt, ap);
	if (count < 256) {
		as = astr_create(buff);
		va_end(ap);
	}
	else {
		va_start(ap2, fmt);
		newbuff = malloc(count + 1);
		count2 = vsnprintf(newbuff, count + 1, fmt, ap2);
		as = astr_create(newbuff);
		va_end(ap2);
		free(newbuff);
	}

	return as;
}

/*
 * astr_tok
 *
 * Create a new astr with contents from one token at a time from an astr
 * instance, delimited by delims.
 *
 * The delimiters can be different for each call to astr_tok.
 * Once you start extracting tokens from an astr instance, subsequent calls
 * will continue extracting tokens.
 *
 * Just like the standard C library function strtok, a null byte is written
 * to the source astr's string at the end of each token.  You will need to copy
 * the original astr instance if you need it in its original form later.
 *
 * Parameter: The astr instance  to be tokenized
 * Parameter: The string containing the delimiters
 * Returns:   Pointer to the new astr instance
 */
astr *astr_tok(astr *as, char *delims) {
	char *default_delims = " \t\r\n";
	astr *atok = NULL;

	if (as != NULL) {
		if (delims == NULL) {
			// Use the default delimiters.
			delims = default_delims;
		}

		if (as->tokenend == NULL) {
			// This is the first call for this astr, start at the beginning.
			as->tokenend = as->string;
		}

		if(*(as->tokenend) != '\0') {
			// We did not reach the end on the last call and/or the string ahead is not empty.
			// Skip any leading delimiters.
			as->tokenend += strspn(as->tokenend, delims);
			if(*(as->tokenend) != '\0') {
				// There is more string ahead to tokenize.
				char *tok = as->tokenend;
				// Find the next delimiter.
				as->tokenend += strcspn(as->tokenend, delims);
				if (*(as->tokenend) != '\0') {
					// This token ended before the end of the string.
					// Write '\0', set up for the next call.
					*(as->tokenend) = '\0';
					as->tokenend++;
				}
				// Make the new astr instance with this token.
				atok = astr_create(tok);
			}
		}
		astr_update(as);
	}

	return atok;
}

/*
 * astr_split
 *
 * Create an array of new astr instances with contents from all the tokens from 
 * an astr instance, delimited by delims.
 *
 * Just like the standard C library function, strtok, a null byte is written
 * to the source astr's string.  You will need to copy the original astr
 * instance if you need it in its original form later.
 *
 * Parameter: The astr instance  to be tokenized
 * Parameter: The string containing the delimiters
 * Returns:   Pointer to an array of astr pointers.  The last array element
 *            will be a NULL pointer for a terminator.
 */
astr **astr_split(astr *as, char *delims) {
	int numelements = 1;
	astr **asa = (astr**) malloc(numelements * sizeof(astr *));

	for(astr *atok = astr_tok(as, delims); atok != NULL; atok = astr_tok(as, delims)) {
		numelements++;
		asa = (astr**) realloc(asa, numelements);
		asa[numelements - 2] = astr_copy(atok);
	}

	asa[numelements - 1] = NULL;

	return asa;
}

/*
 * astr_set
 *
 * (Re)initialize an astr instance with contents from a string.
 *
 * Parameter: The astr instance to be reinitialized
 * Parameter: The source null-terminated string
 * Returns:   Pointer to the astr instance
 */
astr *astr_set(astr *as, const char *string) {
	const char *s;

	if (as == NULL) {
		as = (astr *)calloc(1, sizeof(astr));
	}

	if (as != NULL) {
		astr_clear(as);

		if (string == NULL) {
			return as;
		}

		astr_reallocate_string(as, strlen(string));
		if (as->string == NULL) {
			return as;
		}

		s = string;
		while (*s != '\0') {
			as->length++;
			as->checksum += *s++;
		}
		strcpy(as->string, string);
	}
	return as;
}

/*
 * astr_set_from_buffer
 *
 * (Re)initialize an astr instance with contents from a buffer.
 *
 * The string will be right-filled with spaces to the length specified if needed.
 *
 * Parameter: The astr instance to be reinitialized
 * Parameter: The source buffer
 * Parameter: The length of the source buffer
 * Returns:   Pointer to the astr instance
 */
astr *astr_set_from_buffer(astr *as, const char *buffer, const int length) {
	const char *s = buffer;
	const char *s_end = buffer + length;
	char *d;
	const char *d_end;
	
	if (as == NULL) {
		as = (astr *)calloc(1, sizeof(astr));
	}
	
	if (as != NULL) {
		astr_clear(as);

		if (buffer == NULL) {
			return as;
		}

		astr_reallocate_string(as, length);
		if (as->string == NULL) {
			return as;
		}

		d = as->string;
		d_end = as->string + length;
		while (*s != '\0' && s < s_end && d < d_end) {
			as->length++;
			as->checksum += *s;
			*d++ = *s++;
		}
		while (d < d_end) {
			as->length++;
			as->checksum += space_char;
			*d++ = space_char;
		}
	}
	return as;
}

/*
 * astr_append
 *
 * Append a string to an astr instance.
 *
 * Parameter: The astr instance to be appended to
 * Parameter: The source null-terminated string to be appended
 * Returns:   Pointer to the astr instance
 */
astr *astr_append(astr *as, const char *string) {
	const char *s;
	char *d;
	int newstringlen;
	if (as != NULL && string != NULL) {
		newstringlen = as->length + strlen(string);
		astr_reallocate_string(as, newstringlen);
		if (as->string == NULL) {
			return as;
		}

		d = as->string + strlen(as->string);
		s = string;
		while (*s != '\0') {
			as->length++;
			as->checksum += *s;
			*d++ = *s++;
		}
	}
	return as;
}

/*
 * astr_append_buffer
 *
 * Append a buffer to an astr instance.
 *
 * The string will be right-filled with spaces to the length specified if needed.
 *
 * Parameter: The astr instance to be reinitialized
 * Parameter: The source buffer
 * Parameter: The length of the source buffer
 * Returns:   Pointer to the astr instance
 */
astr *astr_append_buffer(astr *as, const char *buffer, const int length) {
	const char *s;
	const char *s_end;
	char *d;
	const char *d_end;
	int newstringlen;
	if (as != NULL && buffer != NULL && length > 0) {
		newstringlen = as->length + length;
		astr_reallocate_string(as, newstringlen);
		if (as->string == NULL) {
			return as;
		}

		d = as->string + strlen(as->string);
		d_end = as->string + as->length + length;
		s = buffer;
		s_end = buffer + length;
		while (*s != '\0' && s < s_end && d < d_end) {
			as->length++;
			as->checksum += *s;
			*d++ = *s++;
		}

		while (d < d_end) {
			as->length++;
			as->checksum += space_char;
			*d++ = space_char;
		}
	}
	return as;
}

/*
 * astr_allocate_string
 *
 * Allocate the string member of an astr instance.
 *
 * Parameter: The astr instance
 * Parameter: The length of the string member
 * Returns:   Pointer to the astr instance
 */
static astr *astr_allocate_string(astr *as, size_t length) {
	if (as != NULL) {
		if (as->string != NULL) {
			if (length + 1 > as->allocated_length) {
				astr_free_string(as);
				as->string = (char *)calloc(length + 1, sizeof(char));
				as->allocated_length = length + 1;
			}
			else {
				astr_clear(as);
			}
		}
		else {
			astr_clear(as);
			as->string = (char *)calloc(length + 1, sizeof(char));
			as->allocated_length = length + 1;
		}

		if (as->string == NULL) {
			astr_clear(as);
		}

		as->tokenend = NULL;
	}
	return as;
}

/*
 * astr_reallocate_string
 *
 * Reallocate the string member of an astr instance.
 *
 * Parameter: The astr instance
 * Parameter: The new length of the string member
 * Returns:   Pointer to the astr instance
 */
static astr *astr_reallocate_string(astr *as, size_t length) {
	char *newstring = NULL;
	if (as != NULL) {
		if (as->string != NULL) {
			if (length + 1 > as->allocated_length) {
				newstring = (char *)calloc(length + 1, sizeof(char));
				if (newstring != NULL) {
					as->allocated_length = length + 1;
					strcpy(newstring, as->string);
					free(as->string);
					as->string = newstring;
				}
			}
		}
		else {
			astr_allocate_string(as, length);
		}

		as->tokenend = NULL;
	}
	return as;
}

/*
 * astr_clear
 *
 * Clear an astr instance.
 * Keep the storage allocation, set the other members to zero/NULL.
 *
 * Parameter: The astr instance
 * Returns:   Pointer to the astr instance
 */
static astr *astr_clear(astr *as) {
	if (as != NULL) {
		if (as->string != NULL) {
			memset(as->string, '\0', as->allocated_length);
		}
		as->length = 0;
		as->checksum = 0;
		as->tokenend = NULL;
	}
	return as;
}

/*
 * astr_free_string
 *
 * Free the string member of an astr instance.
 * Free the storage allocation, set all members to zero.
 *
 * Parameter: The astr instance
 * Returns:   Pointer to the astr instance
 */
static astr *astr_free_string(astr *as) {
	if (as != NULL) {
		if (as->string != NULL) {
			free(as->string);
		}
		as->string = NULL;
		as->allocated_length = 0;
		as->length = 0;
		as->checksum = 0;
		as->tokenend = NULL;
	}
	return as;
}

/*
 * astr_free
 *
 * Free an astr instance.
 * Frees the allocated memory for the string member, then frees the memory for
 * the astr instance itself.
 *
 * Parameter: The astr instance
 * Returns:   NULL pointer
 */
astr *astr_free(astr *as) {
	if (as != NULL) {
		if (as->string != NULL) {
			free(as->string);
		}
		free(as);
	}
	return NULL;
}
