// test_adatetime_range.c - test the datetime range functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "astr.h"
#include "adatetime.h"
#include "adatetime_range.h"
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
void create_test_times();
void free_test_times();

// ----------

// Globals used by the tests
time_t tnow, tsecless, tminless, thourless, tdayless, tmonless, tyearless;
adatetime *adtnow;
adatetime *adtnow2;
adatetime *adtsecless;
adatetime *adtminless;
adatetime *adthourless;
adatetime *adtdayless;
adatetime *adtmonless;
adatetime *adtyearless;

void create_test_times() {
	time_t tnow;

	// Create a time_t that is 10:30:30 on July 15th of this year.
	tnow = time(0);
	adatetime *adt = adatetime_create_from_time_t(&tnow);
	adt->loc->tm_hour = 10;
	adt->loc->tm_min = 30;
	adt->loc->tm_sec = 30;
	adt->loc->tm_mon = 6;
	adt->loc->tm_mday = 15;
	adatetime *adt2 = adatetime_create_from_loctime(adt->loc);
	tnow = adt2->time;
	adt = adatetime_free(adt);
	adt2 = adatetime_free(adt2);

	tsecless = tnow - 1;
	tminless = tnow - 60;
	thourless = tnow - (60 * 60);
	tdayless = tnow - (60 * 60 * 24);
	tmonless = tnow - (60 * 60 * 24 * 31);
	tyearless = tnow - (60 * 60 * 24 * 366);

	adtnow = adatetime_create_from_time_t(&tnow);
	adtnow2 = adatetime_create_from_time_t(&tnow);
	adtsecless = adatetime_create_from_time_t(&tsecless);
	adtminless = adatetime_create_from_time_t(&tminless);
	adthourless = adatetime_create_from_time_t(&thourless);
	adtdayless = adatetime_create_from_time_t(&tdayless);
	adtmonless = adatetime_create_from_time_t(&tmonless);
	adtyearless = adatetime_create_from_time_t(&tyearless);
}

void free_test_times() {
	adtnow = adatetime_free(adtnow);
	adtnow2 = adatetime_free(adtnow2);
	adtsecless = adatetime_free(adtsecless);
	adtminless = adatetime_free(adtminless);
	adthourless = adatetime_free(adthourless);
	adtdayless = adatetime_free(adtdayless);
	adtmonless = adatetime_free(adtmonless);
	adtyearless = adatetime_free(adtyearless);
}

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

void test_create_test_times() {
	create_test_times();
	aut_assert("now test_create_test_times", adtnow != NULL);
	aut_assert("sec less test_create_test_times", adtsecless != NULL);
	aut_assert("min less test_create_test_times", adtminless != NULL);
	aut_assert("hour less test_create_test_times", adthourless != NULL);
	aut_assert("day less test_create_test_times", adtdayless != NULL);
	aut_assert("mon less test_create_test_times", adtmonless != NULL);
	aut_assert("year less test_create_test_times", adtyearless != NULL);
	free_test_times();
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_create_test_times);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
