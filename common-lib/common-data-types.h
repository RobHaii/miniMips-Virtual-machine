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


//define the data types
typedef unsigned char Byte;         //8bits
typedef unsigned short HalfWord;    //16 bits
typedef unsigned int Word;          //32bits
typedef unsigned long DWord;        //64 bits


#endif //VIRT_MACHINE_COMMON_DATA_TYPES_H
