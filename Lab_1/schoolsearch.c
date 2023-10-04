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

const struct Name NO_NAME = {.firstName = NULL, .lastName = NULL};
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

    if (&name == NULL) {
        return NO_NAME;    
    }
    
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
    StringArray row = splitString(line, ',');

    //printf(line);
    //Student* student = malloc(sizeof(Student));
    
    Student student = {
        .studentName = {.lastName = row.array[0], .firstName = row.array[1]},
        .grade = atoi(row.array[2]),
        .classroom = atoi(row.array[3]),
        .bus = atoi(row.array[4]),
        .gpa = atof(row.array[5]),
        .teacherName = {.lastName = row.array[6], .firstName = row.array[7]}
    };
    
    //freeStringArray(&row);
    //free(row);
    
    return student;
}

StudentList* fromString(char* str) {
    StringArray lines = splitString(str, '\n');
    
    size_t sz = lines.size;
    
    StudentList* studentsList = malloc(sizeof(StudentList));
    studentsList->size = sz;
    for (size_t i = 0; i < sz; ++i) {
        studentsList->array[i] = fromLine(lines.array[i]);
    }
    
    //freeStringArray(&lines);
    
    return studentsList;
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
int matchesStudent(Student* incompleteInfo, Student* potentialMatch) {
    if (incompleteInfo->studentName.firstName != NO_NAME.firstName && strcmp(incompleteInfo->studentName.firstName, potentialMatch->studentName.firstName) != 0) {
        return 0;
    }
    
    if (incompleteInfo->studentName.lastName != NO_NAME.lastName && strcmp(incompleteInfo->studentName.lastName, potentialMatch->studentName.lastName) != 0) {
        return 0;
    }
    
    if (incompleteInfo->grade != NULL_STUDENT.grade && incompleteInfo->grade != potentialMatch->grade) {
        return 0;
    }
    
    if (incompleteInfo->classroom != NULL_STUDENT.classroom && incompleteInfo->classroom != potentialMatch->classroom) {
        return 0;
    }
    
    if (incompleteInfo->bus != NULL_STUDENT.bus && incompleteInfo->bus != potentialMatch->bus) {
        return 0;
    }
    
    if (incompleteInfo->gpa != NULL_STUDENT.gpa && incompleteInfo->gpa != potentialMatch->gpa) {
        return 0;
    }
    
    if (incompleteInfo->teacherName.firstName != NO_NAME.firstName && strcmp(incompleteInfo->teacherName.firstName, potentialMatch->teacherName.firstName) != 0) {
        return 0;
    }
    
    if (incompleteInfo->teacherName.lastName != NO_NAME.lastName && strcmp(incompleteInfo->teacherName.lastName, potentialMatch->teacherName.lastName) != 0) {
        return 0;
    }
    
    return 1;
}

/* Info can be incomplete */
StudentList findStudentsByInfo(Student partialInfo, StudentList* studentList) {
    size_t sizeStudents = studentList->size;

    StudentList students;
    Student studentInfo = MakeQuery(partialInfo);

    size_t s = 0;
    for(size_t i = 0; i < sizeStudents; ++i) {
        Student* student = &(studentList->array[i]);
        if (matchesStudent(&studentInfo, student)) {
            students.array[s++] = *student;
        }
    }
    
    students.size = s;
    return students;
}

/* Commands */
int isCommandArg(char* str, CommandArg cmdArg) {
    int str_len = strlen(str);
    if (str_len == 1) {
        return str[0] == cmdArg.shorthand;
    } else if (str_len == 2) {
        return str[0] == cmdArg.shorthand && str[1] == ':';
    } else {
        return strcmp(substring(str, 0, strlen(str) - 1, 1), cmdArg.full) == 0 || strcmp(substring(str, 0, strlen(str) - 2, 1), cmdArg.full) == 0;
    }
}

static void student_cmd(char* lastName, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.studentName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery, studentList);
    
    /* For each student/entry found, print:
     * last name, first name
     * grade, classroom assignment
     * the name of their teacher (last and first name). 
     */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Grade: %d; Classroom: %d; Teacher: %s,%s\n", 
                result.studentName.lastName, result.studentName.firstName,
                result.grade, result.classroom,
                result.teacherName.lastName, result.teacherName.firstName);
    }
    
    //free(results->array);
    free(results);
}

