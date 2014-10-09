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

void test_atm_create_now() {
	atm *at1 = atm_create_now();
	aut_assert("test atm_create_now", at1 != NULL);
//	printf("\nCreated from now...\n");
//	print_atm(at1);
	at1 = atm_free(at1);
}

void test_atm_create_from_time_t() {
	time_t t1 = time(0);
	atm *at1 = atm_create_from_time_t(&t1);
	aut_assert("test_atm_create_from_time_t", at1 != NULL);
//	printf("\nCreated from time_t...\n");
//	print_atm(at1);
	at1 = atm_free(at1);
}

void test_atm_create_from_gmtime() {
	time_t t1 = time(0);
	struct tm *tm1 = gmtime(&t1);
	atm *at1 = atm_create_from_gmtime(tm1);
	aut_assert("test_atm_create_from_gmtime", at1 != NULL);
//	printf("\nCreated from gmtime...\n");
//	print_atm(at1);
	at1 = atm_free(at1);
}

void test_atm_create_from_loctime() {
	time_t t1 = time(0);
	struct tm *tm1 = localtime(&t1);
	atm *at1 = atm_create_from_loctime(tm1);
	aut_assert("test_atm_create_from_loctime", at1 != NULL);
//	printf("\nCreated from localtime...\n");
//	print_atm(at1);
	at1 = atm_free(at1);
}

void test_free() {
	time_t t = time(0);
	atm *at = atm_create_from_time_t(&t);
	aut_assert("test_free_created", at != NULL);
	at = atm_free(at);
	aut_assert("test_free_freed", at == NULL);
	at = atm_free(at);
	aut_assert("test_free_freed", at == NULL);
}

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

void test_atm_compare() {
	create_test_times();
	aut_assert("now == now2", atm_compare(atnow, atnow2, DATEANDTIME) == 0);
	aut_assert("now == now2", atm_compare(atnow, atnow2, DATEONLY) == 0);
	aut_assert("now == now2", atm_compare(atnow, atnow2, TIMEONLY) == 0);
	aut_assert("year less < mon less", atm_compare(atyearless, atmonless, DATEANDTIME) < 0);
	aut_assert("now == hourless DATEONLY", atm_compare(atnow, athourless, DATEONLY) == 0);
	aut_assert("now == dayless TIMEONLY", atm_compare(atnow, atdayless, TIMEONLY) == 0);
	free_test_times();
}

void test_atm_compare_dateandtime() {
	create_test_times();
	aut_assert("year less < mon less", atm_compare(atyearless, atmonless, DATEANDTIME) < 0);
	aut_assert("mon less < day less", atm_compare(atmonless, atdayless, DATEANDTIME) < 0);
	aut_assert("day less < hour less", atm_compare(atdayless, athourless, DATEANDTIME) < 0);
	aut_assert("hour less < min less", atm_compare(athourless, atminless, DATEANDTIME) < 0);
	aut_assert("min less < sec less", atm_compare(atminless, atsecless, DATEANDTIME) < 0);
	free_test_times();
}

void test_atm_compare_dateonly() {
	create_test_times();
//	printf("\ntest_atm_compare_dateonly()\n");
//	printf("now... ");
//	print_atm(atnow);
//	printf("hourless... ");
//	print_atm(athourless);
//	printf("dayless... ");
//	print_atm(atdayless);
	aut_assert("year less < mon less", atm_compare(atyearless, atmonless, DATEONLY) < 0);
	aut_assert("mon less < day less", atm_compare(atmonless, atdayless, DATEONLY) < 0);
	aut_assert("day less < hour less", atm_compare(atdayless, athourless, DATEONLY) < 0);
	aut_assert("hour less == min less", atm_compare(athourless, atminless, DATEONLY) == 0);
	aut_assert("min less == sec less", atm_compare(atminless, atsecless, DATEONLY) == 0);
	free_test_times();
}

void test_atm_compare_timeonly() {
	create_test_times();
//	printf("\ntest_atm_compare_timeonly()\n");
//	printf("now... ");
//	print_atm(atnow);
//	printf("hourless... ");
//	print_atm(athourless);
//	printf("dayless... ");
//	print_atm(atdayless);
	aut_assert("year less == mon less", atm_compare(atyearless, atmonless, TIMEONLY) == 0);
	aut_assert("mon less == day less", atm_compare(atmonless, atdayless, TIMEONLY) == 0);
	aut_assert("day less > hour less", atm_compare(atdayless, athourless, TIMEONLY) > 0);
	aut_assert("hour less < min less", atm_compare(athourless, atminless, TIMEONLY) < 0);
	aut_assert("min less < sec less", atm_compare(atminless, atsecless, TIMEONLY) < 0);
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
