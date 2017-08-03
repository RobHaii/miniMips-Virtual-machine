//
// Created by x3r0 on 8/1/17.
//

#ifndef VIRT_MACHINE_COMMON_DATA_TYPES_H
#define VIRT_MACHINE_COMMON_DATA_TYPES_H


//instruction encoding modes
#define R_TYPE  0
#define I_TYPE  1
#define J_TYPE  2

#define $zero   0   ///AHRD WIRED ZERO
#define $at     1   ///assembler temporary
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
#define $k1     27
#define $gp     28
#define $sp     29
#define $fp     20
#define $ra     31




///OPcodes and mnemonics

///all operations use data in register unless specified with the word "immediate" in the commnet
/// all operation assume signed data unless specified by the term "unsigned" in the associated comment
#define ADD     0   ///addition
#define SUB     1   ///subtraction
#define ADDI    2   ///addition with immediate value
#define MUL     3   ///multiplication
#define DIV     4   ///division
#define LW      5   ///load word from memory
#define SW      6   ///store word to memory
#define LUI     7   /// load upper segment of memory with immediate data
#define AND     8   /// logical and
#define OR      9   ///logical or
#define NOR     10  ///logical NOR
#define ANDI    11  ///logical and with immediate data
#define ORI     12  ///logical or with immediate data
#define XOR     13  ///logical execlusive
#define SLL     14  ///shif left logical
#define SRL     15  ///shift right logical
#define BEQ     16  ///branch if equal
#define BNE     17  ///branch if not equal
#define SLT     18  ///set on less than
#define SLTU    19  ///set on less than (unsigned commparision)
#define SLTI    20  ///set on less than with immediate operand
#define SLTIU   21  ///set on less than with immediate operand ( unsigned comparison)
#define SGT     22  ///set on greater than
#define SGTU    23  ///set on greater than (unsigned comparison)
#define SGTI    24  ///set on greater than with immediate operand
#define SGTIU   25  ///set on greater than with immmediate operand (usigned comparison)
#define J       26  ///jump
#define JR      27  ///jump to location in register
#define JAL     28  ///jump and link, procedure call, stores the current $pc + 4 to register $ra
#define HALT    29  /// halt
#define INT     30  ///handle interupt specified by the INTERUPT_ID
#define UNDEFINED     31    /// NOT IN USE!!!


//define the data types
typedef unsigned char Byte;         //8bits
typedef unsigned short HalfWord;    //16 bits
typedef unsigned int Word;          //32bits
typedef unsigned long DWord;        //64 bits


#endif //VIRT_MACHINE_COMMON_DATA_TYPES_H
