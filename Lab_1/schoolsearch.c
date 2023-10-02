/* 
 * File:   schoolsearch.c
 * Author: Ameer Arsala
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schoolsearch.h"

const struct Name NO_NAME = {NULL, NULL};
const Student NULL_STUDENT = {.studentName = NO_NAME, 
                              .grade = -1,
                              .classroom = -1
                              .bus = -1,
                              .gpa = -1.0,
                              .teacherName = NO_NAME };

const CommandArg CMD_STUDENT = {'S', "Student"};
const CommandArg CMD_TEACHER = {'T', "Teacher"};
const CommandArg CMD_BUS = {'B', "Bus"};
const CommandArg CMD_GRADE = {'G', "Grade"};
const CommandArg ARG_HIGH = {'H', "High"};
const CommandArg ARG_LOW = {'L', "Low"};
const CommandArg CMD_AVERAGE = {'A', "Average"};
const CommandArg CMD_INFO = {'I', "Info"};
const CommandArg CMD_QUIT = {'Q', "Quit"};

Student MakeQuery(Student studentInfo) {

}

struct Name MakeName(struct Name name) {
    struct Name nameFinal = NO_NAME
}

char* toString(Student* studentInfo) {
    Student student = *studentInfo;
    
    char
}