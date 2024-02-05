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

//Assumes f is a valid file pointer
static int removeNonCodeChar(bf_state* state,FILE* f){
    size_t cmds_count = countCmds(f);
    state->cmds_count = cmds_count;
    state->cmds = calloc(cmds_count + 1,1);
    if (state->cmds == NULL){
        fprintf(stderr, "Error: Could not allocate memory for cmds\n");
        fclose(f);
        destroyInterpreter(&state);
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
    state->arr = calloc(MAX_ARRAY_SIZE,sizeof(byte));
    if (state->arr == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for 'state->arr'\n");
        free(state);
        fclose(f);
        return NULL;
    }
    state->ptr = 0;
    return removeNonCodeChar(state, f) == 0 ? state : NULL;
}


void destroyInterpreter(bf_state** state){
    free((*state)->arr);
    free((*state)->cmds);
    free(*state);
    *state = NULL; //makes sure cannot be double freed
}


static void goToOpeningBracket(bf_state *bfp, int *cptr) {
    int nest = 0;
    while (bfp->cmds[*cptr]) {
        if (bfp->cmds[*cptr] == ']') {
            nest += 1;
        } else if (bfp->cmds[*cptr] == '[' && nest > 0) {
            nest -= 1;
        } else if (bfp->cmds[*cptr] == '[' && nest == 0)
            break;

        *cptr -= 1;
    }
}

static void goToClosingBracket(bf_state *bfp, int *cptr) {
    int nest = 0;
    while (bfp->cmds[*cptr]) {
        if (bfp->cmds[*cptr] == '[') {
            nest += 1;
        } else if (bfp->cmds[*cptr] == ']' && nest > 0) {
            nest -= 1;
        } else if (bfp->cmds[*cptr] == ']' && nest == 0)
            break;

        *cptr += 1;
    }
}
int runInterpreter(bf_state* state){
    int cmd_ptr = 0;
    while (state->cmds[cmd_ptr]) {
        switch (state->cmds[cmd_ptr]) {
            case '>':
                state->ptr++;
                cmd_ptr++;
                break;
            case '<':
                state->ptr--;
                cmd_ptr++;
                break;
            case '+':
                state->arr[state->ptr] += 1;
                cmd_ptr++;
                break;
            case '-':
                state->arr[state->ptr] -= 1;
                cmd_ptr++;
                break;
            case '.':
                putc(state->arr[state->ptr],stdout);
                cmd_ptr++;
                break;
            case ',':
                scanf("%c",&state->arr[state->ptr]);
                cmd_ptr++;
                break;
            case '[':
                if (state->arr[state->ptr] == 0){ //loop is done, go to the end of the loop
                    cmd_ptr++; //we skip the first '[' because it will affect how next function works
                    goToClosingBracket(state,&cmd_ptr);
                    cmd_ptr++; // skip ']' because the loop is done
                } else { //loop is not done, read loop instructions
                    cmd_ptr++; 
                }
                break;
            case ']':
                if (state->arr[state->ptr] != 0){ //loop is not done, go the start
                    cmd_ptr--;
                    goToOpeningBracket(state,&cmd_ptr);
                    cmd_ptr++;
                } else { //loop's done, continue the program
                    cmd_ptr++;
                }
                break;
        }
        if (state->ptr < 0) {
            fprintf(stderr,"Out of bounds access.\n");
            exit(1);
            return 1;
        }
    }
    return 0;
}
int main(int argc, char* argv[]){
    if (argc != 2){
        fprintf(stderr, "Usage: %s <source_file>\n",argv[0]);
        exit(1);
    }
    bf_state* state = createInterpreter(argv[1]);
    printf("Cmds-count : %zu\n",state->cmds_count);
    printf("Cmds : %s\n\n",state->cmds);
    int result = runInterpreter(state);
    if (result != 0) {
        printf("Something went wrong during execution.\n");
        exit(1);
    }
    destroyInterpreter(&state);
    return 0;
}