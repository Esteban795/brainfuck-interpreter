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

static int removeNonCodeChar(bf_state* state,FILE* f){
    size_t cmds_count = countCmds(f);
    state->cmds_count = cmds_count;
    state->cmds = calloc(cmds_count + 1,1);
    if (state->cmds == NULL){
        fprintf(stderr, "Error: Could not allocate memory for cmds\n");
        fclose(f);
        return 1;
    }
    char c;
    size_t i = 0;
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
                state->cmds[i] = c;
                i += 1;
                break;
        }
    }
    printf("cmds: %s\n", state->cmds);
    rewind(f);
    return 0;
}



bf_state* createInterpreter(const char* source_path){
    FILE* f = fopen(source_path, "rb");
    if (f == NULL){
        fprintf(stderr, "Error: Could not open file %s\n", source_path);
        return NULL;
    }
    bf_state* state = malloc(sizeof(bf_state));
    if (state == NULL){
        fprintf(stderr, "Error: Could not allocate memory for 'state'.\n");
        fclose(f);
        return NULL;
    }
    state->arr = malloc(MAX_ARRAY_SIZE * sizeof(byte));
    if (state->arr == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for 'state->arr'\n");
        free(state);
        fclose(f);
        return NULL;
    }
    state->ptr = 0;
    removeNonCodeChar(state, f);
    return state;
}


void destroyInterpreter(bf_state** state){
    free((*state)->arr);
    free((*state)->cmds);
    free(*state);
    *state = NULL; //makes sure cannot be double freed
}

int main(void){
    bf_state* state = createInterpreter("./examples/helloworld.bf");
    destroyInterpreter(&state);
    if (state == NULL){
        return 1;
    }
    return 0;
}