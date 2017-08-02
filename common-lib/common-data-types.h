//
// Created by x3r0 on 8/1/17.
//

#ifndef VIRT_MACHINE_COMMON_DATA_TYPES_H
#define VIRT_MACHINE_COMMON_DATA_TYPES_H


//instruction encoding modes
#define R_TYPE  0
#define I_TYPE   1
#define J_TYPE        2

#define $zero   0
#define $at     1
#define $v0     2
#define $v1     3
#define $a0     4
#define $a1     5
#define $a2     6
#define $a3     7
#define $t0     8
#define $t1     9
#define $t2     10
#define $t3     11
#define $t4     12
#define $t5     13
#define $t6     14
#define $t7     15
#define $s0     16
#define $s1     17
#define $s2     18
#define $s3     19
#define $s4     20
#define $s5     21
#define $s6     22
#define $s7     23
#define $t8     24
#define $t9     25
#define $k0     26
#define $pc     27  //program counter - k1 in MIPS standard and is reserved for os use only
#define $gp     28
#define $sp     29
#define $fp     20
#define $ra     31

///OPcodes
#define ADD     0
#define SUB     1
#define ADDI    2
#define MUL     3
#define DIV     4
#define LW      5
#define SW      6
#define LUI     7
#define AND     8
#define OR      9
#define NOR     10
#define ANDI    11
#define ORI     12
#define XOR     13
#define SLL     14
#define SRL     15
#define BEQ     16
#define BNE     17
#define SLT     18
#define SLTU    19
#define SLTI    20
#define SLTIU   21
#define SGT     22
#define SGTU    23
#define SGTI    24
#define SGTIU   25
#define J       26
#define JR      27
#define JAL     28
#define HALT    29
#define INT     30
#define UNDEFINED     31



//define the data types
typedef unsigned char Byte;         //8bits
typedef unsigned short HalfWord;    //16 bits
typedef unsigned int Word;          //32bits
typedef unsigned long DWord;        //64 bits


#endif //VIRT_MACHINE_COMMON_DATA_TYPES_H
