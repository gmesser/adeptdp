// astr_utilities.c - Adept String Utilities

/*
 * General-purpose utility functions.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "astr.h"

static char *dump_buffer(char *dst, size_t dstlen, const void *src, size_t srclen);

/*
 * astr_update
 *
 * Update the checksum and length of an astr instance.
 * Call this when the string member is modified using a function from outside
 * the astr module that does not update these fields.
 *
 * Parameter: The astr instance
 * Returns:   Pointer to the astr instance
 */
astr *astr_update(astr *as) {
	char *p;
	int cs = 0;
	int len = 0;

	if (as != NULL) {
		if (as->string != NULL) {
			for (p = as->string; *p != '\0'; p++) {
				cs += *p;
				len++;
			}
		}
		as->checksum = cs;
		as->length = len;
	}
}

/*
 * astr_print
 *
 * Print an astr instance in labeled string format.
 */
char *astr_print(const astr *as) {
	char *struct_print;
	char *string_print;
	char *print;
	char eol[3];

	struct_print = astr_print_struct(as);
	string_print = astr_print_string(as);
	sprintf(eol, "%s", "\n");
	print = calloc(strlen(struct_print) + strlen(eol) + strlen(string_print) + strlen(eol) + 1, sizeof(char));
	if (print != NULL) {
		strcpy(print, struct_print);
		strcat(print, eol);
		strcat(print, string_print);
		strcat(print, eol);
	}
	free(struct_print);
	free(string_print);
	return print;
}

/*
 * astr_print_string
 *
 * Print the string in an astr instance.
 */
char *astr_print_string(const astr *as) {
	char *lbl_string =           "String val:  ";
	int length;
	char *string = NULL;

	if (as != NULL && as->string != NULL) {
		length = strlen(lbl_string);
		length += strlen(as->string) + 3;

		string = calloc(length, sizeof(char));
		sprintf(string, "%s\"%s\"", lbl_string, as->string);
	}
	return string;
}

/*
 * astr_print_struct
 *
 * Print an astr structure.
 * Label: NNNNNNNNN\n
 */
char *astr_print_struct(const astr *as) {
	char *lbl_string =           "String ptr:  ";
	char *lbl_checksum =         "Checksum:    ";
	char *lbl_length =           "Length:      ";
	char *lbl_allocated_length = "Alloc. len.: ";
	int length;
	char *string = NULL;
	
	if (as != NULL) {
		length = strlen(lbl_string) + strlen(lbl_checksum) + strlen(lbl_length) + strlen(lbl_allocated_length);
		length += 12 + 10 + 10 + 10;
		string = calloc(length, sizeof(char));
		sprintf(string, "%s%p\n%s%d\n%s%d\n%s%d",
			lbl_string, as->string, lbl_checksum, as->checksum, 
			lbl_length, as->length, lbl_allocated_length, as->allocated_length);
	}
	return string;
}

/*
 * astr_hexdump
 *
 * Dump an astr instance in hex dump format.
 * This function dumps the structure and the string.
 *
 * Parameter: The astr instance
 * Returns:	  The address of the null-terminated destination buffer.
 */
char *astr_hexdump(const astr *as) {
	char *struct_dump;
	char *string_dump;
	char *dump = NULL;
	char eol[3];

	if (as != NULL) {
		struct_dump = astr_hexdump_struct(as);
		string_dump = astr_hexdump_string(as);
		sprintf(eol, "%s", "\n");
		dump = calloc(strlen(struct_dump) + strlen(eol) + strlen(string_dump) + strlen(eol) + 1, sizeof(char));
		if (dump != NULL) {
			strcpy(dump, struct_dump);
			strcat(dump, eol);
			strcat(dump, string_dump);
			strcat(dump, eol);
		}
		free(struct_dump);
		free(string_dump);
	}
	return dump;
}

/*
 * astr_hexdump_string
 *
 * Dump the string in an astr instance in hex dump format.
 *
 * Parameter: The astr instance
 * Returns:	  The address of the null-terminated destination buffer.
 */
char *astr_hexdump_string(const astr *as) {
	char *dumpbuffer = NULL;
	int  dumpbufferlen;

	if (as != NULL && as->string != NULL) {
		// Calculate the size of the buffer required to hold the hex dump.
		dumpbufferlen = (74 * ((as->allocated_length / 16) + (as->allocated_length % 16))) + 1;

		// Allocate the buffer to hold the hex dump.
		dumpbuffer = (char *) calloc(dumpbufferlen, sizeof(char));

		// If the allocation worked, write the hex dump into the input buffer.
		if(dumpbuffer != NULL) {
			dump_buffer(dumpbuffer, dumpbufferlen, as->string, as->allocated_length);
		}
	}

	return dumpbuffer;
}

