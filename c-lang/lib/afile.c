// afile.c - Adept File 

/*
 * File processing functions.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "astr.h"
#include "afile.h"

const char *default_open_modes = "r"; // readonly mode
const size_t default_buffer_size = 16 * 1024; // 16KB buffer size
const int default_buffering_mode = _IOLBF; // line buffering mode

/*
 * afile_create
 *
 * Create an afile instance with default parameters.
 *
 * Parameter: The file specification (optional path, file name)
 * Parameter: The file open modes
 */
afile *afile_create(astr *filespec, astr *open_modes) {
	afile *af = NULL;

	af = calloc(1, sizeof(afile));

	af->file = NULL;
	afile_set_filespec(af, filespec);
	afile_set_open_modes(af, open_modes);
	afile_set_buffer_size(af, default_buffer_size);
	afile_set_buffering_mode(af, default_buffering_mode);

	return af;
}

/*
 * afile_create_explicit
 *
 * Create an afile instance with the specified explicit parameters.
 *
 * Parameter: The file specification (optional path, file name)
 * Parameter: The file open modes
 * Parameter: The file buffer size
 * Parameter: The file buffering mode
 * Returns:   Pointer to the afile instance
 */
afile *afile_create_explicit(astr *filespec, astr *open_modes, size_t buffer_size, int buffering_mode) {
	afile *af = NULL;

	af = calloc(1, sizeof(afile));

	af->file = NULL;
	afile_set_filespec(af, filespec);
	afile_set_open_modes(af, open_modes);
	afile_set_buffer_size(af, buffer_size);
	afile_set_buffering_mode(af, buffering_mode);

	return af;
}

/*
 * afile_set_filespec
 *
 * Initialize an afile file specification.
 *
 * This can only be set before the file is opened.
 *
 * Parameter: The afile instance
 * Parameter: The file specification (optional path, file name)
 */
void afile_set_filespec(afile *af, astr *filespec) {
	if (af != NULL && af->file == NULL) {
		if (filespec != NULL && filespec->string != NULL) {
			af->filespec = astr_copy(filespec);
		}
	}
}

/*
 * afile_set_open_modes
 *
 * Initialize an afile open mode string.
 *
 * This can only be set before the file is opened.
 *
 * Parameter: The afile instance
 * Parameter: The file open modes
 */
void afile_set_open_modes(afile *af, astr *open_modes) {
	if (af != NULL && af->file == NULL) {
		if (!astr_is_empty(open_modes)) {
			af->open_modes = astr_copy(open_modes);
		}
		else {
			af->open_modes = astr_create(default_open_modes);
		}
	}
}

/*
 * afile_set_buffering_mode
 *
 * Initialize an afile buffering mode.
 *
 * This can only be set before the file is opened.
 *
 * Parameter: The afile instance
 * Parameter: The file buffering mode
 */
void afile_set_buffering_mode(afile *af, int buffering_mode) {
	if (af != NULL && af->file == NULL) {
		if (buffering_mode == _IOLBF || buffering_mode == _IOFBF || buffering_mode == _IONBF) {
			af->buffering_mode = buffering_mode;
		}
		else {
			af->buffering_mode = default_buffering_mode;
		}
	}
}

/*
 * afile_set_buffer_size
 *
 * Initialize an afile buffer size.
 *
 * This can only be set before the file is opened.
 *
 * Parameter: The afile instance
 * Parameter: The file buffer size
 */
void afile_set_buffer_size(afile *af, size_t size) {
	if (af != NULL && af->file == NULL) {
		if (size > 0) {
			af->buffer_size = size;
		}
		else {
			af->buffer_size = default_buffer_size;
		}
	}
}

/*
 * afile_stat
 *
 * Stat an afile.
 *
 * Parameter: The afile instance
 * Returns:   0 = success, non-zero = errno value from the failed operation
 */
int afile_stat(afile *af) {
	int result = 0;

	if (af != NULL && af->filespec != NULL) {
		result = stat(af->filespec->string, &(af->stats));
		if(result == -1) {
			result = errno;
		} 	
	}
	
	return result;
}

/*
 * afile_exists
 *
 * Determiine if an afile exists.
 *
 * Parameter: The afile instance
 * Returns:   0 = file does not exist, 1 = file exists.
 */
int afile_exists(afile *af) {
	int result = 0;

	result = afile_stat(af);
	if (result == ENOENT) {
		return 0;
	}
	else {
		return 1;
	}
}

/*
 * afile_open
 *
 * Open the file.
 *
 * Parameter: The afile instance
 * Returns:   0 = success, non-zero = errno value from the failed operation
 */
int afile_open(afile *af) {
	int result = 0;

	if (af != NULL && !astr_is_empty(af->filespec) && !astr_is_empty(af->open_modes) && af->file == NULL) {
		af->file = fopen(af->filespec->string, af->open_modes->string);
		if (af->file == NULL) {
			result = errno;
			printf("Error on open: %s (%d)\n", strerror(result), result);
		}
	}

	if (af->file != NULL) {
		result = setvbuf(af->file, NULL, af->buffering_mode, af->buffer_size);
		if (result != 0) {
			fclose(af->file);
			printf("Error on setvbuf: %s (%d)\n", strerror(result), result);
		}
	}

	if (af->file != NULL && result == 0) {
		af->buffer = (char *)calloc(af->buffer_size, sizeof(char));
		if (af->buffer == NULL) {
			result = errno;
			fclose(af->file);
			printf("Error on calloc of %ld bytes: %s (%d)\n", af->buffer_size * sizeof(char), strerror(result), result);
		}
	}

	return result;
}

