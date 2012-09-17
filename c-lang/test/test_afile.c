// test_afile.c - test the file functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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


// ----------

void test_astr_creation(void) {
	char *str = "ABC";
	char *buf = "DEF   ";
	astr *as_str;
	astr *as_buf;

	as_str = astr_create(str);
	aut_assert("1 test_astr_creation", strcmp(as_str->string, str) == 0);

	as_buf = astr_create_from_buffer(buf, strlen(buf));
	aut_assert("2 test_astr_creation", strcmp(as_buf->string, buf) == 0);

	astr_free(as_str);
	astr_free(as_buf);
}

void test_afile_creation(void) {
	char *name = "test_file.tmp";
	astr *filename;
	afile *af;

	filename = astr_create(name);
	aut_assert("1 test_afile_creation", strcmp(filename->string, name) == 0);

	af = afile_create_explicit(filename, NULL, -1, -1);
	aut_assert("2 test_afile_creation", strcmp(af->filespec->string, name) == 0);

	astr_free(filename);
}

void test_create_file(void) {
	char *name = "test_create_file.tmp";
	char *modes = "a";
	astr *filename;
	astr *open_modes;
	afile *af;
	int result;

	filename = astr_create(name);
	aut_assert("1 test_create_file", strcmp(filename->string, name) == 0);

	open_modes = astr_create(modes);
	aut_assert("2 test_create_file", strcmp(open_modes->string, modes) == 0);

	af = afile_create_explicit(filename, open_modes, 0, -1);
	aut_assert("3 test_create_file", strcmp(af->filespec->string, name) == 0);

	result = afile_open(af);
	aut_assert("4 test_create_file", result == 0);

	result = afile_close(af);
	aut_assert("5 test_create_file", result == 0);

	af = afile_free(af);
	aut_assert("6 test_create_file", af == NULL);

	unlink(name);

	astr_free(filename);
	astr_free(open_modes);
}

void test_write(void) {
	char *name = "test_write.tmp";
	char *modes = "a";
	astr *filename;
	astr *open_modes;
	afile *af;
	int result;
	int i;

	filename = astr_create(name);
	aut_assert("1 test_write", strcmp(filename->string, name) == 0);

	open_modes = astr_create(modes);
	aut_assert("2 test_write", strcmp(open_modes->string, modes) == 0);

	af = afile_create(filename, open_modes);
	aut_assert("3 test_write", strcmp(af->filespec->string, name) == 0);

	result = afile_open(af);
	aut_assert("4 test_write", result == 0);

	for (i = 0; i < 5; i++) {
		fprintf(af->file, "%s\n", content_lower);
		fprintf(af->file, "%s\n", content_upper);
		fprintf(af->file, "%s\n", content_mixed);
	}

	result = afile_close(af);
	aut_assert("5 test_write", result == 0);

	af = afile_free(af);
	aut_assert("6 test_write", af == NULL);

	unlink(name);

	astr_free(filename);
	astr_free(open_modes);
}

void test_process_lines(void) {
	char *name = "test_write_read.tmp";
	char *append_modes = "a";
	char *read_modes = "r";
	astr *filename;
	astr *open_modes;
	afile *af;
	int result;
	int i;

	filename = astr_create(name);
	aut_assert("1 test_write_read", strcmp(filename->string, name) == 0);

	open_modes = astr_create(append_modes);
	aut_assert("2 test_write_read", strcmp(open_modes->string, append_modes) == 0);

	af = afile_create_explicit(filename, open_modes, 0, -1);
	aut_assert("3 test_write_read", strcmp(af->filespec->string, name) == 0);

	result = afile_open(af);
	aut_assert("4 test_write_read", result == 0);

	for (i = 0; i < 5; i++) {
		fprintf(af->file, "%s\n", content_lower);
		fprintf(af->file, "%s\n", content_upper);
		fprintf(af->file, "%s\n", content_mixed);
	}

/*
	printf("%s\n", afile_print(af));
*/

	result = afile_close(af);
	aut_assert("5 test_write_read", result == 0);

	open_modes = astr_set(open_modes, read_modes);
	aut_assert("6 test_write_read", strcmp(open_modes->string, read_modes) == 0);

	afile_set_open_modes(af, open_modes);

	result = afile_open(af);
	aut_assert("7 test_write_read", result == 0);

/*
	while(fgets(af->buffer, 27, af->file) != NULL) {
		printf("%s", af->buffer);
	}
*/

/*
	printf("%s\n", afile_print(af));
*/

	result = afile_close(af);
	aut_assert("8 test_write_read", result == 0);

	result = unlink(af->filespec->string);
	aut_assert("9 test_write_read", result == 0);

	af = afile_free(af);
	aut_assert("10 test_test_write_readwrite", af == NULL);

	astr_free(filename);
	astr_free(open_modes);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_astr_creation);
	aut_run_test(test_afile_creation);
	aut_run_test(test_create_file);
	aut_run_test(test_write);
	aut_run_test(test_process_lines);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
