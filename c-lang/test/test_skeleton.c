// test_skeleton.c - test skeleton

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
char error[256];

// ----------

void test_creation(void) {
	char *str = "ABC";
	char *buf = "DEF   ";
	astr *as_str;
	astr *as_buf;

	as_str = astr_create(str);
	aut_assert("1 test_creation", strcmp(as_str->string, str) == 0);

	as_buf = astr_create_from_buffer(buf, strlen(buf));
	aut_assert("2 test_creation", strcmp(as_buf->string, buf) == 0);

	astr_free(as_str);
	astr_free(as_buf);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_creation);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
