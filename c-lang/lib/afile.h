// afile.h - Adept file handling

#ifndef AFILE_H_INCLUDED
#define AFILE_H_INCLUDED

#include <stdio.h>
#include <sys/stat.h>

#include "astr.h"

/*
 * The afile object provides storage of the attributes of a standard C file, as
 * well as standard operations on files.
 * 
 * Additionally, the object provides a framework for processing some or all of 
 * the lines in a file with a user-specified function.  This is powerful 
 * because a filter-type program can be written without having to code the file
 * handling process. You can code the function that processes each line, and
 * optionally code the function that matches particular lines from the file.
 * The framework will handle reading the file and will call the specified
 * match and processing functions passing the current line from the file.
 *
 * There are no I/O functions defined here.  Use the standard C library
 * functions to perform I/O with the file inside the afile object.
 */

typedef struct afile {
	astr *filespec;
	astr *open_modes;
	int buffering_mode;
	size_t buffer_size;
	char *buffer;
	FILE *file;
	struct stat stats;
} afile;

#ifdef	__cplusplus
extern "C" {
#endif

// ----------------------
// Creation and
// Management

// Create an afile instance with default attributes.
afile *afile_create(astr *filespec, astr *open_modes);

// Create an afile instance with the attributes explicitly specified.
afile *afile_create_explicit(astr *filespec, astr *open_modes, size_t buffer_size, int buffering_mode);

// Create an afile instance for a temporary file with default attributes.
afile *afile_create_temp(astr *open_modes);

// Create an afile instance for a temporary file with the attributes explicitly specified.
afile *afile_create_temp_explicit(astr *open_modes, size_t buffer_size, int buffering_mode);

// Set the afile filespec.
void afile_set_filespec(afile *af, astr *filespec);

// Set the afile open modes.
void afile_set_open_modes(afile *af, astr *open_modes);

// Set the afile buffering mode.
void afile_set_buffering_mode(afile *af, int buffering_mode);

// Set the afile buffer size.
void afile_set_buffer_size(afile *af, size_t size);

// Stat the file.
int afile_stat(afile *af);

// Determine if the file exists.
int afile_exists(afile *af);

// Open the afile.
int afile_open(afile *af);

// Close the afile.
int afile_close(afile *af);

// Free the afile.
afile *afile_free(afile *af);

// ----------------------
// Processing

// Process all lines from the afile.
int afile_process_lines(afile *af, int (*process)(astr *as));

// Process the lines from the afile that satisfy the match function.
int afile_process_matching_lines(afile *af, int (*match)(astr *as), int (*process)(astr *as));

// ----------------------
// Utility

// Print the afile.
char *afile_print(const afile *af);

// Print the afile structure.
char *afile_print_struct(const afile *af);

#ifdef	__cplusplus
}
#endif

#endif	// AFILE_H_INCLUDED
