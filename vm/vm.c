//
// Created by x3r0 on 8/1/17.
//

#include "vm.h"

void init_vm(){

    ///init vm itself
    vm = (VM *) checked_malloc(sizeof(VM));

    ///init the virtual machine components
    ///init the register file
    vm->Registers = (Word *) checked_malloc(32* sizeof(Word));

    ///init the RAM space
    vm->RAM = (Word *) checked_malloc(RAM_SIZE);

    ///set the running flag
    vm->_status_running = 1;

    ///ini the PC at 0x0040
    vm->Registers[$pc] = 0x0040;

    ///init the SP and the FP at the bottom of the ram
    vm->Registers[$sp] = RAM_SIZE/4;
    vm->Registers[$fp] = RAM_SIZE/4;

    ///init the global pointer $gp
    vm->Registers[$gp] = 0x1000;
}
void delete_vm(){

    /// free the register sapce
    free(vm->Registers);

    ///free the ram space
    free(vm->RAM);

    ///free the vm space
    free(vm);

}

void fetch(){
    ///load the current instruction
    _inst_buffer = vm->RAM[vm->Registers[$pc]];

    ///increment the program counter to the next instruction
    vm->Registers[$pc] += 4;
}

void decode()
{
    int opcode;
    opcode = (_inst_buffer & 0xfc000000) >> 26; ///read the 6 MSBits into an integer value
    switch(opcode){
        case 0:     ///arithmetic               -- parse R-TYPE
            parse_args(R_TYPE);
            break;
        case 1:     ///memory access            -- parse I-TYPE
            parse_args(I_TYPE);
            break;
        case 2:     ///logical operations       -- parse R-TYPE
            parse_args(R_TYPE);
            break;
        case 3:     ///conditional branch       -- parse I-TYPE
            parse_args(I_TYPE);
            break;
        case 4:     ///unconditional branch     -- parse J-TYPE
            parse_args(J_TYPE);
            break;
        case 5:     ///Halt                     -- parse J-TYPE
            parse_args(J_TYPE);
            break;
        default:    ///i dont know what to call this,
            printf("[-]ERROR::Unknown opcode %d found in instruction -> [%x] %x", opcode, vm->Registers[$pc], _inst_buffer);

            ///kill the virtual machine
            vm->_status_running = 0;
            break;
    }
}

void execute()
{

}

void run()
{
    while(vm->_status_running)
    {
        fetch();
        decode();
        if(vm->_status_running)
            execute();
        else break;
    }
}


void parse_args(int instruction_type)
{

    ///decode the instruction operands based on
    /// the recieved instruction type id
    switch (instruction_type){
        case 0:     ///R-TYPE
            break;
        case 1:     ///I-TYPE
            break;
        case 2:     ///J-TYPE
            break;
        default:    ///some thing must have gone wrong
            printf("[-]ERROR::unknown instruction type %d\n", instruction_type);
            vm->_status_running = 0;
            break;
    }
}