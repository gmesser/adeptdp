// test_afile_process.c - test the file processing functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <regex.h>

#include "astr.h"
#include "afile.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
int test_runs;
int test_fails;
astr *test_messages;
char error[256];

char *content_lower = "abcdefghijklmnopqrstuvwxyz";
char *content_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *content_mixed = "AbcdeFghijKlmnopQrstuvWxyz";

int cat_line(astr *as) {
	as = astr_to_upper_case(as);
	return 0;
}

int match_line_lowercase(astr *as) {
	int result;
	astr *expression = astr_create("^[a-z\r\n]+$");
	result = astr_match(as, expression, REG_EXTENDED);
	astr_free(expression);
	return result;
}

// ----------

void test_process_lines(void) {
	char *name = "test_process_lines.tmp";
	char *append_modes = "a";
	char *read_modes = "r";
	astr *filename;
	astr *open_modes;
	afile *af;
	int result;
	int i;
	int nlines;

	filename = astr_create(name);
	aut_assert("1 test_process_lines", strcmp(filename->string, name) == 0);

	open_modes = astr_create(append_modes);
	aut_assert("2 test_process_lines", strcmp(open_modes->string, append_modes) == 0);

	af = afile_create(filename, open_modes);
	aut_assert("3 test_process_lines", strcmp(af->filespec->string, name) == 0);

	result = afile_open(af);
	aut_assert("4 test_process_lines", result == 0);

	for (i = 0; i < 5; i++) {
		fprintf(af->file, "%s\n", content_lower);
		fprintf(af->file, "%s\n", content_upper);
		fprintf(af->file, "%s\n", content_mixed);
	}

/*
	printf("%s\n", afile_print(af));
*/

	result = afile_close(af);
	aut_assert("5 test_process_lines", result == 0);

	open_modes = astr_set(open_modes, read_modes);
	aut_assert("6 test_process_lines", strcmp(open_modes->string, read_modes) == 0);

	afile_set_open_modes(af, open_modes);

	result = afile_open(af);
	aut_assert("7 test_process_lines", result == 0);

	// Process all lines.
	nlines = afile_process_lines(af, cat_line);
	aut_assert("8 test_process_lines", nlines == 15);

	result = afile_close(af);
	aut_assert("9 test_process_lines", result == 0);

	result = unlink(af->filespec->string);
	aut_assert("10 test_process_lines", result == 0);

	af = afile_free(af);
	aut_assert("11 test_process_lines", af == NULL);

	astr_free(filename);
	astr_free(open_modes);
}

void test_process_matching_lines(void) {
	char *name = "test_process_matching_lines.tmp";
	char *append_modes = "a";
	char *read_modes = "r";
	astr *filename;
	astr *open_modes;
	afile *af;
	int result;
	int i;
	int nlines;

	filename = astr_create(name);
	aut_assert("1 test_process_matching_lines", strcmp(filename->string, name) == 0);

	open_modes = astr_create(append_modes);
	aut_assert("2 test_process_matching_lines", strcmp(open_modes->string, append_modes) == 0);

	af = afile_create_explicit(filename, open_modes, 0, -1);
	aut_assert("3 test_process_matching_lines", strcmp(af->filespec->string, name) == 0);

	result = afile_open(af);
	aut_assert("4 test_process_matching_lines", result == 0);

	for (i = 0; i < 5; i++) {
		fprintf(af->file, "%s\n", content_lower);
		fprintf(af->file, "%s\n", content_upper);
		fprintf(af->file, "%s\n", content_mixed);
	}

/*
	printf("%s\n", afile_print(af));
*/

	result = afile_close(af);
	aut_assert("5 test_process_matching_lines", result == 0);

	open_modes = astr_set(open_modes, read_modes);
	aut_assert("6 test_process_matching_lines", strcmp(open_modes->string, read_modes) == 0);

	afile_set_open_modes(af, open_modes);

	result = afile_open(af);
	aut_assert("7 test_process_matching_lines", result == 0);

	// Process matching lines.
	nlines = afile_process_matching_lines(af, match_line_lowercase, cat_line);
	aut_assert("8 test_process_matching_lines", nlines == 5);

	result = afile_close(af);
	aut_assert("9 test_process_matching_lines", result == 0);

	result = unlink(af->filespec->string);
	aut_assert("10 test_process_matching_lines", result == 0);

	af = afile_free(af);
	aut_assert("11 test_process_matching_lines", af == NULL);

	astr_free(filename);
	astr_free(open_modes);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_process_lines);
	aut_run_test(test_process_matching_lines);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
