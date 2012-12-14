// test_astr_utilities.c - test astr utility functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"
#include "aclock.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
aclock *suite_clock;
int test_runs;
int test_fails;
astr *test_messages;

// ----------

char *str = " \t\n\v\f\r A BCDE FGHI JKLM []{}|\\;:,.<>/?`-=~!@#$%^&*()_+ NOPQ RSTU VWXY Z \t\n\v\f\r ";

void test_astr_calc_checksum(void) {
	astr *as_str;
	int chk;

	as_str = astr_create(str);
	chk = astr_calc_checksum(as_str);
	aut_assert("1 calc checksum", as_str->checksum == chk);
}

void test_astr_hexdump(void) {
	astr *as_str;
	char *dump;

	as_str = astr_create(str);
	aut_assert("1 hexdump", strcmp(as_str->string, str) == 0);

	dump = astr_hexdump(as_str);
	aut_assert("2 hexdump", dump != NULL);
	aut_assert("3 hexdump", strlen(dump) > 0);
/*
	printf("%s", dump);
*/

	free(dump);
	astr_free(as_str);
}

void test_astr_print(void) {
	astr *as_str;
	char *print;

	as_str = astr_create(str);
	aut_assert("1 print", strcmp(as_str->string, str) == 0);

	print = astr_print(as_str);
	aut_assert("2 print", print != NULL);
	aut_assert("3 print", strlen(print) > 0);
/*
	printf("%s", print);
*/

	free(print);
	astr_free(as_str);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_astr_calc_checksum);
	aut_run_test(test_astr_hexdump);
	aut_run_test(test_astr_print);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
