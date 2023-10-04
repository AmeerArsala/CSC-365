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
#include "utils.h"

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

StudentList* studentList;
int finished = 0;

/* Initialize the list of students from the file */
void initsearch(char* file) {
    studentList = malloc(sizeof(StudentList));
    *studentList = fromString(file);
    finished = 0;
}

void cleanup() {
    free(studentList);
}

/*
 * Queries are just incomplete students
 * Every student has a complete set of attributes; they cannot have anything be like NULL_STUDENT
 * Therefore, this function will standardize that to enforce that 'constraint'
 */
Student MakeQuery(Student studentInfo) {
    /* set by value, not reference */
    Student query = NULL_STUDENT;
    
    query.studentName = MakeName(studentInfo.studentName);
    
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
    
    query.teacherName = MakeName(studentInfo.teacherName);
    
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
           student.grade, student.classroom, student.bus, student.gpa,
           student.teacherName.lastName, student.teacherName.firstName);
    
    return str;
}

Student fromLine(char* line) {
    StringArray* row = malloc(sizeof(StringArray));
    
    *row = splitString(line, ',');
    
    Student student = {
        .studentName = {.firstName = row->array[0], .lastName = row->array[1]},
        .grade = atoi(row->array[2]),
        .classroom = atoi(row->array[3]),
        .bus = atoi(row->array[4]),
        .gpa = atof(row->array[5]),
        .teacherName = {.firstName = row->array[6], .lastName = row->array[7]}
    };
    
    free(row);
    
    return student;
}

StudentList fromString(char* str) {
    StringArray* lines = malloc(sizeof(StringArray));
    
    *lines = splitString(str, '\n');
    
    /* num students = *lines.size */
    Student students[lines->size];
    for (size_t i = 0; i < lines->size; ++i) {
        students[i] = fromLine(lines->array[i]);
    }
    
    free(lines);
    
    StudentList studentList = {students, lines->size};
    
    return studentList;
}

int namesEqual(struct Name n1, struct Name n2) {
    int equal = (strcmp(n1.lastName, n1.lastName) == 0) && (strcmp(n1.firstName, n2.firstName) == 0);
    
    return equal;
}

int studentsEqual(Student s1, Student s2) {
    int equal = namesEqual(s1.studentName, s2.studentName);
    /* Treating last name first name as the primary key; maybe modify this */
    
    return equal;
}

/* Incomplete Info = standardized partial info */
int matchesStudent(Student incompleteInfo, Student potentialMatch) {
    
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

/* Info can be incomplete */
StudentList findStudentsByInfo(Student partialInfo) {
    Student students[studentList->size];
    Student studentInfo = MakeQuery(partialInfo);
    
    size_t s = 0;
    for(size_t i = 0; i < studentList->size; ++i) {
        if (matchesStudent(studentInfo, studentList->array[i])) {
            students[s++] = studentList->array[i];
        }
    }
    
    StudentList studentList = {students, s};
    return studentList;
}

/* Commands */
int isCommandArg(char* str, CommandArg cmdArg) {
    return (str[0] == cmdArg.shorthand && str[1] == ':') || strcmp(substring(str, 0, strlen(str) - 1, 1), cmdArg.full) == 0;
}

static void student_cmd(char* lastName) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.studentName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery); 
    
    /* For each student/entry found, print:
     * last name, first name
     * grade, classroom assignment
     * the name of their teacher (last and first name). 
     */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Grade: %d; Classroom: %d; Teacher: %s,%s", 
                result.studentName.lastName, result.studentName.firstName,
                result.grade, result.classroom,
                result.teacherName.lastName, result.teacherName.firstName);
    }
    
    free(results);
}

static void student_bus_cmd(char* lastName) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.studentName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery);
    
    /* For each entry found, print the last name, first name and the bus route the student takes. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Bus Route: %d", 
                result.studentName.lastName, result.studentName.firstName,
                result.bus);
    }
    
    free(results);
}

static void teacher_cmd(char* lastName) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.teacherName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery); 
    
    /* For each entry found, print the last and the first name of the student. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s", result.studentName.lastName, result.studentName.firstName);
    }
    
    free(results);
}

static void bus_cmd(int bus) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.bus = bus};
    *results = findStudentsByInfo(searchQuery); 
    
    /* For each such entry, output the name of the student (last and first) and their grade and classroom. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Grade: %d; Classroom: %d", 
                result.studentName.lastName, result.studentName.firstName,
                result.grade, result.classroom);
    }
    
    free(results);
}

static void grade_cmd(int grade) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery); 
    
    /* For each entry, output the name (last and first) of the student. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s", result.studentName.lastName, result.studentName.firstName);
    }
    
    free(results);
}

static void grade_high_cmd(int grade) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery); 
    
    Student entry = results->array[0];
    for (size_t i = 1; i < results->size; ++i) {
        Student result = results->array[i];
        if (result.gpa > entry.gpa) {
            entry = result;
        }
    }
    
    /* Report the contents of the highest GPA entry (name of the student, GPA, teacher, bus route). */
    printf("%s,%s; GPA: %f; Teacher: %s,%s; Bus Route: %d", 
            entry.studentName.lastName, entry.studentName.firstName,
            entry.gpa,
            entry.teacherName.lastName, entry.teacherName.firstName,
            entry.bus);
    
    free(results);
}

static void grade_low_cmd(int grade) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery); 
    
    Student entry = results->array[0];
    for (size_t i = 1; i < results->size; ++i) {
        Student result = results->array[i];
        if (result.gpa < entry.gpa) {
            entry = result;
        }
    }
    
    /* Report the contents of the lowest GPA entry (name of the student, GPA, teacher, bus route). */
    printf("%s,%s; GPA: %f; Teacher: %s,%s; Bus Route: %d", 
            entry.studentName.lastName, entry.studentName.firstName,
            entry.gpa,
            entry.teacherName.lastName, entry.teacherName.firstName,
            entry.bus);
    
    free(results);
}

static void average_cmd(int grade) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery); 
    
    double average = 0;
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        average += result.gpa;
    }
    
    average /= results->size;
    
    /* Compute the average GPA score for the entries found. Output the grade level (the number provided in command) and the average GPA score computed. */
    printf("Grade Level: %d => Average: %f", grade, average);
    
    free(results);
}

static void info_cmd() {
    for (size_t i = 0; i <= 6; ++i) {
        StudentList* results = malloc(sizeof(StudentList));
        
        /* Search for students */
        Student searchQuery = {.grade = i};
        *results = findStudentsByInfo(searchQuery);
        
        /* Report number of students per grade */
        printf("%d: %d", i, (int)results->size);
        
        free(results);
    }
}

static void quit_cmd() {
    finished = 1;
    cleanup();
}

void interpretCommand(char* cmd) {
    StringArray* line = malloc(sizeof(StringArray));
    
    *line = splitString(cmd, ' '); /* split by space */
    
    /* Student */
    if (isCommandArg(line->array[0], CMD_STUDENT)) {
        char* lastName = line->array[1];
        if (line->size == 2) {
            student_cmd(lastName);
        } else { /* line->size == 3 */
            if (isCommandArg(line->array[2], CMD_BUS)) {
                student_bus_cmd(lastName);
            }
        }
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
    
    free(line);
}

