//
// Created by x3r0 on 8/1/17.
//

#ifndef VIRT_MACHINE_VM_H
#define VIRT_MACHINE_VM_H

#define DEBUG 0

#define RAM_SIZE 64*1024*1024   //64Mb

///memory lay out assumed
/*
+-------------------+
|    Reserved       |
+-------------------+   <- 0x0040 $pc
|       Text        |
+-------------------+
|   Static Data     |   <- 0x10000 $gp
+-------------------+
|                   |
|   Dynamic data    |
|                   |
|                   |
|                   |
|                   |
|                   |
|   Stack area      |
+-------------------+   <- $ffff    $fp, $sp
 */
#include "../common-lib/common-utils.h"
#include "../common-lib/common-data-types.h"

struct Vitrual_Machine{
    Word $pc;
    Word *Registers;
    Word *RAM;
    int _status_running;
};
typedef struct Vitrual_Machine VM;

VM *vm;



///buffers to hold data and instruction
Word _inst_buffer;

/// global variable used as buffer for operand storage
/// used in instruction decoding
signed int operands[4];     ///signed because it also holds signed immediate values

///buffer to hold opecode
int opcode;

///init and deinit funcs for vm
void init_vm();
void delete_vm();

///program loader
///should load program and data,
/// and it should patch memory references
/// that require immediate memory address
void load_program(char *);

///fetch instruction
void fetch();

///decode instruction
void decode();

/// execute the current instruction
void execute();

//run the vm
void run(char *);

///helper function to parse arguments given
/// the instrucrion type {R | I | J}
void parse_args(int );



///helper functions to aid the main execute function
/// implement primitive operations
///perform the neccesary operations based on pre-parsed information
void arithmetic_operations();
void logical_operations();
void memory_access();
void conditional_operations();
void unconditional_branch_operations();
void halt();
void interrupt();

///helper function to help the helper function interrupt :)
///parses out four character out of a given word and vice versa
void parse_word_to_char(Word , char *);
void parse_word_from_chars(Word *, char *);



///helper functions to help convert signed immediate values to diffrent data sizes [word -> half word && half word -> word]
///sign extention and stuff
signed int _convert_halfword_to_word(int );




///helper function to help the loader and linker load the progrma file
int binary_size(FILE *);

#endif //VIRT_MACHINE_VM_H
