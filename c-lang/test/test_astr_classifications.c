// test_astr_classifications.c - test classification functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "astr.h"
#include "adept_unit_test.h"

int suite_runs;
int suite_fails;
int test_runs;
int test_fails;
astr *test_messages;
char error[256];

// ----------

char *nonempty = "abc  123";
char *empty = "";
char *blank = "     ";
char *space = " \t\n  ";
char *alphabetic = "aAbBzZ";
char *alphanumeric = "1122aAbBzZ0099";
char *one = "1";
char *zero = "0";
char *y = "y";
char *Y = "Y";
char *n = "n";
char *N = "N";
char *yes = "yes";
char *Yes = "Yes";
char *YES = "YES";
char *no = "no";
char *No = "No";
char *NO = "NO";
char *t = "t";
char *T = "T";
char *f = "f";
char *F = "F";
char *truestr = "true";
char *Truestr = "True";
char *TRUEstr = "TRUE";
char *falsestr = "false";
char *Falsestr = "False";
char *FALSEstr = "FALSE";
astr *as;

void test_is_nothing(void) {
	
	as = astr_create(nonempty);
	aut_assert("1 nonempty", astr_is_empty(as) == 0);
	aut_assert("2 nonempty", astr_is_blank(as) == 0);
	aut_assert("3 nonempty", astr_is_space(as) == 0);

	as = astr_set(as, empty);
	aut_assert("4 empty", astr_is_empty(as) == 1);
	aut_assert("5 empty", astr_is_blank(as) == 0);
	aut_assert("6 empty", astr_is_space(as) == 0);

	as = astr_set(as, blank);
	aut_assert("7 blank", astr_is_empty(as) == 0);
	aut_assert("8 blank", astr_is_blank(as) == 1);
	aut_assert("9 blank", astr_is_space(as) == 1);
	
	as = astr_set(as, space);
	aut_assert("10 space", astr_is_empty(as) == 0);
	aut_assert("11 space", astr_is_blank(as) == 0);
	aut_assert("12 space", astr_is_space(as) == 1);

	as = astr_free(as);
}

int test_is_alphabetic(void) {
	as = astr_create(nonempty);
	aut_assert("nonempty", astr_is_alphabetic(as) == 0);

	as = astr_set(as, empty);
	aut_assert("empty", astr_is_alphabetic(as) == 0);

	as = astr_set(as, blank);
	aut_assert("blank", astr_is_alphabetic(as) == 0);

	as = astr_set(as, space);
	aut_assert("space", astr_is_alphabetic(as) == 0);

	as = astr_set(as, alphabetic);
	aut_assert("alphabetic", astr_is_alphabetic(as) == 1);

	as = astr_set(as, alphanumeric);
	aut_assert("alphanumeric", astr_is_alphabetic(as) == 0);

	as = astr_free(as);
}

int test_is_alphanumeric(void) {
	as = astr_create(nonempty);
	aut_assert("nonempty", astr_is_alphanumeric(as) == 0);

	as = astr_set(as, empty);
	aut_assert("empty", astr_is_alphanumeric(as) == 0);

	as = astr_set(as, blank);
	aut_assert("blank", astr_is_alphanumeric(as) == 0);

	as = astr_set(as, space);
	aut_assert("space", astr_is_alphanumeric(as) == 0);

	as = astr_set(as, alphabetic);
	aut_assert("alphabetic", astr_is_alphanumeric(as) == 1);

	as = astr_set(as, alphanumeric);
	aut_assert("alphanumeric", astr_is_alphanumeric(as) == 1);

	as = astr_free(as);
}

