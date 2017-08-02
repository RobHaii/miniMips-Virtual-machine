//
// Created by x3r0 on 8/1/17.
//

#ifndef VIRT_MACHINE_VM_H
#define VIRT_MACHINE_VM_H

#define RAM_SIZE 64*1024*1024   //2 GB => 1073741824

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
int operands[4];

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
void run();

///helper function to parse arguments given
/// the instrucrion type {R | I | J}
void parse_args(int );

#endif //VIRT_MACHINE_VM_H
