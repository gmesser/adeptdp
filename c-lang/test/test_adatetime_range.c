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

void test_in_range() {
	create_test_times();
	adatetime_range *range = adatetime_range_create(adtmonless, adthourless);
	aut_assert("day less is in range month less and hour less", adatetime_is_in_range(adtdayless, range, DATEANDTIME));
	aut_assert("year less is not in range month less and hour less", !adatetime_is_in_range(adtyearless, range, DATEANDTIME));
	aut_assert("min less is not in range month less and hour less", !adatetime_is_in_range(adtminless, range, DATEANDTIME));
	adatetime_range_free(range);
	free_test_times();
}

void test_is_equal() {
	create_test_times();
	adatetime_range *range1 = adatetime_range_create(adtmonless, adthourless);
	adatetime_range *range2 = adatetime_range_create(adtmonless, adthourless);
	adatetime_range *range3 = adatetime_range_create(adtyearless, adthourless);
	adatetime_range *range4 = adatetime_range_create(adtmonless, adtminless);
	adatetime_range *range5 = adatetime_range_create(adthourless, adtminless);
	adatetime_range *range6 = adatetime_range_create(adtnow, adtminless);
	adatetime_range *range7 = adatetime_range_create(adtmonless, adtdayless);
	adatetime_range *range8 = adatetime_range_create(adtnow, adtmonless);
	aut_assert("self is equal self", adatetime_range_is_equal(range1, range1, DATEANDTIME));
	aut_assert("should be equal", adatetime_range_is_equal(range1, range2, DATEANDTIME));
	aut_assert("should be equal date only", adatetime_range_is_equal(range5, range6, DATEONLY));
	aut_assert("should be equal time only", adatetime_range_is_equal(range7, range8, TIMEONLY));
	aut_assert("starts earlier, should not be equal", !adatetime_range_is_equal(range1, range3, DATEANDTIME));
	aut_assert("ends later, should not be equal", !adatetime_range_is_equal(range1, range4, DATEANDTIME));
	adatetime_range_free(range1);
	adatetime_range_free(range2);
	adatetime_range_free(range3);
	adatetime_range_free(range4);
	adatetime_range_free(range5);
	adatetime_range_free(range6);
	adatetime_range_free(range7);
	adatetime_range_free(range8);
	free_test_times();
}

void test_is_before() {
	create_test_times();
	adatetime_range *range = adatetime_range_create(adtmonless, adthourless);
	aut_assert("day less is not before month less and hour less", !adatetime_is_before_range(adtdayless, range, DATEANDTIME));
	aut_assert("year less is before range month less and hour less", adatetime_is_before_range(adtyearless, range, DATEANDTIME));
	adatetime_range_free(range);
	free_test_times();
}

void test_is_after() {
	create_test_times();
	adatetime_range *range = adatetime_range_create(adtmonless, adthourless);
	aut_assert("day less is not after month less and hour less", !adatetime_is_after_range(adtdayless, range, DATEANDTIME));
	aut_assert("minute less is after range month less and hour less", adatetime_is_after_range(adtminless, range, DATEANDTIME));
	adatetime_range_free(range);
	free_test_times();
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_create_test_times);
	aut_run_test(test_in_range);
	aut_run_test(test_is_equal);
	aut_run_test(test_is_before);
	aut_run_test(test_is_after);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
