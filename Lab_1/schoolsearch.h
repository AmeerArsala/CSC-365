/* 
 * File:   schoolsearch.h
 * Author: Ameer Arsala
 *
 * Created on October 2, 2023, 9:58 AM
 */

#ifndef SCHOOLSEARCH_H
#define SCHOOLSEARCH_H

extern int finished;

struct Name {
    char* lastName;
    char* firstName;
};

typedef struct {
    struct Name studentName;
    int grade;
    int classroom;
    int bus; /* you make me busss, yugonmakmibussss */
    double gpa;
    struct Name teacherName;
} Student;

typedef struct {
    Student* array;
    size_t size;
} StudentList;


typedef struct {
    char shorthand;
    char* full;
} CommandArg;

extern const struct Name NO_NAME;
extern const Student NULL_STUDENT;

extern const CommandArg CMD_STUDENT;
extern const CommandArg CMD_TEACHER;
extern const CommandArg CMD_BUS;
extern const CommandArg CMD_GRADE;
extern const CommandArg ARG_HIGH;
extern const CommandArg ARG_LOW;
extern const CommandArg CMD_AVERAGE;
extern const CommandArg CMD_INFO;
extern const CommandArg CMD_QUIT;

extern StudentList* studentList;

/* Program Initialization */
void initsearch(char* file);
void cleanup();

/* Struct Initialization */
Student MakeQuery(Student studentInfo);
struct Name MakeName(struct Name name);

/* (Student* studentInfo) => studentInfo pointer REFERENCES the value *studentInfo */

char* toString(Student* studentInfo);

Student fromLine(char* line);
StudentList fromString(char* lines);

int namesEqual(struct Name n1, struct Name n2);
int studentsEqual(Student s1, Student s2);
int matchesStudent(Student incompleteInfo, Student potentialMatch);

StudentList findStudentsByInfo(Student partialInfo);

/* Commands */
int isCommandArg(char* str, CommandArg cmdArg);
void interpretCommand(char* cmd);

#endif /* SCHOOLSEARCH_H */

