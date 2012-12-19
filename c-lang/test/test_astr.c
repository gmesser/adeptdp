// test_astr_create.c - test the creation and reinitialization functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "astr.h"
#include "aclock.h"
#include "adept_unit_test.h"

aclock *suite_clock;
astr *suite_messages;
int suite_runs;
int suite_fails;
int test_runs;
int test_fails;

// ----------

void test_creation(void) {
	char *str = "ABC";
	char *buf = "DEF   ";
	char *empty = "";
	astr *as_str;
	astr *as_buf;
	astr *as_empty;

	as_str = astr_create(str);
	aut_assert("1 creation", strcmp(as_str->string, str) == 0);
	aut_assert("1 length", as_str->length == strlen(str));

	as_buf = astr_create_from_buffer(buf, strlen(buf));
	aut_assert("2 creation", strcmp(as_buf->string, buf) == 0);
	aut_assert("2 length", as_buf->length == strlen(buf));

	as_empty = astr_create(empty);
	aut_assert("3 creation", strcmp(as_empty->string, empty) == 0);
	aut_assert("3 length", as_empty->length == strlen(empty));

	astr_free(as_str);
	astr_free(as_buf);
	astr_free(as_empty);
}

void test_reinitialization_without_creation(void) {
	char *str = "ABC";
	char *buf = "DEF   ";
	char *empty = "";
	astr *as_str = NULL;
	astr *as_buf = NULL;
	astr *as_empty = NULL;

	as_str = astr_set(as_str, str);
	aut_assert("1 reinit str", strcmp(as_str->string, str) == 0);

	as_buf = astr_set_from_buffer(as_buf, buf, strlen(buf));
	aut_assert("2 reinit buf", strcmp(as_buf->string, buf) == 0);

	as_empty = astr_set(as_empty, empty);
	aut_assert("3 reinit empty", strcmp(as_empty->string, empty) == 0);

	astr_free(as_str);
	astr_free(as_buf);
	astr_free(as_empty);
}

void test_reinitialization(void) {
	char *str1 = "ABC";
	char *str2 = "UVW";
	char *str_long = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *buf1 = "DEF   ";
	char *buf2 = "XYZ   ";
	char *buf_long = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	astr *as_str;
	astr *as_buf;

	as_str = astr_create(str1);
	aut_assert("create from string", strcmp(as_str->string, str1) == 0);
	aut_assert("create from string", as_str->length == strlen(str1));
	aut_assert("create from string", as_str->allocated_length == strlen(str1) + 1);
	as_str = astr_set(as_str, str2);
	aut_assert("set from same-length string", strcmp(as_str->string, str2) == 0);
	aut_assert("set from same-length string", as_str->length == strlen(str2));
	aut_assert("set from same-length string", as_str->allocated_length == strlen(str2) + 1);

	as_str = astr_set(as_str, str_long);
	aut_assert("set from long string", strcmp(as_str->string, str_long) == 0);
	aut_assert("set from long string", as_str->length == strlen(str_long));
	aut_assert("set from long string", as_str->allocated_length == strlen(str_long) + 1);
	as_str = astr_set(as_str, str2);
	aut_assert("set from short string", strcmp(as_str->string, str2) == 0);
	aut_assert("set from short string", as_str->length == strlen(str2));
	aut_assert("set from short string", as_str->allocated_length == strlen(str_long) + 1);

	as_buf = astr_create_from_buffer(buf1, strlen(buf1));
	aut_assert("create from buffer", strcmp(as_buf->string, buf1) == 0);
	aut_assert("create from buffer", as_buf->length == strlen(buf1));
	aut_assert("create from buffer", as_buf->allocated_length == strlen(buf1) + 1);
	as_buf = astr_set_from_buffer(as_buf, buf2, strlen(buf2));
	aut_assert("set from same-length buffer", strcmp(as_buf->string, buf2) == 0);
	aut_assert("set from same-length buffer", as_buf->length == strlen(buf2));
	aut_assert("set from same-length buffer", as_buf->allocated_length == strlen(buf2) + 1);

	as_buf = astr_set_from_buffer(as_buf, buf_long, strlen(buf_long));
	aut_assert("set from long buffer", strcmp(as_buf->string, buf_long) == 0);
	aut_assert("set from long buffer", as_buf->length == strlen(buf_long));
	aut_assert("set from long buffer", as_buf->allocated_length == strlen(buf_long) + 1);
	as_buf = astr_set_from_buffer(as_buf, buf2, strlen(buf2));
	aut_assert("set from short buffer", strcmp(as_buf->string, buf2) == 0);
	aut_assert("set from short buffer", as_buf->length == strlen(buf2));
	aut_assert("set from short buffer", as_buf->allocated_length == strlen(buf_long) + 1);

	astr_free(as_str);
	astr_free(as_buf);
}

