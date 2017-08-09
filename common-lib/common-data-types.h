//
// Created by x3r0 on 8/1/17.
//

#ifndef VIRT_MACHINE_COMMON_DATA_TYPES_H
#define VIRT_MACHINE_COMMON_DATA_TYPES_H


//instruction encoding modes
#define R_TYPE  0
#define I_TYPE  1
#define J_TYPE  2

#define $zero   0   ///HARD WIRED ZERO
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

///R-TYPE INSTRUCTIONS
#define ADD     0   ///addition
#define SUB     1   ///subtraction
#define MUL     2   ///multiplication
#define DIV     3   ///division
#define AND     4   /// logical and
#define OR      5   ///logical or
#define NOR     6  ///logical NOR
#define XOR     7  ///logical execlusive
#define SLT     8  ///set on less than
#define SLTU    9  ///set on less than (unsigned commparision)
#define SGT     10  ///set on greater than
#define SGTU    11  ///set on greater than (unsigned comparison)
#define SLL     12  ///shif left logical
#define SRL     13  ///shift right logical

///I-TYPE
#define ADDI    14   ///addition with immediate value
#define ANDI    15  ///logical and with immediate data
#define ORI     16  ///logical or with immediate data
#define LW      17   ///load word from memory
#define SW      18   ///store word to memory
#define LUI     19   /// load upper segment of memory with immediate data
#define BEQ     20  ///branch if equal
#define BNE     21  ///branch if not equal
#define SLTI    22  ///set on less than with immediate operand
#define SLTIU   23  ///set on less than with immediate operand ( unsigned comparison)
#define SGTI    24  ///set on greater than with immediate operand
#define SGTIU   25  ///set on greater than with immmediate operand (usigned comparison)
#define JR      26  ///jump to location in register

///J-TYPE
#define J       27  ///jump
#define JAL     28  ///jump and link, procedure call, stores the current $pc + 4 to register $ra
#define HALT    29  /// halt
#define INT     30  ///handle interupt specified by the INTERUPT_ID
#define UNDEFINED     31    /// NOT IN USE!!!


///Interrupt driven IO
///interupts codes are listed below
#define INT_IO_WRITE   0           ///Register $a0 holds the memory address of the string to be written / or read to
#define INT_IO_READ    1           ///Register $a1 provides the type of data (data type : int /char ) to be read or written


#define DATA_TYPE_INT   0           ///read/write data as interger
#define DATA_TYPE_CHAR  1           ///read or write data as a char [sequence]


//define the data types
typedef unsigned char Byte;         //8bits
typedef unsigned short HalfWord;    //16 bits
typedef unsigned int Word;          //32bits
typedef unsigned long DWord;        //64 bits

///object file header structure
struct Obj_Header{
    int text_size;
    int text_start;
    int entry_point;
    int data_start;
}__attribute__((__packed__));

typedef struct Obj_Header O_Header;


#endif //VIRT_MACHINE_COMMON_DATA_TYPES_H
