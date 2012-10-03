// test_astr_create.c - test the creation and reinitialization functions

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

// ----------

int main(int argc, char *argv[]) {
	aut_initialize_suite();
	aut_run_test(test_creation);
	aut_run_test(test_reinitialization_without_creation);
//	aut_run_test(test_reinitialization);
//	aut_run_test(test_append);
	aut_report();
	aut_terminate_suite();
	aut_return();
}