void test_append(void) {
	char *str = "ABC";
	char *buf = "XYZ   ";
	char *str_plus_str = "ABCABC";
	char *str_plus_buf = "ABCXYZ   ";
	char *buf_plus_buf = "XYZ   XYZ   ";
	char *buf_plus_str = "XYZ   ABC";
	astr *as_str;
	astr *as_buf;

	as_str = astr_create(str);
	as_str = astr_append(as_str, str);
	aut_assert("1 test_append string", strcmp(as_str->string, str_plus_str) == 0);

	as_buf = astr_set_from_buffer(as_buf, buf, strlen(buf));
	as_buf = astr_append(as_buf, str);
	aut_assert("2 test_append string", strcmp(as_buf->string, buf_plus_str) == 0);

	as_str = astr_set(as_str, str);
	as_str = astr_append_buffer(as_str, buf, strlen(buf));
	aut_assert("3 test_append buffer", strcmp(as_str->string, str_plus_buf) == 0);

	as_buf = astr_create_from_buffer(buf, strlen(buf));
	as_buf = astr_append_buffer(as_buf, buf, strlen(buf));
	aut_assert("4 test_append buffer", strcmp(as_buf->string, buf_plus_buf) == 0);

	astr_free(as_str);
	astr_free(as_buf);
}

void test_printf(void) {
	char *str = "ABC";
	int num = 1;
	char *num_str = "001";
	char *str_plus_num_str = "ABC 001";
	astr *as1;
	astr *as2;
	astr *as3;

	as1 = astr_printf("%s", str);
	aut_assert("1 test_printf creation", strcmp(as1->string, str) == 0);
	aut_assert("1 test_printf length", as1->length == strlen(str));

	as2 = astr_printf("%03d", num);
	aut_assert("2 test_printf creation", strcmp(as2->string, num_str) == 0);
	aut_assert("2 test_printf length", as2->length == strlen(num_str));

	as3 = astr_printf("%s %03d", str, num);
	aut_assert("3 test_printf creation", strcmp(as3->string, str_plus_num_str) == 0);
	aut_assert("3 test_printf length", as3->length == strlen(str_plus_num_str));

	astr_free(as1);
	astr_free(as2);
	astr_free(as3);
}

void test_printf_long(void) {
	int num = 1;
	char *num_str = "001";
	char long_str[301];
	char num_str_plus_long_str_plus_num_str[309];
	astr *as1;
	astr *as2;

	memset(long_str, 'A', 300);
	long_str[300] = '\0';

	as1 = astr_printf("%s", long_str);
	aut_assert("1 test_printf long creation", strcmp(as1->string, long_str) == 0);
	aut_assert("1 test_printf long length", as1->length == strlen(long_str));

	strcpy(num_str_plus_long_str_plus_num_str, num_str);
	strcat(num_str_plus_long_str_plus_num_str, " ");
	strcat(num_str_plus_long_str_plus_num_str, long_str);
	strcat(num_str_plus_long_str_plus_num_str, " ");
	strcat(num_str_plus_long_str_plus_num_str, num_str);

	as2 = astr_printf("%03d %s %03d", num, long_str, num);
	aut_assert("2 test_printf long creation", strcmp(as2->string, num_str_plus_long_str_plus_num_str) == 0);
	aut_assert("2 test_printf long length", as2->length == strlen(num_str_plus_long_str_plus_num_str));

	astr_free(as1);
	astr_free(as2);
}

void test_printf_and_append(void) {
	astr *as;
	char long_str[301];
	memset(long_str, 'A', 300);
	long_str[300] = '\0';
	int i, j;

	for (j = 0; j < 100; j++) {
		as = astr_printf("%s", long_str);
		for (i = 0; i < 100; i++) {
			as = astr_append(as, long_str);
		}
		as = astr_free(as);

		as = astr_printf("%s", long_str);
		for (i = 0; i < 100; i++) {
			as = astr_append(as, long_str);
		}
		as = astr_free(as);

		as = astr_printf("%s", long_str);
		for (i = 0; i < 100; i++) {
			as = astr_append(as, long_str);
		}
		as = astr_free(as);

		as = astr_printf("%s", long_str);
		for (i = 0; i < 100; i++) {
			as = astr_append(as, long_str);
		}
		as = astr_free(as);

		as = astr_printf("%s", long_str);
		for (i = 0; i < 100; i++) {
			as = astr_append(as, long_str);
		}
		as = astr_free(as);
	}
}