static void student_bus_cmd(char* lastName, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.studentName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery, studentList);
    
    /* For each entry found, print the last name, first name and the bus route the student takes. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Bus Route: %d\n", 
                result.studentName.lastName, result.studentName.firstName,
                result.bus);
    }
    
    free(results);
}

static void teacher_cmd(char* lastName, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.teacherName = {.lastName = lastName}};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    /* For each entry found, print the last and the first name of the student. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s\n", result.studentName.lastName, result.studentName.firstName);
    }
    
    free(results);
}

static void bus_cmd(int bus, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.bus = bus};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    /* For each such entry, output the name of the student (last and first) and their grade and classroom. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s; Grade: %d; Classroom: %d\n", 
                result.studentName.lastName, result.studentName.firstName,
                result.grade, result.classroom);
    }
    
    free(results);
}

static void grade_cmd(int grade, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    /* For each entry, output the name (last and first) of the student. */
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        printf("%s,%s\n", result.studentName.lastName, result.studentName.firstName);
    }
    
    free(results);
}

static void grade_high_cmd(int grade, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    Student entry = results->array[0];
    for (size_t i = 1; i < results->size; ++i) {
        Student result = results->array[i];
        if (result.gpa > entry.gpa) {
            entry = result;
        }
    }
    
    /* Report the contents of the highest GPA entry (name of the student, GPA, teacher, bus route). */
    printf("%s,%s; GPA: %f; Teacher: %s,%s; Bus Route: %d\n", 
            entry.studentName.lastName, entry.studentName.firstName,
            entry.gpa,
            entry.teacherName.lastName, entry.teacherName.firstName,
            entry.bus);
    
    free(results);
}

static void grade_low_cmd(int grade, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    Student entry = results->array[0];
    for (size_t i = 1; i < results->size; ++i) {
        Student result = results->array[i];
        if (result.gpa < entry.gpa) {
            entry = result;
        }
    }
    
    /* Report the contents of the lowest GPA entry (name of the student, GPA, teacher, bus route). */
    printf("%s,%s; GPA: %f; Teacher: %s,%s; Bus Route: %d\n", 
            entry.studentName.lastName, entry.studentName.firstName,
            entry.gpa,
            entry.teacherName.lastName, entry.teacherName.firstName,
            entry.bus);
    
    free(results);
}

static void average_cmd(int grade, StudentList* studentList) {
    StudentList* results = malloc(sizeof(StudentList));
    
    /* Search for students */
    Student searchQuery = {.grade = grade};
    *results = findStudentsByInfo(searchQuery, studentList); 
    
    double average = 0;
    for (size_t i = 0; i < results->size; ++i) {
        Student result = results->array[i];
        average += result.gpa;
    }
    
    average /= results->size;
    
    /* Compute the average GPA score for the entries found. Output the grade level (the number provided in command) and the average GPA score computed. */
    printf("Grade Level: %d => Average: %f\n", grade, average);
    
    free(results);
}

static void info_cmd(StudentList* studentList) {
    for (size_t i = 0; i <= 6; ++i) {
        StudentList* results = malloc(sizeof(StudentList));
        
        /* Search for students */
        Student searchQuery = {.grade = i};
        *results = findStudentsByInfo(searchQuery, studentList);
        
        /* Report number of students per grade */
        printf("%d: %d\n", (int)i, (int)results->size);
        
        free(results);
    }
}

static void quit_cmd(int* finished) {
    *finished = 1;
}

void interpretCommand(char* cmd, StudentList* studentList, int* finishedPtr) {
    StringArray line = splitString(cmd, ' '); /* split by space */
    
    /* Student */
    if (isCommandArg(line.array[0], CMD_STUDENT)) {
        char* lastName = line.array[1];
        if (line.size == 2) {
            student_cmd(lastName, studentList);
        } else { /* line.size == 3 */
            if (isCommandArg(line.array[2], CMD_BUS)) {
                student_bus_cmd(lastName, studentList);
            }
        }
    }
    
    /* Teacher */
    if (isCommandArg(line.array[0], CMD_TEACHER)) {
        teacher_cmd(line.array[1], studentList);
    }
    
    /* Bus */
    if (isCommandArg(line.array[0], CMD_BUS)) {
        bus_cmd(atoi(line.array[1]), studentList);
    }
    
    /* Grade */
    if (isCommandArg(line.array[0], CMD_GRADE)) {
        int grade = atoi(line.array[1]);
        if (line.size == 3) {
            if (isCommandArg(line.array[2], ARG_HIGH)) {
                grade_high_cmd(grade, studentList);
            } else if (isCommandArg(line.array[2], ARG_LOW)) {
                grade_low_cmd(grade, studentList);
            }
        } else { /* line->size == 2 */
            grade_cmd(grade, studentList);
        }
    }
    
    /* Average */
    if (isCommandArg(line.array[0], CMD_AVERAGE)) {
        average_cmd(atoi(line.array[1]), studentList);
    }
    
    /* Info */
    if (isCommandArg(line.array[0], CMD_INFO)) {
        info_cmd(studentList);
    }
    
    /* Quit */
    if (isCommandArg(line.array[0], CMD_QUIT)) {
        quit_cmd(finishedPtr);
    }
    
    printf("\n"); // new line at the end
    freeStringArray(&line);
}

