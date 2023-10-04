/* 
 * File:   utils.h
 * Author: Ameer Arsala
 *
 * Created on October 2, 2023, 9:08 PM
 */

#ifndef UTILS_H
#define UTILS_H

#define INITIAL_CAPACITY 10

typedef struct {
    char **array;
    size_t size;
    size_t capacity;
} StringArray;

void initStringArray(StringArray *arr);
void addString(StringArray *arr, const char *str);
void printStringArray(const StringArray *arr);
void freeStringArray(StringArray *arr);

StringArray splitString(const char* input, const char delimiter);

char* substringFrom(const char* str, int start, int length);
char* substring(const char* str, int start, int end, int length);

#endif /* UTILS_H */

