// test_aclock.c - test the clock functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "astr.h"
#include "afile.h"
#include "aclock.h"
#include "adept_unit_test.h"

aclock *suite_clock;
astr *suite_messages;
int suite_runs;
int suite_fails;
int test_runs;
int test_fails;

// ----------

void spend_time() {
	char *str = "STR";
	astr *as;
	int i;

	as = astr_create(str);
	for(i = 0; i < 10000; i++) {
		astr_append(as, str);
	}
	astr_free(as);
}


void aclock_print(aclock *ac) {
	assert(ac != NULL);

	printf(" clock address: %p\n", ac);
	printf(" clock started: %4.9g\n", ac->started);
	printf(" clock stopped: %4.9g\n", ac->stopped);
	printf(" clock elapsed: %4.9g\n", ac->elapsed);
	printf("clock lastloop: %4.9g\n\n", ac->lastloop);
}

void test_aclock_creation(void) {
	aclock *ac = NULL;

	ac = aclock_create();
	aut_assert("0 test_aclock_creation", ac != NULL);
	aut_assert("1 test_aclock_creation", ac->stopped == 0.0);

	aclock_free(ac);
}

void test_aclock_elapsed(void) {
	aclock *ac1;
	aclock *ac2;

	ac1 = aclock_create();
	aut_assert("0 test_aclock_elapsed", ac1 != NULL);
	aut_assert("1 test_aclock_elapsed", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_elapsed", ac1->elapsed == 0.0);

	ac2 = aclock_create();
	aut_assert("3 test_aclock_elapsed", ac2->stopped == 0.0);
	aut_assert("4 test_aclock_elapsed", ac2->elapsed == 0.0);

	spend_time();

	aclock_elapsed(ac1);
	aut_assert("5 test_aclock_elapsed", ac1->stopped == 0.0);
	aut_assert("6 test_aclock_elapsed", ac1->elapsed > 0.0);

	// No change to ac2.
	aut_assert("7 test_aclock_elapsed", ac2->stopped == 0.0);
	aut_assert("8 test_aclock_elapsed", ac2->elapsed == 0.0);

	aclock_free(ac1);
	aclock_free(ac2);
}

void test_aclock_loop(void) {
	aclock *ac1;
	double r;
	double ll;

	ac1 = aclock_create();
	aut_assert("0 test_aclock_loop", ac1 != NULL);
	aut_assert("1 test_aclock_loop", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_loop", ac1->lastloop == ac1->started);
	aut_assert("3 test_aclock_loop", ac1->elapsed == 0.0);

	spend_time();

	r = aclock_loop(ac1);
	aut_assert("4 test_aclock_loop", ac1->stopped == 0.0);
	aut_assert("5 test_aclock_loop", r > 0.0);
	aut_assert("6 test_aclock_loop", ac1->lastloop > 0.0);
	aut_assert("7 test_aclock_loop", ac1->elapsed == 0.0);

	spend_time();

	r = aclock_loop(ac1);
	aut_assert("8 test_aclock_loop", ac1->stopped == 0.0);
	aut_assert("9 test_aclock_loop", r > 0.0);
	aut_assert("10 test_aclock_loop", ac1->lastloop > 0.0);
	aut_assert("11 test_aclock_loop", ac1->elapsed == 0.0);

	spend_time();
	spend_time();

	r = aclock_check_loop(ac1);
	ll = ac1->lastloop;
	aut_assert("12 test_aclock_loop", ac1->stopped == 0.0);
	aut_assert("13 test_aclock_loop", r > 0.0);
	aut_assert("14 test_aclock_loop", ac1->lastloop > 0.0);
	aut_assert("15 test_aclock_loop", r != ac1->lastloop);
	aut_assert("16 test_aclock_loop", r != ll);
	aut_assert("17 test_aclock_loop", ac1->elapsed == 0.0);

	aclock_free(ac1);
}

void test_aclock_reset(void) {
	aclock *ac1;
	double r;
	double ll;

	ac1 = aclock_create();
	aut_assert("0 test_aclock_reset", ac1 != NULL);
	aut_assert("1 test_aclock_reset", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_reset", ac1->lastloop == ac1->started);
	aut_assert("3 test_aclock_reset", ac1->elapsed == 0.0);

	spend_time();

	r = aclock_loop(ac1);
	aut_assert("4 test_aclock_reset", ac1->stopped == 0.0);
	aut_assert("5 test_aclock_reset", r > 0.0);
	aut_assert("6 test_aclock_reset", ac1->lastloop > 0.0);
	aut_assert("7 test_aclock_reset", ac1->elapsed == 0.0);

	r = aclock_reset(ac1);
	aut_assert("8 test_aclock_reset", ac1->started > 0.0);
	aut_assert("9 test_aclock_reset", ac1->stopped == ac1->started);
	aut_assert("10 test_aclock_reset", ac1->lastloop == ac1->started);
	aut_assert("11 test_aclock_reset", ac1->elapsed == 0.0);

	aclock_free(ac1);
}

void test_aclock_stop(void) {
	aclock *ac1;
	double r;
	double ll;

	ac1 = aclock_create();
	aut_assert("0 test_aclock_stop", ac1 != NULL);
	aut_assert("1 test_aclock_stop", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_stop", !(aclock_isstopped(ac1)));
	aut_assert("3 test_aclock_stop", ac1->elapsed == 0.0);

	spend_time();

	r = aclock_stop(ac1);
	aut_assert("4 test_aclock_stop", ac1->stopped > 0.0);
	aut_assert("5 test_aclock_stop", aclock_isstopped(ac1));
	aut_assert("6 test_aclock_stop", ac1->elapsed == r);

	spend_time();

	aclock_elapsed(ac1);
	aut_assert("7 test_aclock_stop", ac1->stopped > 0.0);
	aut_assert("8 test_aclock_stop", aclock_isstopped(ac1));
	aut_assert("9 test_aclock_stop", ac1->elapsed == r);

	aclock_free(ac1);
}

void test_aclock_start_when_stopped(void) {
	aclock *ac1;
	double r;
	double ll;

	ac1 = aclock_create();
	aut_assert("1 test_aclock_start_when_stopped", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_start_when_stopped", !(aclock_isstopped(ac1)));
	aut_assert("3 test_aclock_start_when_stopped", ac1->elapsed == 0.0);

	spend_time();

	r = aclock_stop(ac1);
	aut_assert("4 test_aclock_start_when_stopped", ac1->stopped > 0.0);
	aut_assert("5 test_aclock_start_when_stopped", aclock_isstopped(ac1));
	aut_assert("6 test_aclock_start_when_stopped", ac1->elapsed == r);

	spend_time();

	aclock_elapsed(ac1);
	aut_assert("7 test_aclock_start_when_stopped", ac1->stopped > 0.0);
	aut_assert("8 test_aclock_start_when_stopped", aclock_isstopped(ac1));
	aut_assert("9 test_aclock_start_when_stopped", ac1->elapsed == r);

	aclock_start(ac1);
	spend_time();

	aclock_elapsed(ac1);
	aut_assert("10 test_aclock_start_when_stopped", ac1->stopped == 0.0);
	aut_assert("11 test_aclock_start_when_stopped", !(aclock_isstopped(ac1)));
	aut_assert("12 test_aclock_start_when_stopped", ac1->elapsed > r);

	aclock_free(ac1);
}

void test_aclock_start_when_started(void) {
	aclock *ac1;
	double r;
	double ll;

	ac1 = aclock_create();
	aut_assert("1 test_aclock_start_when_started", ac1->stopped == 0.0);
	aut_assert("2 test_aclock_start_when_started", !(aclock_isstopped(ac1)));
	aut_assert("3 test_aclock_start_when_started", ac1->elapsed == 0.0);

	spend_time();

	aclock_elapsed(ac1);
	aut_assert("4 test_aclock_start_when_stopped", ac1->stopped == 0.0);
	aut_assert("5 test_aclock_start_when_started", !(aclock_isstopped(ac1)));
	aut_assert("6 test_aclock_start_when_stopped", ac1->elapsed > 0.0);

	spend_time();

	r = aclock_start(ac1);
	aut_assert("7 test_aclock_start_when_started", ac1->stopped == 0.0);
	aut_assert("8 test_aclock_start_when_started", !(aclock_isstopped(ac1)));
	aut_assert("9 test_aclock_start_when_started", r == 0.0);
	aut_assert("10 test_aclock_start_when_started", ac1->elapsed == 0.0);

	spend_time();

	aclock_elapsed(ac1);
	aut_assert("11 test_aclock_start_when_started", ac1->stopped == 0.0);
	aut_assert("12 test_aclock_start_when_started", !(aclock_isstopped(ac1)));
	aut_assert("13 test_aclock_start_when_started", ac1->elapsed > 0.0);

	aclock_free(ac1);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_aclock_creation);
	aut_run_test(test_aclock_elapsed);
	aut_run_test(test_aclock_loop);
	aut_run_test(test_aclock_reset);
	aut_run_test(test_aclock_stop);
	aut_run_test(test_aclock_start_when_stopped);
	aut_run_test(test_aclock_start_when_started);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