int test_is_10(void) {
	as = astr_create(nonempty);
	aut_assert("nonempty", astr_is_10(as) == 0);

	as = astr_set(as, empty);
	aut_assert("empty", astr_is_10(as) == 0);

	as = astr_set(as, blank);
	aut_assert("blank", astr_is_10(as) == 0);

	as = astr_set(as, space);
	aut_assert("space", astr_is_10(as) == 0);

	as = astr_set(as, alphabetic);
	aut_assert("alphabetic", astr_is_10(as) == 0);

	as = astr_set(as, alphanumeric);
	aut_assert("alphanumeric", astr_is_10(as) == 0);

	as = astr_set(as, one);
	aut_assert("one", astr_is_10(as) == 1);

	as = astr_set(as, zero);
	aut_assert("zero", astr_is_10(as) == 1);

	as = astr_free(as);
}

int test_is_yn(void) {
	as = astr_create(nonempty);
	aut_assert("nonempty", astr_is_yn(as) == 0);

	as = astr_set(as, empty);
	aut_assert("empty", astr_is_yn(as) == 0);

	as = astr_set(as, blank);
	aut_assert("blank", astr_is_yn(as) == 0);

	as = astr_set(as, space);
	aut_assert("space", astr_is_yn(as) == 0);

	as = astr_set(as, alphabetic);
	aut_assert("alphabetic", astr_is_yn(as) == 0);

	as = astr_set(as, alphanumeric);
	aut_assert("alphanumeric", astr_is_yn(as) == 0);

	as = astr_set(as, y);
	aut_assert("y", astr_is_yn(as) == 1);

	as = astr_set(as, n);
	aut_assert("n", astr_is_yn(as) == 1);

	as = astr_set(as, Y);
	aut_assert("Y", astr_is_yn(as) == 1);

	as = astr_set(as, N);
	aut_assert("N", astr_is_yn(as) == 1);

	as = astr_set(as, yes);
	aut_assert("yes", astr_is_yn(as) == 1);

	as = astr_set(as, no);
	aut_assert("no", astr_is_yn(as) == 1);

	as = astr_set(as, Yes);
	aut_assert("Yes", astr_is_yn(as) == 1);

	as = astr_set(as, No);
	aut_assert("No", astr_is_yn(as) == 1);

	as = astr_set(as, YES);
	aut_assert("YES", astr_is_yn(as) == 1);

	as = astr_set(as, NO);
	aut_assert("NO", astr_is_yn(as) == 1);

	as = astr_free(as);
}

int test_is_tf(void) {
	as = astr_create(nonempty);
	aut_assert("nonempty", astr_is_tf(as) == 0);

	as = astr_set(as, empty);
	aut_assert("empty", astr_is_tf(as) == 0);

	as = astr_set(as, blank);
	aut_assert("blank", astr_is_tf(as) == 0);

	as = astr_set(as, space);
	aut_assert("space", astr_is_tf(as) == 0);

	as = astr_set(as, alphabetic);
	aut_assert("alphabetic", astr_is_tf(as) == 0);

	as = astr_set(as, alphanumeric);
	aut_assert("alphanumeric", astr_is_tf(as) == 0);

	as = astr_set(as, t);
	aut_assert("t", astr_is_tf(as) == 1);

	as = astr_set(as, f);
	aut_assert("f", astr_is_tf(as) == 1);

	as = astr_set(as, T);
	aut_assert("T", astr_is_tf(as) == 1);

	as = astr_set(as, F);
	aut_assert("F", astr_is_tf(as) == 1);

	as = astr_set(as, truestr);
	aut_assert("true", astr_is_tf(as) == 1);

	as = astr_set(as, falsestr);
	aut_assert("false", astr_is_tf(as) == 1);

	as = astr_set(as, Truestr);
	aut_assert("True", astr_is_tf(as) == 1);

	as = astr_set(as, Falsestr);
	aut_assert("False", astr_is_tf(as) == 1);

	as = astr_set(as, TRUEstr);
	aut_assert("TRUE", astr_is_tf(as) == 1);

	as = astr_set(as, FALSEstr);
	aut_assert("FALSE", astr_is_tf(as) == 1);

	as = astr_free(as);
}

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_is_nothing);
	aut_run_test(test_is_alphabetic);
	aut_run_test(test_is_alphanumeric);
	aut_run_test(test_is_10);
	aut_run_test(test_is_yn);
	aut_run_test(test_is_tf);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
