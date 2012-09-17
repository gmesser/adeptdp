// acatfile.c - Adept cat implementation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <regex.h>
#include <assert.h>

#include "astr.h"
#include "afile.h"

int verbose_flag = 0;
int debug_flag = 0;

int cat_file_line(astr *as) {
	printf("%s\n", astr_right_trim(as)->string);
}

// ----------

int cat_file_lines(astr *inputname) {
	char *read_mode = "r";
	size_t buffersize = 10 * 1024;
	int buffermode = _IOLBF;
	astr *open_mode;
	afile *af;
	int result;
	int i;
	int nlines;

	open_mode = astr_create(read_mode);
	af = afile_create_explicit(inputname, open_mode, buffersize, buffermode);
	result = afile_open(af);
	if (result != 0) {
		af = afile_free(af);
		astr_free(open_mode);
		if (debug_flag) {
			assert(result == 0);
		}
		return 0;
	}

	// Process all lines.
	nlines = afile_process_lines(af, cat_file_line);

	result = afile_close(af);
	if (result != 0) {
		af = afile_free(af);
		astr_free(open_mode);
		if (debug_flag) {
			assert(result == 0);
		}
		return 0;
	}

	af = afile_free(af);
	astr_free(open_mode);

	return nlines;
}

// ----------

int main(int argc, char *argv[]) {
	astr *inputname;
	int nlines;
	int i;

	for (i = 1; i < argc; i++) {
		if (argc == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("Usage: acatfile [-h | --help] [-v | --verbose] [-d | --debug] filename [...]\n\n");
			printf("-h or --help: Print this help message and exit without processing any files.");
			printf("-v or --verbose: Turns on printing of verbose messages with the output.\n");
			printf(" The verbose option toggles verbose on or off if it is used more than once.\n");
			printf("-d or --debug: Turns on debug mode where return codes are checked with assert statements.\n");
			return 0;
		}
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
			verbose_flag == 0 ? 1 : 0;
			continue;
		}

		if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0) {
			debug_flag = 1;
			continue;
		}

		if (strlen(argv[i]) > 0) {
			inputname = astr_create(argv[i]);
			if (verbose_flag) {
				printf("acatfile: Processing %s...\n", inputname->string);
			}
			nlines = cat_file_lines(inputname);
			astr_free(inputname);
			if (verbose_flag) {
				printf("acatfile: %s: Printed %d line(s).\n", argv[i], nlines);
			}
		}
	}

	return 0;
}