void test_tok(void) {
	char *delims = " \t\r\n";
	char *tok1 = "ABC";
	char *tok2 = "EFG";
	char *tok3 = "IJK";
	char *str1 = "ABC EFG IJK";
	char *str2 = "   ABC EFG IJK";
	char *str3 = "ABC EFG IJK   ";
	char *str4 = " \t \r\n ABC EFG IJK\r\n \t    ";
	char *tokens[] = {"ABC", "EFG", "IJK"};
	astr *as, *as1, *as2, *as3, *as4;

	// Tokenize str1
	as = astr_create(str1);
	as1 = astr_tok(as, delims);
	aut_assert("str1 as1 not NULL", as1 != NULL);
	aut_assert("str1 as1 correct", strcmp(as1->string, tok1) == 0);
	as2 = astr_tok(as, delims);
	aut_assert("str1 as2 not NULL", as2 != NULL);
	aut_assert("str1 as2 correct", strcmp(as2->string, tok2) == 0);
	as3 = astr_tok(as, delims);
	aut_assert("str1 as3 not NULL", as3 != NULL);
	aut_assert("str1 as3 correct", strcmp(as3->string, tok3) == 0);
	as4 = astr_tok(as, delims);
	aut_assert("str1 as4 IS NULL", as4 == NULL);
	as = astr_free(as);
	as1 = astr_free(as1);
	as2 = astr_free(as2);
	as3 = astr_free(as3);
	as4 = astr_free(as4);

	// Tokenize str2
	as = astr_create(str2);
	as1 = astr_tok(as, delims);
	aut_assert("str2 as1 not NULL", as1 != NULL);
	aut_assert("str2 as1 correct", strcmp(as1->string, tok1) == 0);
	as2 = astr_tok(as, delims);
	aut_assert("str2 as2 not NULL", as2 != NULL);
	aut_assert("str2 as2 correct", strcmp(as2->string, tok2) == 0);
	as3 = astr_tok(as, delims);
	aut_assert("str2 as3 not NULL", as3 != NULL);
	aut_assert("str2 as3 correct", strcmp(as3->string, tok3) == 0);
	as4 = astr_tok(as, delims);
	aut_assert("str2 as4 IS NULL", as4 == NULL);
	as = astr_free(as);
	as1 = astr_free(as1);
	as2 = astr_free(as2);
	as3 = astr_free(as3);
	as4 = astr_free(as4);

	// Tokenize str3
	as = astr_create(str3);
	as1 = astr_tok(as, delims);
	aut_assert("str3 as1 not NULL", as1 != NULL);
	aut_assert("str3 as1 correct", strcmp(as1->string, tok1) == 0);
	as2 = astr_tok(as, delims);
	aut_assert("str3 as2 not NULL", as2 != NULL);
	aut_assert("str3 as2 correct", strcmp(as2->string, tok2) == 0);
	as3 = astr_tok(as, delims);
	aut_assert("str3 as3 not NULL", as3 != NULL);
	aut_assert("str3 as3 correct", strcmp(as3->string, tok3) == 0);
	as4 = astr_tok(as, delims);
	aut_assert("str3 as4 IS NULL", as4 == NULL);
	as = astr_free(as);
	as1 = astr_free(as1);
	as2 = astr_free(as2);
	as3 = astr_free(as3);
	as4 = astr_free(as4);

	// Tokenize str4
	as = astr_create(str4);
	as1 = astr_tok(as, delims);
	aut_assert("str4 as1 not NULL", as1 != NULL);
	aut_assert("str4 as1 correct", strcmp(as1->string, tok1) == 0);
	as2 = astr_tok(as, delims);
	aut_assert("str4 as2 not NULL", as2 != NULL);
	aut_assert("str4 as2 correct", strcmp(as2->string, tok2) == 0);
	as3 = astr_tok(as, delims);
	aut_assert("str4 as3 not NULL", as3 != NULL);
	aut_assert("str4 as3 correct", strcmp(as3->string, tok3) == 0);
	as4 = astr_tok(as, delims);
	aut_assert("str4 as4 IS NULL", as4 == NULL);
	as = astr_free(as);
	as1 = astr_free(as1);
	as2 = astr_free(as2);
	as3 = astr_free(as3);
	as4 = astr_free(as4);
}

