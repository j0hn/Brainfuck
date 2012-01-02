/**
 * Lightweight, open-source brainfuck interpreter written in C.
 *
 * @author Fabian M.
 */

/* Includes. */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "stack.h"

/* Constants. */
#define VERSION "1.0.1"
#define MAX_CELLS 65536 /* 2^16 */
#define CACHE_SIZE MAX_CELLS

/* Program */
typedef struct
{
    /**
     * The file to interpret.
     */
    char *file;

    /**
     * The amount of memory cells to reserve.
     */
    uint32_t cells;
} Program;

/**
 * Output the given char.
 */
void interpreter_output_char(char c) {
    printf("%c", c);
    fflush(stdout);
}

/**
 * Generates error message.
 */
void interpreter_error(char *message) {
    printf("Error at line %i, file \"%s\": %s\n", __LINE__, __FILE__, message);
    exit(EXIT_FAILURE);
}

/*
 * Prints out the version of the program.
 */
void interpreter_version(void) {
    printf("Brainfuck interpreter, version %s.\n\n", VERSION);
    printf("For more info, try \"./brainfuck -h\".\n");

    exit(EXIT_SUCCESS);
}

/*
 * Prints out the help of the program.
 */
void interpreter_help(void) {
    printf("Brainfuck interpreter, version %s.\n\n", VERSION);

    printf("Usage: ./brainfuck <input_file>.\n");
    printf("\t-v    Prints out the version of the program.\n");
    printf("\t-h    Prints out the help (what you are seeing).\n");

    exit(EXIT_SUCCESS);
}

/**
 * Parses the command line arguments.
 *
 * @param Program *program, the program information.
 * @param int argc, the number of arguments.
 * @param char *argv[], the arguments passed string.
 */
void parse_args(Program *program, int argc, char *argv[]) {
    int i = 0;

    /* Clear program->file value. */
    program->file = "";

    for(i=1; i<argc; i++) {
        if(strcmp("-v", argv[i]) == 0) {
            interpreter_version();
        } else if(strcmp("-h", argv[i]) == 0) {
            interpreter_help();
        } else {
            program->file = argv[i];
        }
    }
}

/**
 * Interprets the file given at startup.
 */
void interpret(Program *program) {
    if(!program->file || strcmp(program->file, "") == 0) {
        printf("Missing required arguments, interpreter terminating.\n");
        interpreter_help();
        exit(EXIT_FAILURE);
    }

    /* Open file wrapper. */
    FILE *input = fopen(program->file, "r");

    if(!input) {
        interpreter_error("Could not access input file.");
    }

    /* Set the amount of cells. */
    program->cells = MAX_CELLS;

    /* Contains all memory. */
    int data[program->cells];

    /* Points to the current Loop through all characters.index. */
    int dataPointer = 0;

    /* Get size of file. */
    fseek(input, 0, SEEK_END); /* seek to end of file */
    int file_size = ftell(input); /* get current file pointer */
    fseek(input, 0, SEEK_SET); /* seek back to beginning of file */

    /* Current char. */
    char current_char;
    /* Array that contains all characters. */
    char chars[file_size];
    /* Index at the character array. */
    int char_pointer = 0;

    /* Loop cache */
    Stack loop_stack = stack_empty(CACHE_SIZE);
    int loop_cache[CACHE_SIZE];

    /* Get file contents. */
    char_pointer = 0;
    while((current_char = (char) fgetc(input)) != EOF) {
        chars[char_pointer] = current_char;

        /* Fill the loop cache */
        if(current_char == '[') {
            stack_push(loop_stack, char_pointer);
        } else if(current_char == ']') {
            int top = stack_pop(loop_stack);

            if(top > CACHE_SIZE) {
                printf("Not enought cache size\n");
                exit(EXIT_FAILURE);
            }

            loop_cache[top] = char_pointer;
            loop_cache[char_pointer] = top;
        }

        char_pointer++;
    }

    if(!stack_is_empty(loop_stack)){
        interpreter_error("Unclosed [.");
    }

    /* Loop through all characters. */
    char_pointer = 0;
    while(char_pointer < file_size) {

        current_char = chars[char_pointer];
        switch(current_char) {
        case '>':
            if ((dataPointer + 1) > sizeof(data)) {
                interpreter_error("Data pointer is too big.");
            }
            dataPointer++;
            break;
        case '<':
            if ((dataPointer - 1) < 0) {
                interpreter_error("Data pointer is negative.");
            }
            dataPointer--;
            break;
        case '+':
            data[dataPointer]++;
            break;
        case '-':
            data[dataPointer]--;
            break;
        case '.':
            interpreter_output_char(data[dataPointer]);
            break;
        case ',':
            data[dataPointer] = (int) fgetc(stdin);
            break;
        case '[':
            if(data[dataPointer] == 0){
                char_pointer = loop_cache[char_pointer];
            }
            break;
        case ']':
            if(data[dataPointer] != 0){
                char_pointer = loop_cache[char_pointer];
            }
            break;
        /* Allow hashtags (#). */
        case '#':
            while (chars[char_pointer] != '\n') {
                char_pointer++;
            }

            break;
        }

        /* Avance to the next character in the code. */
        char_pointer++;
    }
}

/**
 * Main execution method of the BrainFuck interpreter.
 */
int main(int argc, char *argv[]) {
    Program program;

    /* Parse arguments. */
    parse_args(&program, argc, argv);

    /* Interpret the program. */
    interpret(&program);

    return 0;
}