/*
 * afile_close
 *
 * Close the file.
 *
 * Parameter: The afile instance
 * Returns:   0 = success, non-zero = errno value from the failed operation
 */
int afile_close(afile *af) {
	int result = 0;

	if (af != NULL) {
		if (af->file != NULL) {
			result = fclose(af->file);
			if (result != 0) {
				result = errno;
			}
			af->file = NULL;
		}

		if (af->buffer != NULL) {
			free(af->buffer);
			af->buffer = NULL;
		}
	}

	return result;
}

/*
 * afile_free
 *
 * Free the memory allocated for the file.
 *
 * Parameter: The afile instance
 */
afile *afile_free(afile *af) {
	if (af != NULL) {
		if (af->file != NULL) {
			afile_close(af);
		}

		if (af->buffer != NULL) {
			free(af->buffer);
			af->buffer = NULL;
		}

		if (af->open_modes != NULL) {
			astr_free(af->open_modes);
			af->open_modes = NULL;
		}

		if (af->filespec != NULL) {
			astr_free(af->filespec);
			af->filespec = NULL;
		}

		free(af);
	}

	return NULL;
}

/*
 * afile_process_lines
 *
 * Process all lines from a file.
 *
 * Read lines from a file and call the specified function to process each line.
 *
 * We have to use an intermediate buffer to read the bytes of the file into.
 * It is not possible to know the length of the strings being read, so the
 * intermediate buffer is allocated to be the size of the file buffer.  This
 * is much larger than necessary.  There is room here for adjustment.  Perhaps
 * 1/4th the size, or some other fraction, can be used.
 *
 * Parameter: The afile instance, opened
 * Parameter: A pointer to a function that will process one line of text
 * Returns:   The number of lines processed
 */
int afile_process_lines(afile *af, int (*process)(astr *as)) {
	int result = 0;
	int line_count = 0;
	char *buffer = NULL;
	astr *work = NULL;

	if (af != NULL && af->file != NULL && process != NULL) {
		buffer = (char *)calloc(af->buffer_size, 1);
		while (fgets(buffer, af->buffer_size, af->file)) {
			work = astr_set(work, buffer);
			line_count++;
			process(work);
		}
	}
	return line_count;
}

/*
 * afile_process_matching_lines
 *
 * Process matching lines from a file.
 *
 * Read lines from a file and call the specified function to process each line.
 *
 * Parameter: The afile instance, opened
 * Parameter: A pointer to a function that will match one line of text
 * Parameter: A pointer to a function that will process one line of text
 * Returns:   The number of lines processed
 */
int afile_process_matching_lines(afile *af, int (*match)(astr *as), int (*process)(astr *as)) {
	int result = 0;
	int line_count = 0;
	char *buffer = NULL;
	astr *work = NULL;

	if (match == NULL) {
		return afile_process_lines(af, process);
	}

	if (af != NULL && af->file != NULL) {
		buffer = (char *)calloc(af->buffer_size, 1);
		while (fgets(buffer, af->buffer_size, af->file)) {
			work = astr_set(work, buffer);
			if (match(work)) {
				line_count++;
				process(work);
			}
		}
	}
	return line_count;
}

/*
 * afile_print
 *
 * Print an afile instance in labeled string format.
 *
 * Parameter: The afile instance
 */
char *afile_print(const afile *af) {
	char *struct_print;
	char *print;
	char eol[3];

	struct_print = afile_print_struct(af);
	sprintf(eol, "%s", "\n");
	print = calloc(strlen(struct_print) + strlen(eol) + 1, sizeof(char));
	if (print != NULL) {
		strcpy(print, struct_print);
		strcat(print, eol);
	}
	free(struct_print);
	return print;
}

/*
 * afile_print_struct
 * 
 * Print an afile structure.
 * Label: NNNNNNNNN\n
 *
 * Parameter: The afile instance
 */
char *afile_print_struct(const afile *af) {
	char *lbl_filespec       = "filespec:       ";
	char *lbl_open_modes     = "open_modes:     ";
	char *lbl_buffering_mode = "buffering_mode: ";
	char *lbl_buffer_size    = "buffer_size:    ";
	char *lbl_buffer         = "buffer pointer: ";
	int length;
	char *string = NULL;

	if (af != NULL) {
		length = strlen(lbl_filespec) + strlen(lbl_open_modes) + strlen(lbl_buffering_mode) + strlen(lbl_buffer_size);
		length += 110; // Guess at the length of the
		string = calloc(length, sizeof(char));
		sprintf(string, "%s%s\n%s%s\n%s%d\n%s%ld\n%s%p",
			lbl_filespec, af->filespec->string, lbl_open_modes, af->open_modes->string,
			lbl_buffering_mode, af->buffering_mode, lbl_buffer_size, af->buffer_size, lbl_buffer, af->buffer);
	}
	return string;
}
