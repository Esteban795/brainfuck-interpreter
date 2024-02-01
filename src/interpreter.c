#include "../include/interpreter.h"

//Assumes f is a valid file pointer
static size_t countCmds(FILE* f){
    char c;
    size_t count = 0;
    while ((c = fgetc(f)) != EOF){
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '.':
            case ',':
            case '[':
            case ']':
                count++;
                break;
            default:
                break;
        }
    }
    rewind(f);
    return count;
}