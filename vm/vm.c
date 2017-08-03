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
    if(opcode >= 0 && opcode <= 11)                     ///-- parse R-TYPE
    {
        parse_args(R_TYPE);
    }else if(opcode >= 12 && opcode <= 26)              ///-- parse I-TYPE
    {
        parse_args(I_TYPE);
    }else if(opcode >= 27 && opcode <= 30)              ///-- parse J-TYPE
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
        case 0:     ///R-TYPE   [opcode(6) | Rs(5) | Rt(5) | Rd(5) | shamt(5) | func(6)]

            ///the func section is not used in our impelmentaion
            ///we already have enough complexity with our ISA :)

            operands[0] = (_inst_buffer & 0x03e00000) >> 21;    ///Rs
            operands[1] = (_inst_buffer & 0x001f0000) >> 16;    ///Rt
            operands[2] = (_inst_buffer & 0x0000f800) >> 11;    ///Rd
            operands[3] = (_inst_buffer & 0x000007c0) >> 6;     ///shammt
            break;
        case 1:     ///I-TYPE
            operands[0] = (_inst_buffer & 0x03e00000) >> 21;    ///Rs
            operands[1] = (_inst_buffer & 0x001f0000) >> 16;    ///Rt
            operands[2] = _convert_halfword_to_word(_inst_buffer & 0x000000ff);          ///Immediate -- converts the
            break;
        case 2:     ///J-TYPE
            operands[0] = (_inst_buffer & 0x03ffffff);          ///Address
            break;
        default:    ///some thing must have gone wrong
            printf("[-]ERROR::unknown instruction type %d\n", instruction_type);
            vm->_status_running = 0;
            break;
    }
}
void load_program(char *filename){
    ///check if the file exists and that file size doesn't exceed memory limit
    ///we dont use virtual memory after all :)


    ///dummy instructions for testing only
    ///write 6 dummy instructions
    ///for testing
    int var = 0x0f000000;
    for(int i = 0; i < 6; i+=4)
    {
        vm->RAM[vm->$pc+i] = var;
        var = var >> 5;
    }
}

///helper functions to help the main execute function
///modularization make life a whole lot easier, so i have heard!!

///basic arithmetic operations : ADD, SUB, ADDI, DIV, MUL
///memory access operations [opcode: 5 OPERATIONS]
void arithmetic_operations()
{
    switch (opcode)
    {
        case ADD:     ///ADD
            vm->Registers[operands[2]] = (Word) (vm->Registers[operands[0]] + vm->Registers[operands[1]]); ///ignores overflow by casting result to 32 bit int
            break;

        case SUB:     ///SUB
            vm->Registers[operands[2]] = (Word) (vm->Registers[operands[0]] + vm->Registers[operands[1]]);
            break;

        case ANDI:     ///ADDI
            vm->Registers[operands[1]] = (Word) (vm->Registers[operands[0]] + operands[2]);
            break;

            ///the next two implemetations are a possible source of bug...
            ///you might want to take a good and let me know if i am missing something
            ///a fresh pair of eyes - take em when ever you can find em

        case MUL:     ///MUL      implicitly uses register %t8 for result [$t8][$rd] = $Rs * $Rt
            long result;
            result = vm->Registers[operands[0]] * vm->Registers[operands[1]];
            vm->Registers[operands[2]] = result & 0xffffffff;   ///lower word stored to the destination register
            vm->Registers[$t8] = (Word) ((result & 0xffffffff00000000) >> 32);
            break;

        case DIV:     ///DIV     implicitly uses the $t8 to retrieve higher word of dividend
            long dividend = (long) vm->Registers[$t8];  ///copy the content of register $t8 to a long variable (64 bit)
            dividend = dividend << 32;                  ///shift it left 32 times
            devidend |= vm->Registers[operands[0]];     ///append the content of the dividend rgister to the long var lower word
            vm->Registers[operands[2]] = (Word ) (dividend / vm->Registers[operands[1]]); /// devide the logn var by the divisor and put the result in destination register
            break;

        default:    ///something must have gone terribly wrong
            ///print error message and quit
            printf("[-]ERROR::opecode %d doesn't represent a valid arithmetic instruction", opcode);
            vm->_status_running = 0;
            break;
    }
}

