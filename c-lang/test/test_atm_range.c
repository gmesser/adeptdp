// test_atm_range.c - test the datetime range functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "astr.h"
#include "atm.h"
#include "atm_range.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
aclock *suite_clock;
int test_runs;
int test_fails;
astr *suite_messages;
char error[256];

void print_atm(atm *adt);
void print_atm_time_t(atm *adt);
void print_atm_gm(atm *adt);
void print_atm_loc(atm *adt);
void create_test_times();
void free_test_times();

// ----------

// Globals used by the tests
time_t tnow, tsecless, tminless, thourless, tdayless, tmonless, tyearless;
atm *adtnow;
atm *adtnow2;
atm *adtsecless;
atm *adtminless;
atm *adthourless;
atm *adtdayless;
atm *adtmonless;
atm *adtyearless;

void create_test_times() {
	time_t tnow;

	// Create a time_t that is 10:30:30 on July 15th of this year.
	tnow = time(0);
	atm *adt = atm_create_from_time_t(&tnow);
	adt->loc->tm_hour = 10;
	adt->loc->tm_min = 30;
	adt->loc->tm_sec = 30;
	adt->loc->tm_mon = 6;
	adt->loc->tm_mday = 15;
	atm *adt2 = atm_create_from_loctime(adt->loc);
	tnow = adt2->time;
	adt = atm_free(adt);
	adt2 = atm_free(adt2);

	tsecless = tnow - 1;
	tminless = tnow - 60;
	thourless = tnow - (60 * 60);
	tdayless = tnow - (60 * 60 * 24);
	tmonless = tnow - (60 * 60 * 24 * 31);
	tyearless = tnow - (60 * 60 * 24 * 366);

	adtnow = atm_create_from_time_t(&tnow);
	adtnow2 = atm_create_from_time_t(&tnow);
	adtsecless = atm_create_from_time_t(&tsecless);
	adtminless = atm_create_from_time_t(&tminless);
	adthourless = atm_create_from_time_t(&thourless);
	adtdayless = atm_create_from_time_t(&tdayless);
	adtmonless = atm_create_from_time_t(&tmonless);
	adtyearless = atm_create_from_time_t(&tyearless);
}

void free_test_times() {
	adtnow = atm_free(adtnow);
	adtnow2 = atm_free(adtnow2);
	adtsecless = atm_free(adtsecless);
	adtminless = atm_free(adtminless);
	adthourless = atm_free(adthourless);
	adtdayless = atm_free(adtdayless);
	adtmonless = atm_free(adtmonless);
	adtyearless = atm_free(adtyearless);
}

void print_atm(atm *adt) {
	print_atm_time_t(adt);
	print_atm_gm(adt);
	print_atm_loc(adt);
}

void print_atm_time_t(atm *adt) {
	printf("time_t... original: %ld, time: %ld\n", 
		adt->original, adt->time);
}

void print_atm_loc(atm *adt) {
	printf("loc...... year: %d, mon: %d, mday: %d, hour: %d, min: %d, sec: %d\n",
		adt->loc->tm_year + 1900, adt->loc->tm_mon + 1, adt->loc->tm_mday,
		adt->loc->tm_hour, adt->loc->tm_min, adt->loc->tm_sec);
}

void print_atm_gm(atm *adt) {
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
	atm_range *range = atm_range_create(adtmonless, adthourless);
	aut_assert("day less is in range month less and hour less", atm_is_in_range(adtdayless, range, DATEANDTIME));
	aut_assert("year less is not in range month less and hour less", !atm_is_in_range(adtyearless, range, DATEANDTIME));
	aut_assert("min less is not in range month less and hour less", !atm_is_in_range(adtminless, range, DATEANDTIME));
	atm_range_free(range);
	free_test_times();
}

void test_is_equal() {
	create_test_times();
	atm_range *range1 = atm_range_create(adtmonless, adthourless);
	atm_range *range2 = atm_range_create(adtmonless, adthourless);
	atm_range *range3 = atm_range_create(adtyearless, adthourless);
	atm_range *range4 = atm_range_create(adtmonless, adtminless);
	atm_range *range5 = atm_range_create(adthourless, adtminless);
	atm_range *range6 = atm_range_create(adtnow, adtminless);
	atm_range *range7 = atm_range_create(adtmonless, adtdayless);
	atm_range *range8 = atm_range_create(adtnow, adtmonless);
	aut_assert("self is equal self", atm_range_is_equal(range1, range1, DATEANDTIME));
	aut_assert("should be equal", atm_range_is_equal(range1, range2, DATEANDTIME));
	aut_assert("should be equal date only", atm_range_is_equal(range5, range6, DATEONLY));
	aut_assert("should be equal time only", atm_range_is_equal(range7, range8, TIMEONLY));
	aut_assert("starts earlier, should not be equal", !atm_range_is_equal(range1, range3, DATEANDTIME));
	aut_assert("ends later, should not be equal", !atm_range_is_equal(range1, range4, DATEANDTIME));
	atm_range_free(range1);
	atm_range_free(range2);
	atm_range_free(range3);
	atm_range_free(range4);
	atm_range_free(range5);
	atm_range_free(range6);
	atm_range_free(range7);
	atm_range_free(range8);
	free_test_times();
}

void test_is_before() {
	create_test_times();
	atm_range *range = atm_range_create(adtmonless, adthourless);
	aut_assert("day less is not before month less and hour less", !atm_is_before_range(adtdayless, range, DATEANDTIME));
	aut_assert("year less is before range month less and hour less", atm_is_before_range(adtyearless, range, DATEANDTIME));
	atm_range_free(range);
	free_test_times();
}

void test_is_after() {
	create_test_times();
	atm_range *range = atm_range_create(adtmonless, adthourless);
	aut_assert("day less is not after month less and hour less", !atm_is_after_range(adtdayless, range, DATEANDTIME));
	aut_assert("minute less is after range month less and hour less", atm_is_after_range(adtminless, range, DATEANDTIME));
	atm_range_free(range);
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
