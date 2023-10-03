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
    int bus; /* you make me busss, yugonmakmibussss */
    double gpa;
    struct Name teacherName;
} Student;

typedef struct {
    Student* array;
    size_t size;
} StudentList;

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
StudentList fromString(char* lines);

int namesEqual(Name n1, Name n2);
int studentsEqual(Student s1, Student s2);
int matchesStudent(Student incompleteInfo, Student potentialMatch);

StudentList findStudentsByInfo(Student* studentInfoPtr, StudentList studentList);

/* Commands */
int isCommandArg(char* str, CommandArg cmdArg);
void interpretCommand(char* cmd);
void student_cmd(char* lastName, int* bus);
void teacher_cmd(char* lastName);
void bus_cmd(int bus);
void grade_cmd(int grade);
void grade_high_cmd(int grade);
void grade_low_cmd(int grade);
void average_cmd(int grade);
void info_cmd();
void quit_cmd();

#endif /* SCHOOLSEARCH_H */