///memory access operations [opcode: 3 OPERATIONS]
void memory_access(){
    switch (opcode)
    {

        case LW:     ///load word
            long effective_addr = 0;
            effective_addr = operands[0] + operands[2];             ///synthesize the effective address
            vm->Registers[operands[1]] = (Word ) vm->RAM[effective_addr];      ///load the data from memory to the register
            break;

        case SW:     ///store word
            long effective_addr = 0;
            effective_addr = operands[0] + operands[2];             ///synthesize the effective address
            vm->RAM[effective_addr] = vm->Registers[operands[1]];      ///load the data from memory to the register
            break;

        case LUI:     ///load upper two bytes with immediate value -> helps create effective mem address
            int immediate = (operands[2] & 0x0000ffff) << 16;
            vm->Registers[operands[1]] = 0x00;  ///clear previous data
            vm->Registers[operands[1]] |= immediate;
            break;

        default:    ///something must have gone sideways
            printf("[-]ERROR::opecode %d doesn't represent a valid memory access instruction", opcode);
            break;
    }
}

///primitive logical operations [8 OPERATIONS]
///simple and primitive logical operations
void logical_operations(){
    switch (opcode){
        case AND:       ///AND
            vm->Registers[operands[2]] = vm->Registers[operands[0]] & vm->Registers[operands[1]];
            break;
        case OR:        ///OR
            vm->Registers[operands[2]] = vm->Registers[operands[0]] | vm->Registers[operands[1]];
            break;
        case NOR:       ///NOR
            vm->Registers[operands[2]] = ~(vm->Registers[operands[0]] | vm->Registers[operands[1]]);
            break;
        case ANDI:     ///ANDI
            vm->Registers[operands[1]] = vm->Registers[operands[0]] & operands[2];
            break;
        case ORI:       ///ORI
            vm->Registers[operands[1]] = vm->Registers[operands[0]] | operands[2];
            break;
        case XOR:       ///XOR
            vm->Registers[operands[2]] = vm->Registers[operands[0]] ^ vm->Registers[operands[1]];
            break;
        case SLL:       ///SLL
            vm->Registers[operands[1]] = vm->Registers[operands[0]] <<  operands[2];
            break;
        case SRL:       ///SRL
            vm->Registers[operands[1]] = vm->Registers[operands[0]] >>  operands[2];
            break;
        default:        ///something must have gone sideways
            printf("[-]ERROR::opecode %d doesn't represent a valid logical instruction", opcode);
            vm->_status_running = 0;
            break;

    }
}


///conditional operations [10 operations]
void conditional_branch_operations()
{
    int addr = vm->$pc;
    switch (opcode)
    {
        case BEQ:       ///BEQ  branch if equal
            if(vm->Registers[operands[0]] == vm->Registers[operands[1]])
            {
                addr += operands[2];
                vm->$pc = addr;
            }
            break;

        case BNE:       ///BNE  branch if not equal
            if(vm->Registers[operands[0]] != vm->Registers[operands[1]])
            {
                addr += operands[2];
                vm->$pc = addr;
            }
            break;

        case SLT:       ///SLT set on less than [slt $t0, $t1, $t2 => $t0 = 1 if $t1 < $t2 else $t0 = 0]
            if( (signed ) vm->Registers[operands[0]] < (signed ) vm->Registers[operands[1]])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        case SLTU:       ///SLTU
            if((unsigned ) vm->Registers[operands[0]] < (unsigned ) vm->Registers[operands[1]])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        case SLTI:       ///SLTI
            if((signed ) vm->Registers[operands[0]] < (signed ) operands[2])
            {
                vm->Registers[operands[1]] = 1;
            }else{
                vm->Registers[operands[1]] = 0;
            }
            break;

        case SLTIU:       ///SLTIU
            if((unsigned ) vm->Registers[operands[0]] < (unsigned ) operands[2])
            {
                vm->Registers[operands[1]] = 1;
            }else{
                vm->Registers[operands[1]] = 0;
            }
            break;

        case SGT:       ///SGT
            if((signed ) vm->Registers[operands[0]] > (signed ) vm->Registers[operands[1]])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        case SGTU:       ///SGTU
            if((unsigned ) vm->Registers[operands[0]] > (unsigned ) vm->Registers[operands[1]])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        case SGTI:       ///SGTI
            if((signed ) vm->Registers[operands[0]] > (signed ) operands[1])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        case SGTIU:       ///SGTIU
            if((unsigned ) vm->Registers[operands[0]] > (unsigned ) operands[1])
            {
                vm->Registers[operands[2]] = 1;
            }else{
                vm->Registers[operands[2]] = 0;
            }
            break;

        default:        ///something must have gone sideways
            printf("[-]ERROR::opecode %d doesn't represent a valid conditional instruction", opcode);
            vm->_status_running = 0;
            break;
    }

}
void unconditional_branch_operations();

///converts a signed half word to signed int
/// its practically a couple of explicit casting statements
/// but makes the code a whole lot easier to read and manage
signed int _convert_halfword_to_word(int number){

    ///casting it to signed short makes the MSB the sign bit
    /// then i will let c take care of the sign extension
    ///clever, huh?
    signed short num = (signed short) number;
    return (signed int) num;
}