/* 
 * File:   schoolsearch.h
 * Author: Ameer Arsala
 *
 * Created on October 2, 2023, 9:58 AM
 */

#ifndef SCHOOLSEARCH_H
#define SCHOOLSEARCH_H

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

typedef struct {
    struct Name studentName;
    int grade;
    int classroom;
    int bus; /* you make me busss */
    double gpa;
    struct Name teacherName;
} Student;

struct Name {
    char* lastName;
    char* firstName;
};

typedef struct {
    char shorthand;
    char* full;
} CommandArg;

Student MakeQuery(Student studentInfo);
struct Name MakeName(struct Name name);

/* (Student* studentInfo) => studentInfo pointer REFERENCES the value *studentInfo */

char* toString(Student* studentInfo);

Student fromLine(char* line);
/* TODO: fromLines */

Student* findStudentsByInfo(Student* studentInfo);



#endif /* SCHOOLSEARCH_H */

