#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "libStdAux.h"


void strcpyChop(char* destination, char* source, int chop) {
    int len = strlen(source);

    if (chop >= len) {
        destination[0] = '\0';
        return;
    }

    int endIndex = len - chop -1;
    strncpy(destination, source + chop, endIndex);
    destination[endIndex] = '\0';

    return;
}

char* argvScan(int argc, char** argv){
    if(argc < 2){
        return NULL;
    }

    for(int i = 0; i < argc; i++){
        int len = strlen(argv[i]);
        if ( (argv[i][0] == '-')  &&  (argv[i][len-1] == '-') ){
            char* strRet = malloc( (len-2) * sizeof(char) );
            strcpyChop(strRet, argv[i], 1);

            return strRet;
        }
    }

    return NULL;
}

int argvFind(int argc, char** argv, char* str){
    if(!(argc && argv && str)){
        return -1;
    }

    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], str) == 0){
            return i;
        }
    }

    return -2;
}

int fileCheck(char* path, int8_t debug){
    if(!path){
        DEBUG(debug, "No path provided\n");
        return NO_PATH_PROVIDED;
    }

    if(access(path, F_OK) != -1){
        DEBUG(debug, "File found at: %s\n", path);
        return 1;
    }
    
    DEBUG(debug, "File not found, check path.\n");
    return FILE_NOT_FOUND;
}

char* getFileName(char* path, int8_t debug){
    if(!path){
        return "err";
    }

    char *fileName = strrchr(path, '/');

    if(fileName != NULL){
        fileName++;
    } else {
        fileName = path;
    }

    DEBUG(debug, "File name: %s\n", fileName);
    return fileName;
}