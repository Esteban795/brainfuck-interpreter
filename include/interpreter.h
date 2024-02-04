#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <stdlib.h>


#define MAX_ARRAY_SIZE 30000

typedef unsigned char byte;


struct state {
    byte* arr;
    int ptr;
    char* cmds;
    size_t cmds_count;
};

typedef struct state bf_state;

bf_state* createInterpreter(const char* source_path);

void destroyInterpreter(bf_state** state);

void runInterpreter(bf_state* state);
#endif