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
    vm->$pc = 0x0040;

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
    _inst_buffer = vm->RAM[vm->$pc];

    ///increment the program counter to the next instruction
    vm->$pc += 4;
}

void decode()
{

    opcode = (_inst_buffer & 0xfc000000) >> 26; ///read the 6 MSBits into an integer value
    if((opcode >= 0 && opcode <= 4) || (opcode >= 8 && opcode <= 15))   ///-- parse R-TYPE
    {
        parse_args(R_TYPE);
    }else if((opcode >= 5 && opcode <= 7)  || (opcode >= 16 && opcode <= 25))    ///-- parse I-TYPE
    {
        parse_args(I_TYPE);
    }else if(opcode >= 26 && opcode <= 32)                          ///-- parse J-TYPE
    {
        parse_args(J_TYPE);
    }else{
        printf("[-]ERROR::Unknown opcode %d found in instruction -> [%x] %x", opcode, vm->$pc, _inst_buffer);

        ///kill the virtual machine
        vm->_status_running = 0;
    }
}

void execute()
{
    printf("[Debug]::Executing instruction\n");
}

void run()
{
    printf("[Debug]::Starting machine\n");
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