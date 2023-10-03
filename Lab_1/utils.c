/* 
 * File:   utils.c
 * Author: Ameer Arsala
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void initStringArray(StringArray *arr) {
    arr->array = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
    arr->size = 0;
    arr->capacity = INITIAL_CAPACITY;
}

void addString(StringArray *arr, const char *str) {
    if (arr->size == arr->capacity) {
        /* Resize the array if needed */
        arr->capacity *= 2;
        arr->array = (char **)realloc(arr->array, arr->capacity * sizeof(char *));
    }

    /* Duplicate and add the string */
    arr->array[arr->size++] = strdup(str);
}

void printStringArray(const StringArray *arr) {
    for (size_t i = 0; i < arr->size; ++i) {
        printf("String %zu: %s\n", i + 1, arr->array[i]);
    }
}

void freeStringArray(StringArray *arr) {
    for (size_t i = 0; i < arr->size; ++i) {
        free(arr->array[i]);
    }
    free(arr->array);
}

StringArray splitString(const char* input, const char delimiter) {
    StringArray strList;
    initStringArray(&strList);
    
    char *token;
    char *str = strdup(input); /* Duplicate the input string since strtok_r modifies the original */
    char *saveptr;
    
    token = strtok_r(str, delimiter, &saveptr);
    
    /* Keep getting tokens while one is found */
    while (token != NULL) {
        addString(&strList, token);
        
        /* next token */
        token = strtok_r(NULL, delimiter, &saveptr);
    }
    
    /* Free duplicated string */
    free(str);
    
    return strList;
}

char* substring(const char* str, int start, int length) {
    /* Check for invalid inputs */
    if (start < 0 || length < 0 || start + length > strlen(str)) {
        return NULL;
    }

    /* Allocate memory for the substring */
    char* result = (char*)malloc((length + 1) * sizeof(char));  // +1 for null terminator

    /* Copy the substring */
    strncpy(result, str + start, length);

    /* Null-terminate the result */
    result[length] = '\0';

    return result;
}

char* substring(const char* str, int start, int end, int length) {
    /* Calculate the actual end index based on end or */
    if (end >= 0) {
        if (end < start) {
            return NULL; /* Invalid range */
        }
        length = end - start + 1;
    } else if (length < 0) {
        return NULL; /* Invalid length */
    }

    /* Check for invalid inputs */
    if (start < 0 || length < 0 || start + length > strlen(str)) {
        return NULL;
    }

    /* Allocate memory for the substring */
    char* result = (char*)malloc((length + 1) * sizeof(char));  // +1 for null terminator

    /* Copy the substring */
    strncpy(result, str + start, length);

    /* Null-terminate the result */
    result[length] = '\0';

    return result;
}