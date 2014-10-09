// test_atm.c - test the datetime functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include "astr.h"
#include "atm.h"
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

void test_atm_create_now() {
	atm *adt1 = atm_create_now();
	aut_assert("test atm_create_now", adt1 != NULL);
//	printf("\nCreated from now...\n");
//	print_atm(adt1);
	adt1 = atm_free(adt1);
}

void test_atm_create_from_time_t() {
	time_t t1 = time(0);
	atm *adt1 = atm_create_from_time_t(&t1);
	aut_assert("test_atm_create_from_time_t", adt1 != NULL);
//	printf("\nCreated from time_t...\n");
//	print_atm(adt1);
	adt1 = atm_free(adt1);
}

void test_atm_create_from_gmtime() {
	time_t t1 = time(0);
	struct tm *tm1 = gmtime(&t1);
	atm *adt1 = atm_create_from_gmtime(tm1);
	aut_assert("test_atm_create_from_gmtime", adt1 != NULL);
//	printf("\nCreated from gmtime...\n");
//	print_atm(adt1);
	adt1 = atm_free(adt1);
}

void test_atm_create_from_loctime() {
	time_t t1 = time(0);
	struct tm *tm1 = localtime(&t1);
	atm *adt1 = atm_create_from_loctime(tm1);
	aut_assert("test_atm_create_from_loctime", adt1 != NULL);
//	printf("\nCreated from localtime...\n");
//	print_atm(adt1);
	adt1 = atm_free(adt1);
}

void test_free() {
	time_t t = time(0);
	atm *adt = atm_create_from_time_t(&t);
	aut_assert("test_free_created", adt != NULL);
	adt = atm_free(adt);
	aut_assert("test_free_freed", adt == NULL);
	adt = atm_free(adt);
	aut_assert("test_free_freed", adt == NULL);
}

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

void test_atm_compare() {
	create_test_times();
	aut_assert("now == now2", atm_compare(adtnow, adtnow2, DATEANDTIME) == 0);
	aut_assert("now == now2", atm_compare(adtnow, adtnow2, DATEONLY) == 0);
	aut_assert("now == now2", atm_compare(adtnow, adtnow2, TIMEONLY) == 0);
	aut_assert("year less < mon less", atm_compare(adtyearless, adtmonless, DATEANDTIME) < 0);
	aut_assert("now == hourless DATEONLY", atm_compare(adtnow, adthourless, DATEONLY) == 0);
	aut_assert("now == dayless TIMEONLY", atm_compare(adtnow, adtdayless, TIMEONLY) == 0);
	free_test_times();
}

void test_atm_compare_dateandtime() {
	create_test_times();
	aut_assert("year less < mon less", atm_compare(adtyearless, adtmonless, DATEANDTIME) < 0);
	aut_assert("mon less < day less", atm_compare(adtmonless, adtdayless, DATEANDTIME) < 0);
	aut_assert("day less < hour less", atm_compare(adtdayless, adthourless, DATEANDTIME) < 0);
	aut_assert("hour less < min less", atm_compare(adthourless, adtminless, DATEANDTIME) < 0);
	aut_assert("min less < sec less", atm_compare(adtminless, adtsecless, DATEANDTIME) < 0);
	free_test_times();
}

void test_atm_compare_dateonly() {
	create_test_times();
//	printf("\ntest_atm_compare_dateonly()\n");
//	printf("now... ");
//	print_atm(adtnow);
//	printf("hourless... ");
//	print_atm(adthourless);
//	printf("dayless... ");
//	print_atm(adtdayless);
	aut_assert("year less < mon less", atm_compare(adtyearless, adtmonless, DATEONLY) < 0);
	aut_assert("mon less < day less", atm_compare(adtmonless, adtdayless, DATEONLY) < 0);
	aut_assert("day less < hour less", atm_compare(adtdayless, adthourless, DATEONLY) < 0);
	aut_assert("hour less == min less", atm_compare(adthourless, adtminless, DATEONLY) == 0);
	aut_assert("min less == sec less", atm_compare(adtminless, adtsecless, DATEONLY) == 0);
	free_test_times();
}

void test_atm_compare_timeonly() {
	create_test_times();
//	printf("\ntest_atm_compare_timeonly()\n");
//	printf("now... ");
//	print_atm(adtnow);
//	printf("hourless... ");
//	print_atm(adthourless);
//	printf("dayless... ");
//	print_atm(adtdayless);
	aut_assert("year less == mon less", atm_compare(adtyearless, adtmonless, TIMEONLY) == 0);
	aut_assert("mon less == day less", atm_compare(adtmonless, adtdayless, TIMEONLY) == 0);
	aut_assert("day less > hour less", atm_compare(adtdayless, adthourless, TIMEONLY) > 0);
	aut_assert("hour less < min less", atm_compare(adthourless, adtminless, TIMEONLY) < 0);
	aut_assert("min less < sec less", atm_compare(adtminless, adtsecless, TIMEONLY) < 0);
	free_test_times();
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_atm_create_now);
	aut_run_test(test_atm_create_from_time_t);
	aut_run_test(test_atm_create_from_gmtime);
	aut_run_test(test_atm_create_from_loctime);
	aut_run_test(test_create_test_times);
	aut_run_test(test_free);
	aut_run_test(test_atm_compare);
	aut_run_test(test_atm_compare_dateandtime);
	aut_run_test(test_atm_compare_dateonly);
	aut_run_test(test_atm_compare_timeonly);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