void test_split(void) {
	char *delims = " \t\r\n";
	char *tok1 = "ABC";
	char *tok2 = "EFG";
	char *tok3 = "IJK";
	char *str1 = "ABC EFG IJK";
	char *str2 = "   ABC EFG IJK";
	char *str3 = "ABC EFG IJK   ";
	char *str4 = " \t \r\n ABC EFG IJK\r\n \t    ";
	char *tokens[] = {"ABC", "EFG", "IJK"};
	astr *as, *as1, *as2, *as3, *as4;
	astr **asa;

	// Split str1
	as = astr_create(str1);
	asa = astr_split(as, delims);

	aut_assert("str1 asa[0] not NULL", asa[0] != NULL);
	aut_assert("str1 asa[0] correct", strcmp((asa[0])->string, tok1) == 0);
	aut_assert("str1 asa[1] not NULL", asa[1] != NULL);
	aut_assert("str1 asa[1] correct", strcmp((asa[1])->string, tok2) == 0);
	aut_assert("str1 asa[2] not NULL", asa[2] != NULL);
	aut_assert("str1 asa[2] correct", strcmp((asa[2])->string, tok3) == 0);
	aut_assert("str1 asa[3] IS NULL", asa[3] == NULL);

	as = astr_free(as);
	for(int i = 0; asa[i] != NULL; i++) {
		asa[i] = astr_free(asa[i]);
		free(asa[i]);
	}

	// Split str2
	as = astr_create(str2);
	asa = astr_split(as, delims);

	aut_assert("str2 asa[0] not NULL", asa[0] != NULL);
	aut_assert("str2 asa[0] correct", strcmp((asa[0])->string, tok1) == 0);
	aut_assert("str2 asa[1] not NULL", asa[1] != NULL);
	aut_assert("str2 asa[1] correct", strcmp((asa[1])->string, tok2) == 0);
	aut_assert("str2 asa[2] not NULL", asa[2] != NULL);
	aut_assert("str2 asa[2] correct", strcmp((asa[2])->string, tok3) == 0);
	aut_assert("str2 asa[3] IS NULL", asa[3] == NULL);

	as = astr_free(as);
	for(int i = 0; asa[i] != NULL; i++) {
		asa[i] = astr_free(asa[i]);
		free(asa[i]);
	}

	// Split str3
	as = astr_create(str3);
	asa = astr_split(as, delims);

	aut_assert("str3 asa[0] not NULL", asa[0] != NULL);
	aut_assert("str3 asa[0] correct", strcmp((asa[0])->string, tok1) == 0);
	aut_assert("str3 asa[1] not NULL", asa[1] != NULL);
	aut_assert("str3 asa[1] correct", strcmp((asa[1])->string, tok2) == 0);
	aut_assert("str3 asa[2] not NULL", asa[2] != NULL);
	aut_assert("str3 asa[2] correct", strcmp((asa[2])->string, tok3) == 0);
	aut_assert("str3 asa[3] IS NULL", asa[3] == NULL);

	as = astr_free(as);
	for(int i = 0; asa[i] != NULL; i++) {
		asa[i] = astr_free(asa[i]);
		free(asa[i]);
	}

	// Split str4
	as = astr_create(str4);
	asa = astr_split(as, delims);

	aut_assert("str4 asa[0] not NULL", asa[0] != NULL);
	aut_assert("str4 asa[0] correct", strcmp((asa[0])->string, tok1) == 0);
	aut_assert("str4 asa[1] not NULL", asa[1] != NULL);
	aut_assert("str4 asa[1] correct", strcmp((asa[1])->string, tok2) == 0);
	aut_assert("str4 asa[2] not NULL", asa[2] != NULL);
	aut_assert("str4 asa[2] correct", strcmp((asa[2])->string, tok3) == 0);
	aut_assert("str4 asa[3] IS NULL", asa[3] == NULL);

	as = astr_free(as);
	for(int i = 0; asa[i] != NULL; i++) {
		asa[i] = astr_free(asa[i]);
		free(asa[i]);
	}
}

// ----------

int main(int argc, char *argv[]) {
	astr *as = NULL;
	aut_initialize_suite();
	aut_run_test(test_creation);
	aut_run_test(test_reinitialization_without_creation);
	aut_run_test(test_reinitialization);
	aut_run_test(test_append);
	aut_run_test(test_printf);
	aut_run_test(test_printf_long);
	aut_run_test(test_printf_and_append);
	aut_run_test(test_tok);
	aut_run_test(test_split);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
