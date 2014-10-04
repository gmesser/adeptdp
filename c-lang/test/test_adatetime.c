// test_adatetime.c - test the datetime functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "astr.h"
#include "adatetime.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
aclock *suite_clock;
int test_runs;
int test_fails;
astr *suite_messages;
char error[256];

void print_adatetime(adatetime *adt);
void print_adatetime_time_t(adatetime *adt);
void print_adatetime_gm(adatetime *adt);
void print_adatetime_loc(adatetime *adt);

// ----------

void print_adatetime(adatetime *adt) {
	print_adatetime_time_t(adt);
	print_adatetime_gm(adt);
	print_adatetime_loc(adt);
}

void print_adatetime_time_t(adatetime *adt) {
	printf("time_t... original: %ld, time: %ld\n", 
		adt->original, adt->time);
}

void print_adatetime_loc(adatetime *adt) {
	printf("loc...... year: %d, mon: %d, mday: %d, hour: %d, min: %d, sec: %d\n",
		adt->loc->tm_year + 1900, adt->loc->tm_mon + 1, adt->loc->tm_mday,
		adt->loc->tm_hour, adt->loc->tm_min, adt->loc->tm_sec);
}

void print_adatetime_gm(adatetime *adt) {
	printf("gm....... year: %d, mon: %d, mday: %d, hour: %d, min: %d, sec: %d\n",
		adt->gm->tm_year + 1900, adt->gm->tm_mon + 1, adt->gm->tm_mday,
		adt->gm->tm_hour, adt->gm->tm_min, adt->gm->tm_sec);
}

// ----------

void test_adatetime_create_now() {
	adatetime *adt1 = adatetime_create_now();
	aut_assert("test adatetime_create_now", adt1 != NULL);
	printf("\nCreated from now...\n");
	print_adatetime(adt1);
}

void test_adatetime_create_from_time_t() {
	time_t t1 = time(0);
	adatetime *adt1 = adatetime_create_from_time_t(t1);
	aut_assert("test_adatetime_create_from_time_t", adt1 != NULL);
	printf("\nCreated from time_t...\n");
	print_adatetime(adt1);
}

void test_adatetime_create_from_gmtime() {
	time_t t1 = time(0);
	struct tm *tm1 = gmtime(&t1);
	adatetime *adt1 = adatetime_create_from_gmtime(tm1);
	aut_assert("test_adatetime_create_from_gmtime", adt1 != NULL);
	printf("\nCreated from gmtime...\n");
	print_adatetime(adt1);
}

void test_adatetime_create_from_loctime() {
	time_t t1 = time(0);
	struct tm *tm1 = localtime(&t1);
	adatetime *adt1 = adatetime_create_from_loctime(tm1);
	aut_assert("test_adatetime_create_from_loctime", adt1 != NULL);
	printf("\nCreated from localtime...\n");
	print_adatetime(adt1);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_adatetime_create_now);
	aut_run_test(test_adatetime_create_from_time_t);
	aut_run_test(test_adatetime_create_from_gmtime);
	aut_run_test(test_adatetime_create_from_loctime);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
