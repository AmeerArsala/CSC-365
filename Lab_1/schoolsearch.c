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
#include "utils.h";

const struct Name NO_NAME = {NULL, NULL};
const Student NULL_STUDENT = {.studentName = NO_NAME, 
                              .grade = -1,
                              .classroom = -1,
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

/*
 * Queries are just incomplete students
 * Every student has a complete set of attributes; they cannot have anything be like NULL_STUDENT
 * Therefore, this function will standardize that to enforce that 'constraint'
 */
Student MakeQuery(Student studentInfo) {
    /* set by value, not reference */
    Student query = NULL_STUDENT;
    
    if (studentInfo.studentName) {
        query.studentName = studentInfo.studentName;
    }
    
    if (studentInfo.grade) {
        query.grade = studentInfo.grade;
    }
    
    if (studentInfo.classroom) {
        query.classroom = studentInfo.classroom;
    }
    
    if (studentInfo.bus) {
        query.bus = studentInfo.bus;
    }
    
    if (studentInfo.gpa) {
        query.gpa = studentInfo.gpa;
    }
    
    if (studentInfo.teacherName) {
        query.teacherName = studentInfo.teacherName;
    }
    
    /*query.studentName = studentInfo.studentName ? studentInfo.studentName : NULL_STUDENT.studentName;
    query.grade = studentInfo.grade ? studentInfo.grade : NULL_STUDENT.grade;
    query.classroom = studentInfo.classroom ? studentInfo.classroom : NULL_STUDENT.classroom;
    query.bus = studentInfo.bus ? studentInfo.bus : NULL_STUDENT.bus;
    query.gpa = studentInfo.gpa ? studentInfo.gpa : NULL_STUDENT.gpa;
    query.teacherName = studentInfo.teacherName ? studentInfo.teacherName : NULL_STUDENT.teacherName;*/
    
    return query;
}

struct Name MakeName(struct Name name) {
    struct Name nameFinal;
    
    nameFinal.firstName = name.firstName ? name.firstName : NO_NAME.firstName;
    nameFinal.lastName = name.lastName ? name.lastName : NO_NAME.lastName;
    
    return nameFinal;
}

char* toString(Student* studentInfoPtr) {
    Student student = *studentInfoPtr;
    
    char* str = "%s,%s,%d,%d,%d,%f,%s,%s";
    
    snprintf(str, (strlen(str) + 1),
           student.studentName.lastName, student.studentName.firstName,
           student.grade, student.classroom, student.bus, student.gpa
           student.teacherName.lastName, student.teacherName.firstName);
    
    return str;
}

Student fromLine(char* line) {
    StringArray* row;
    
    row = &splitString(line, ',');
    
    Student student = {
        .studentName = {.firstName = row->array[0], .lastName = row->array[1]},
        .grade = atoi(row->array[2]),
        .classroom = atoi(row->array[3]),
        .bus = atoi(row->array[4]),
        .gpa = atof(row->array[5]),
        .teacherName = {.firstName = row->array[6], .lastName = row->array[7]}
    };
    
    return student;
}

StudentList fromString(char* lines) {
    StringArray* lines;
    
    lines = &splitString(lines, '\n');
    
    /* num students = *lines.size */
    Student students[lines->size];
    for (size_t i = 0; i < lines->size; ++i) {
        students[i] = fromLine(lines->array[i]);
    }
    
    StudentList studentList = {students, lines->size};
    
    return studentList;
}

int namesEqual(Name n1, Name n2) {
    int equal = (strcmp(n1.lastName, n1.lastName) == 0) && (strcmp(n1.firstName, n2.firstName) == 0);
    
    return equal;
}

int studentsEqual(Student s1, Student s2) {
    int equal = namesEqual(s1.studentName, s2.studentName);
    /* Treating last name first name as the primary key; maybe modify this */
    
    return equal;
}

int matchesStudent(Student incompleteInfo, Student potentialMatch) {
    incompleteInfo = MakeQuery(incompleteInfo);
    
    if (incompleteInfo.studentName.firstName != NO_NAME.firstName && strcmp(incompleteInfo.studentName.firstName, potentialMatch.studentName.firstName) != 0) {
        return 0;
    }
    
    if (incompleteInfo.studentName.lastName != NO_NAME.lastName && strcmp(incompleteInfo.studentName.lastName, potentialMatch.studentName.lastName) != 0) {
        return 0;
    }
    
    if (incompleteInfo.grade != NULL_STUDENT.grade && incompleteInfo.grade != potentialMatch.grade) {
        return 0;
    }
    
    if (incompleteInfo.classroom != NULL_STUDENT.classroom && incompleteInfo.classroom != potentialMatch.classroom) {
        return 0;
    }
    
    if (incompleteInfo.bus != NULL_STUDENT.bus && incompleteInfo.bus != potentialMatch.bus) {
        return 0;
    }
    
    if (incompleteInfo.gpa != NULL_STUDENT.gpa && incompleteInfo.gpa != potentialMatch.gpa) {
        return 0;
    }
    
    if (incompleteInfo.teacherName.firstName != NO_NAME.firstName && strcmp(incompleteInfo.teacherName.firstName, potentialMatch.teacherName.firstName) != 0) {
        return 0;
    }
    
    if (incompleteInfo.teacherName.lastName != NO_NAME.lastName && strcmp(incompleteInfo.teacherName.lastName, potentialMatch.teacherName.lastName) != 0) {
        return 0;
    }
    
    return 1;
}

StudentList findStudentsByInfo(Student* studentInfoPtr, StudentList* studentList) {
    Student students[studentList->size];
    
    size_t s = 0;
    for(size_t i = 0; i < studentList->size; ++i) {
        if (matchesStudent(*studentInfoPtr, studentList->array[i])) {
            students[s++] = studentList->array[i];
        }
    }
    
    StudentList studentList = {students, s};
    return studentList;
}

/* Commands */
int isCommandArg(char* str, CommandArg cmdArg) {
    return (str[0] == cmdArg.shorthand && str[1] == ':') || strcmp(substring(str, 0, strlen(str) - 1), cmdArg.full) == 0;
}

void interpretCommand(char* cmd, StudentList* studentList) {
    StringArray* line;
    
    line = &splitString(cmd, ' '); /* split by space */
    
    /* Student */
    if (isCommandArg(line->array[0], CMD_STUDENT)) {
        int* bus;
        if (line->size == 3) {
            bus = atoi(line->array[2]);
        } else { /* line->size == 2 */
            bus = NULL;
        }
        
        student_cmd(line->array[1], bus);
    }
    
    /* Teacher */
    if (isCommandArg(line->array[0], CMD_TEACHER)) {
        teacher_cmd(line->array[1]);
    }
    
    /* Bus */
    if (isCommandArg(line->array[0], CMD_BUS)) {
        bus_cmd(atoi(line->array[1]));
    }
    
    /* Grade */
    if (isCommandArg(line->array[0], CMD_GRADE)) {
        int grade = atoi(line->array[1]);
        if (line->size == 3) {
            if (isCommandArg(line->array[2], ARG_HIGH)) {
                grade_high_cmd(grade);
            } else if (isCommandArg(line->array[2], ARG_LOW)) {
                grade_low_cmd(grade);
            }
        } else { /* line->size == 2 */
            grade_cmd(grade);
        }
    }
    
    /* Average */
    if (isCommandArg(line->array[0], CMD_AVERAGE)) {
        bus_cmd(atoi(line->array[1]));
    }
    
    /* Info */
    if (isCommandArg(line->array[0], CMD_INFO)) {
        info_cmd();
    }
    
    /* Quit */
    if (isCommandArg(line->array[0], CMD_QUIT)) {
        quit_cmd();
    }
}

void student_cmd(char* lastName, int* bus) {}
void teacher_cmd(char* lastName) {}
void bus_cmd(int bus) {}
void grade_cmd(int grade) {}
void grade_high_cmd(int grade) {}
void grade_low_cmd(int grade) {}
void average_cmd(int grade) {}
void info_cmd() {}
void quit_cmd() {}