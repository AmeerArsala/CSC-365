/* 
 * File:   schoolsearch.h
 * Author: Ameer Arsala
 *
 * Created on October 2, 2023, 9:58 AM
 */

#ifndef SCHOOLSEARCH_H
#define SCHOOLSEARCH_H

typedef struct {
    struct Name studentName;
    int grade;
    int classroom;
    int bus; /* you make me busss */
    float gpa;
    struct Name teacherName;
} Student;

struct Name {
    char* lastName;
    char* firstName;
};

char* toString(Student* studentInfo);



#endif /* SCHOOLSEARCH_H */

