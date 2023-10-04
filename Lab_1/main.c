#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "schoolsearch.h"

#define MAX_INPUT_LENGTH 100

/* Size of each input chunk to be
   read and allocate for. */
#ifndef  READALL_CHUNK
#define  READALL_CHUNK  262144
#endif

#define  READALL_OK          0  /* Success */
#define  READALL_INVALID    -1  /* Invalid parameters */
#define  READALL_ERROR      -2  /* Stream error */
#define  READALL_TOOMUCH    -3  /* Too much input */
#define  READALL_NOMEM      -4  /* Out of memory */

/* This function returns one of the READALL_ constants above.
   If the return value is zero == READALL_OK, then:
     (*dataptr) points to a dynamically allocated buffer, with
     (*sizeptr) chars read from the file.
     The buffer is allocated for one extra char, which is NUL,
     and automatically appended after the data.
   Initial values of (*dataptr) and (*sizeptr) are ignored.
*/
int readall(FILE *in, char **dataptr, size_t *sizeptr)
{
    char  *data = NULL, *temp;
    size_t size = 0;
    size_t used = 0;
    size_t n;

    /* None of the parameters can be NULL. */
    if (in == NULL || dataptr == NULL || sizeptr == NULL)
        return READALL_INVALID;

    /* A read error already occurred? */
    if (ferror(in))
        return READALL_ERROR;

    while (1) {

        if (used + READALL_CHUNK + 1 > size) {
            size = used + READALL_CHUNK + 1;

            /* Overflow check. Some ANSI C compilers
               may optimize this away, though. */
            if (size <= used) {
                free(data);
                return READALL_TOOMUCH;
            }

            temp = realloc(data, size);
            if (temp == NULL) {
                free(data);
                return READALL_NOMEM;
            }
            data = temp;
        }

        n = fread(data + used, 1, READALL_CHUNK, in);
        if (n == 0)
            break;

        used += n;
    }

    if (ferror(in)) {
        free(data);
        return READALL_ERROR;
    }

    temp = realloc(data, used + 1);
    if (temp == NULL) {
        free(data);
        return READALL_NOMEM;
    }
    data = temp;
    data[used] = '\0';

    *dataptr = data;
    *sizeptr = used;

    return READALL_OK;
}

void searchschool(char* datafile) {
    //initsearch(datafile);
    StudentList* studentList = fromString(datafile);
    int finished = 0;

    while (!finished) {
        char userInput[MAX_INPUT_LENGTH];

        /* Prompt User */
        printf("Query> ");

        /* Read the string using fgets and print a new line */
        fgets(userInput, sizeof(userInput), stdin);

        /* Remove the newline character at the end (if present) */
        if (userInput[strlen(userInput) - 1] == '\n') {
            userInput[strlen(userInput) - 1] = '\0';
        }

        /* Process the entered string */
        interpretCommand(userInput, studentList, &finished);
    }

    free(studentList);
}

/*
 * 
 */
int main(int argc, char** argv) {
    const char* file_path = "students.txt";
    
    /* Open file */
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open file");  // Output any error message
        return 1;
    }
    
    /* Read file */
    char *data;
    size_t size;
    int result = readall(file, &data, &size);
    if (result != READALL_OK) {
        fprintf(stderr, "Failed to read file, error code: %d\n", result);
        return 1;
    }

    // Now, 'data' points to the file data and 'size' is the size of the data
    // You can use 'data' and 'size' here...
    /* Main operation with the file */
    searchschool(data);
    
    /* Cleanup: close the file and free allocated memory */
    free(data);
    fclose(file);
    
    return 0;
}

