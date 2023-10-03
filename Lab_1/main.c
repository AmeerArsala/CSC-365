/* 
 * File:   main.c
 * Author: Ameer Arsala
 *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 
 */
int main(int argc, char** argv) {
    const char* file_path = "students.txt";
    
    /* Open file */
    FILE* file = fopen(file_path, "r");
    
    /* Check if file was opened successfully */
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    /* Detetrmine file size */
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    /* Allocate memory */
    char* file_content = (char*)malloc(file_size + 1); /* + 1 for null terminator */
    
    /* Check if memory allocation was successful */
    if (file_content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return 1;
    }
    
    /* Read file into memory now */
    size_t bytes_read = fread(file_content, 1, file_size, file);
    
    /* Check if read operation was successful */
    if (bytes_read != file_size) {
        perror("Error reading file");
        free(file_content);
        fclose(file);
        return 1;
    }
    
    /* Null-terminate the string (why?) */
    file_content[file_size] = '\0';
    
    /* Main operation with the file */
    search(file_content);
    
    /* Cleanup: close the file and free allocated memory */
    free(file_content);
    fclose(file);
    
    return 0;
}

void search(char* datafile) {
    
}

