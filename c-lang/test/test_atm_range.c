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

void print_atm(atm *at);
void print_atm_time_t(atm *at);
void print_atm_gm(atm *at);
void print_atm_loc(atm *at);
void create_test_times();
void free_test_times();

// ----------

// Globals used by the tests
time_t tnow, tsecless, tminless, thourless, tdayless, tmonless, tyearless;
atm *atnow;
atm *atnow2;
atm *atsecless;
atm *atminless;
atm *athourless;
atm *atdayless;
atm *atmonless;
atm *atyearless;

void create_test_times() {
	time_t tnow;

	// Create a time_t that is 10:30:30 on July 15th of this year.
	tnow = time(0);
	atm *at = atm_create_from_time_t(&tnow);
	at->loc->tm_hour = 10;
	at->loc->tm_min = 30;
	at->loc->tm_sec = 30;
	at->loc->tm_mon = 6;
	at->loc->tm_mday = 15;
	atm *at2 = atm_create_from_loctime(at->loc);
	tnow = at2->time;
	at = atm_free(at);
	at2 = atm_free(at2);

	tsecless = tnow - 1;
	tminless = tnow - 60;
	thourless = tnow - (60 * 60);
	tdayless = tnow - (60 * 60 * 24);
	tmonless = tnow - (60 * 60 * 24 * 31);
	tyearless = tnow - (60 * 60 * 24 * 366);

	atnow = atm_create_from_time_t(&tnow);
	atnow2 = atm_create_from_time_t(&tnow);
	atsecless = atm_create_from_time_t(&tsecless);
	atminless = atm_create_from_time_t(&tminless);
	athourless = atm_create_from_time_t(&thourless);
	atdayless = atm_create_from_time_t(&tdayless);
	atmonless = atm_create_from_time_t(&tmonless);
	atyearless = atm_create_from_time_t(&tyearless);
}

void free_test_times() {
	atnow = atm_free(atnow);
	atnow2 = atm_free(atnow2);
	atsecless = atm_free(atsecless);
	atminless = atm_free(atminless);
	athourless = atm_free(athourless);
	atdayless = atm_free(atdayless);
	atmonless = atm_free(atmonless);
	atyearless = atm_free(atyearless);
}

void print_atm(atm *at) {
	print_atm_time_t(at);
	print_atm_gm(at);
	print_atm_loc(at);
}

void print_atm_time_t(atm *at) {
	printf("time_t... original: %ld, time: %ld\n", 
		at->original, at->time);
}

void print_atm_loc(atm *at) {
	printf("loc...... year: %02d, mon: %02d, mday: %02d, hour: %02d, min: %02d, sec: %02d\n",
		at->loc->tm_year + 1900, at->loc->tm_mon + 1, at->loc->tm_mday,
		at->loc->tm_hour, at->loc->tm_min, at->loc->tm_sec);
}

void print_atm_gm(atm *at) {
	printf("gm....... year: %02d, mon: %02d, mday: %02d, hour: %02d, min: %02d, sec: %02d\n",
		at->gm->tm_year + 1900, at->gm->tm_mon + 1, at->gm->tm_mday,
		at->gm->tm_hour, at->gm->tm_min, at->gm->tm_sec);
}

// ----------

void test_create_test_times() {
	create_test_times();
	aut_assert("now test_create_test_times", atnow != NULL);
	aut_assert("sec less test_create_test_times", atsecless != NULL);
	aut_assert("min less test_create_test_times", atminless != NULL);
	aut_assert("hour less test_create_test_times", athourless != NULL);
	aut_assert("day less test_create_test_times", atdayless != NULL);
	aut_assert("mon less test_create_test_times", atmonless != NULL);
	aut_assert("year less test_create_test_times", atyearless != NULL);
	free_test_times();
}

void test_in_range() {
	create_test_times();
	atm_range *range = atm_range_create(atmonless, athourless);

//	printf("yearless: "); print_atm_loc(atyearless);
//	printf("monless:  "); print_atm_loc(atmonless);
//	printf("dayless:  "); print_atm_loc(atdayless);
//	printf("hourless: "); print_atm_loc(athourless);
//	printf("minless:  "); print_atm_loc(atminless);
//	printf("now:      "); print_atm_loc(atnow);
	
	aut_assert("day less should be in range month less and hour less", atm_is_in_range(atdayless, range, DATEANDTIME));
	aut_assert("year less should not be in range month less and hour less", !atm_is_in_range(atyearless, range, DATEANDTIME));
	aut_assert("min less should not be in range month less and hour less", !atm_is_in_range(atminless, range, DATEANDTIME));
	atm_range_free(range);
	free_test_times();
}

void test_is_equal() {
	create_test_times();
	atm_range *range1 = atm_range_create(atmonless, athourless);
	atm_range *range2 = atm_range_create(atmonless, athourless);
	atm_range *range3 = atm_range_create(atyearless, athourless);
	atm_range *range4 = atm_range_create(atmonless, atminless);
	atm_range *range5 = atm_range_create(athourless, atminless);
	atm_range *range6 = atm_range_create(atnow, atminless);
	atm_range *range7 = atm_range_create(atmonless, atdayless);
	atm_range *range8 = atm_range_create(atnow, atmonless);
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
	atm_range *range = atm_range_create(atmonless, athourless);
	aut_assert("day less is not before month less and hour less", !atm_is_before_range(atdayless, range, DATEANDTIME));
	aut_assert("year less is before range month less and hour less", atm_is_before_range(atyearless, range, DATEANDTIME));
	atm_range_free(range);
	free_test_times();
}

void test_is_after() {
	create_test_times();
	atm_range *range = atm_range_create(atmonless, athourless);
	aut_assert("day less should not be after month less and hour less", !atm_is_after_range(atdayless, range, DATEANDTIME));
	aut_assert("minute less should be after range month less and hour less", atm_is_after_range(atminless, range, DATEANDTIME));
	atm_range_free(range);
	free_test_times();
}

void test_to_string() {
	create_test_times();
	atm_range *range = atm_range_create(athourless, atmonless);
	char *rangestr = atm_range_to_string(range);
	aut_assert("test atm_range_to_string not null", rangestr != NULL);
//	print_atm_time_t(range->begin);
//	print_atm_time_t(range->end);
//	printf("%s\n", rangestr);

	char teststr[128];
	snprintf(teststr, 128, "%lu:%lu", range->begin->time, range->end->time);
	aut_assert("test atm_range_to_string length", strlen(rangestr) == strlen(teststr));
	aut_assert("test atm_range_to_string not null", strcmp(rangestr, teststr) == 0);

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
	aut_run_test(test_to_string);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