/*
 * astr_hexdump_struct
 *
 * Dump an astr structure instance in hex dump format.
 *
 * Parameter: The astr instance
 * Returns:	  The address of the null-terminated destination buffer.
 */
char *astr_hexdump_struct(const astr *as) {
	char *dumpbuffer = NULL;
	int  dumpbufferlen;

	if (as != NULL) {
		// Calculate the size of the buffer required to hold the hex dump.
		dumpbufferlen = (74 * ((sizeof(as) / 16) + (sizeof(as) % 16))) + 1;

		// Allocate the buffer to hold the hex dump.
		dumpbuffer = (char *) calloc(dumpbufferlen, sizeof(char));

		// If the allocation worked, write the hex dump into the input buffer.
		if(dumpbuffer != NULL)
		{
			dump_buffer(dumpbuffer, dumpbufferlen, as, sizeof(astr));
		}
	}

	return dumpbuffer;
}

/*
 * dump_buffer
 *
 * Dump a buffer.
 *
 * This function dumps a buffer in the familiar dump format of DEBUG,
 * with 16 hexadecimal values per line on the left and 16 ASCII
 * characters per line on the right.  Unprintable ASCII characters show
 * as '.'.
 *
 * The destination buffer must be large enough to contain the dumped
 * bytes.  The size of the destination buffer can be found using the
 * following equation:
 *
 * 			(74 * ((buflen / 16) + (buflen % 16))) + 1
 *
 * Output:
 *
 * 00000  41 20 6C 6F 6E 67 20 73 74 72 69 6E 67 20 74 68  A long string th
 * 00010  61 74 20 69 73 20 6D 75 63 68 20 6C 6F 6E 67 CC  at is much long.
 * ----=----1----=----2----=----3----=----4----=----5----=----6----=----7----=
 *                                           Buffer with LF eol ends at 73 ^
 *                                         Buffer with CR/LF eol ends at 74 ^
 *
 * Parameter: dst    - address of the destination buffer
 * Parameter: dstlen - length of the destination buffer
 * Parameter: src     - address of the buffer to be dumped
 * Parameter: srclen  - length of the buffer to be dumped
 * Returns:	  The address of the null-terminated destination buffer.
 * ------------------------------------------------------------------------------
 */

static char *dump_buffer(char *dst, size_t dstlen, const void *src, size_t srclen) {
	int i;
	int count_out;
	int offset = 0;
	char eol[3];
	int eollen = 2;
	char temp[6];
	short val;
	char *pdst = 0;
	char *pend = 0;

	struct
	{
		char offset[5];
		char filler1[2];
		struct
		{
			char hexval[2];
			char space;
		} hexvalues[16];
		char filler2;
		char ASCIIval[16];
	} dumpline;

	if (src == NULL || srclen <= 0 || dst == NULL) {
		return NULL;
	}

	sprintf(eol, "\n");
	eollen = strlen(eol);

	pdst = dst;
	pend = pdst + dstlen;

	count_out = 0;

	while(count_out < srclen)
	{
		memset((char *) &dumpline, ' ', sizeof(dumpline));

		sprintf(temp, "%5.5X", offset);
		memcpy(dumpline.offset, temp, strlen(temp));

		// Dump sixteen bytes on this line...
		for(i = 0; i < 16 && count_out < srclen; i++, count_out++, offset++) {
			val = ((char *)src)[count_out];

			val &= 0xFF;

			sprintf(temp, "%2.2X", val);
			memcpy(dumpline.hexvalues[i].hexval, temp, strlen(temp));

			dumpline.hexvalues[i].space = ' ';

			if(isprint(val)) {
				dumpline.ASCIIval[i] = val;
			}
			else {
				dumpline.ASCIIval[i] = '.';
			}
		}

		if(pdst + sizeof(dumpline) < pend) {
			memcpy(pdst, (char *) &dumpline, sizeof(dumpline));
			pdst += sizeof(dumpline);
		}
		else {
			if(pend - pdst > 0) {
				memset(pdst, ' ', pend - pdst);
			}
			pdst = pend;
		}

		if(pdst + eollen < pend) {
			memcpy(pdst, eol, eollen);
			pdst += eollen;
		}
		else {
			if(pend - pdst > 0) {
				memset(pdst, ' ', pend - pdst);
			}

			pdst = pend;
		}
	}

	if(pdst < pend) {
		*pdst = '\0';
	}
	else {
		pdst = pend - 1;
		*pdst = '\0';
	}

	return dst;
}
