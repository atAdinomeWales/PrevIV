#ifndef SRCLIB_H
#define SRCLIB_H

#include <stdint.h>

#ifndef DEFAULT_DEBUG_STATUS 
#define DEFAULT_DEBUG_STATUS false
#endif

#define BUFFER_SIZE 100

#define DEBUG(_cond, ...) if(_cond) { printf(__VA_ARGS__); }
#define ASSERT(_e, ...) if(!(_e)) {fprintf(stderr, __VA_ARGS__); exit(1);}

enum ERRORS {
    NO_PATH_PROVIDED,
    FILE_NOT_FOUND,
    FILE_ACCSES_FAILED
};

void strcpyChop(char* destination, char* source, int chop);

//Scan argv buffer for string signaled by specific characters at the ends of the statement
char* argvScan(int argc, char** argv);

//Find an expression in argv and return its position or a negative interger if it can not be found
int argvFind(int argc, char** argv, char* str);

//If found returns 1 otherwise 0
int fileCheck(char* path, int8_t debug);

char* getFileName(char* path, int8_t debug);

#endif//SRCLIB_H