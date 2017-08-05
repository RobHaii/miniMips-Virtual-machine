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

    //printf("[Debug]::PC 0x%x\n", vm->$pc);

    ///increment the program counter to the next instruction
    vm->$pc += 1;
}

void decode(){

    opcode = (_inst_buffer & 0xfc000000) >> 26; ///read the 6 MSBits into an integer value
    if(opcode >= ADD && opcode <= SRL)                     ///-- parse R-TYPE
    {
        parse_args(R_TYPE);
    }else if(opcode >= ADDI && opcode <= JR)              ///-- parse I-TYPE
    {
        parse_args(I_TYPE);
    }else if(opcode >= J && opcode <= INT)              ///-- parse J-TYPE
    {
        parse_args(J_TYPE);
    }else{
        printf("[-]ERROR::Unknown opcode %d found in instruction -> [%x] %x", opcode, vm->$pc, _inst_buffer);

        ///kill the virtual machine
        vm->_status_running = 0;
    }
}

void execute(){
//    printf("[Debug]::Executing instruction\n");
    switch (opcode)
    {
            ///arithmetic operations (5)
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case ADDI:
            arithmetic_operations();
            break;

            ///logical operations (8)
        case AND:
        case OR:
        case NOR:
        case XOR:
        case ANDI:
        case ORI:
        case SLL:
        case SRL:
            logical_operations();
            break;

            ///memory access operations (3)
        case LW:
        case SW:
        case LUI:
            memory_access();
            break;

            ///conditional operations (10)
        case BEQ:
        case BNE:
        case SLT:
        case SLTU:
        case SLTI:
        case SLTIU:
        case SGT:
        case SGTU:
        case SGTI:
        case SGTIU:
            conditional_operations();
            break;

            ///unconditional branch operations (3)
        case J:
        case JR:
        case JAL:
            unconditional_branch_operations();
            break;

            ///halt
        case HALT:
            halt();
            break;


            ///interrupts
        case INT:
            interrupt();
            break;


            ///default --- ERROR
        default: ///something must have gone terribly wrong
            ///print error message and quit
            printf("[-]ERROR::opecode %d doesn't represent a valid arithmetic instruction\n", opcode);
            vm->_status_running = 0;
            break;

    }


}

void run(char *filename){
    Word pc = vm->$pc;
    printf("[+]::Starting machine...\n");
    load_program(filename);
    while(vm->_status_running)
    {
        fetch();
        decode();
        if(vm->_status_running)
            execute();
        else break;

        if(vm->$pc > pc + 40)
            break;
    }
    printf(" Done\n");
}

void parse_args(int instruction_type){
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
            operands[2] = _convert_halfword_to_word(_inst_buffer & 0x0000ffff);          ///Immediate -- converts the
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


    FILE *handle;
    handle = fopen(filename, "rb+");

    if(!handle)
    {
        printf("[ERROR]::No such file [%s]. please provide a valid program file\n", filename);
        vm->_status_running = 0;
        return;
    }

    int prog_size;
    prog_size = binary_size(handle);
    printf("Program size: %d Bytes\n", prog_size);
    Word inst;
    for(int i = 0; i < prog_size; i++)
    {
        fread(&inst, sizeof(Word), 1, handle);
        vm->RAM[vm->$pc+i] = (Word) inst;
        inst = 0;
    }




    ///dummy instructions for testing only
    ///write 6 dummy instructions
    ///for testing

    ///common instructions  - for testing only
    ///Arithmetic
    /// -   Halt                0x74000000
    /// -   Add $t2, $t0, $t1   0x01095000
    /// -   Sub $t2, $t0, $t1   0x05095000
    /// -   mul $t2, $t0, $t1   0x09095000
    /// -   div $t2, $t0, $t1   0x0d095000

    /*{
        ///print hello program
        0x44084865,
        0x39086c6c,
        0x43880000,
        0x44086f00,
        0x43880004,
        0x001c2000,
        0x30050001,
        0x78000000
    }*/

//    //print hello function
//    int instructions[] = {
//            0x44084865,
//            0x39086c6c,
//            0x43880000,
//            0x44086f0a,
//            0x43880001,
//            0x001c2000,
//            0x30050001,
//            0x78000000,
//            0x74000000};  /// halt
//    for(int j = 0; j < 10; j++)
//    {
//        vm->RAM[vm->$pc+ j] =  instructions[j];
//    }
}

///helper functions to help the main execute function
///modularization make life a whole lot easier, so i have heard!!

///basic arithmetic operations : ADD, SUB, ADDI, DIV, MUL
///memory access operations [opcode: 5 OPERATIONS]
void arithmetic_operations(){
    long int result, dividend, divisor;
//    printf("Opcode: %d Operand1: %d Operand2: %d Operand3: %d\n", opcode, operands[0], operands[1], operands[2]);
    switch (opcode)
    {
        case ADD:     ///ADD
            vm->Registers[operands[2]] = (Word) (vm->Registers[operands[0]] + vm->Registers[operands[1]]); ///ignores overflow by casting result to 32 bit int
//            printf("[Debug]\tAddition %d + %d = %d\n", vm->Registers[operands[0]],
//                   vm->Registers[operands[1]],
//                   vm->Registers[operands[2]]
//            );
            break;

        case SUB:     ///SUB
            vm->Registers[operands[2]] = (Word) (vm->Registers[operands[0]] - vm->Registers[operands[1]]);
//            printf("[Debug]\tSubtraction %d - %d = %d\n", vm->Registers[operands[0]],
//                   vm->Registers[operands[1]],
//                   vm->Registers[operands[2]]
//            );
            break;

        case ADDI:     ///ADDI
            vm->Registers[operands[1]] = (Word) (vm->Registers[operands[0]] + operands[2]);
//            printf("[Debug]\tAddition Immediate %d + %d = %d\n", vm->Registers[operands[0]],
//                   (unsigned int) operands[2],
//                   vm->Registers[operands[1]]
//            );
            break;

            ///the next two implemetations are a possible source of bug...
            ///you might want to take a good and let me know if i am missing something
            ///a fresh pair of eyes - take em when ever you can find em

        case MUL:     ///MUL      implicitly uses register %t8 for result [$t8][$rd] = $Rs * $Rt
            result = vm->Registers[operands[0]] * vm->Registers[operands[1]];
            vm->Registers[operands[2]] = result & 0xffffffff;   ///lower word stored to the destination register
            vm->Registers[$t8] = (Word) ((result & 0xffffffff00000000) >> 32);
//            printf("[Debug]\tMultiplication %d * %d = [%d][%d] \n", vm->Registers[operands[0]],
//                   vm->Registers[operands[1]],
//                   vm->Registers[$t8],
//                   vm->Registers[operands[2]]
//            );
            break;

        case DIV:     ///DIV     implicitly uses the $t8 to retrieve higher word of dividend
            dividend = (long int) vm->Registers[$t8];  ///copy the content of register $t8 to a long variable (64 bit)
            dividend = dividend << 32;                  ///shift it left 32 times
            dividend |= vm->Registers[operands[0]];     ///append the content of the dividend rgister to the long var lower word
            divisor = vm->Registers[operands[1]];

            ////check for division by zero
            if(divisor == 0)
            {
                printf("[-]ERROR::Division by zero occurred at instruction [%x]: %x", (Word ) vm->$pc-4, (Word ) vm->RAM[vm->$pc - 4]);
                vm->_status_running = 0;
                return;
            }
            vm->Registers[operands[2]] = (Word ) (dividend / divisor); /// devide the logn var by the divisor and put the result in destination register
//            printf("[Debug]\tDivision [%d][%d] / %d = %d \n",vm->Registers[$t8],
//                   vm->Registers[operands[0]],
//                   vm->Registers[operands[1]],
//                   vm->Registers[operands[2]]
//            );
            break;


        default:    ///something must have gone terribly wrong
            ///print error message and quit
            printf("[-]ERROR::opecode %d doesn't represent a valid arithmetic instruction\n", opcode);
            vm->_status_running = 0;
            break;
    }
}

///memory access operations [opcode: 3 OPERATIONS]
void memory_access(){
    long int effective_addr = 0;
    int immediate;
//    printf("[Debug]::Accessing memory\n");
    int data;

//    printf("[Debug]:: Operand1: %d operand2: %d Operand3: %d", operands[0], operands[1], operands[2]);
    switch (opcode)
    {

        case LW:     ///load word
            effective_addr = vm->Registers[operands[0]] + (operands[2]);             ///synthesize the effective address
            vm->Registers[operands[1]] = (Word ) vm->RAM[effective_addr];      ///load the data from memory to the register
//            printf("[Debug]::\tLoading data from memory\n");
            break;

        case SW:     ///store word
            effective_addr = vm->Registers[operands[0]] + (operands[2]);             ///synthesize the effective address
            data = vm->Registers[operands[1]];
            vm->RAM[effective_addr] = data;     ///load the data from memory to the register
//            printf("[Debug]::\tStoring data to memory [0x%x] %x\n", (unsigned int) effective_addr, vm->RAM[effective_addr]);
            break;

        case LUI:     ///load upper two bytes with immediate value -> helps create effective mem address
            immediate = (operands[2] & 0x0000ffff) << 16;
            vm->Registers[operands[1]] = 0x00;  ///clear previous data
            vm->Registers[operands[1]] |= immediate;
//            printf("[Debug]::\tLoading data to upper half register %x\n", operands[2]);
            break;

        default:    ///something must have gone sideways
            printf("[-]ERROR::opecode %d doesn't represent a valid memory access instruction\n", opcode);
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
            vm->Registers[operands[2]] = vm->Registers[operands[1]] <<  operands[3];
            printf("[Debug]::Shift left 0x%x << %d = 0x%x\n",
                   vm->Registers[operands[1]],
                   operands[3],
                   vm->Registers[operands[2]]
            );
            break;
        case SRL:       ///SRL
            vm->Registers[operands[2]] = vm->Registers[operands[1]] >>  operands[3];
            printf("[Debug]::Shift left 0x%x >> %d = 0x%x\n",
                   vm->Registers[operands[1]],
                   operands[3],
                   vm->Registers[operands[2]]
            );
            break;
        default:        ///something must have gone sideways
            printf("[-]ERROR::opecode %d doesn't represent a valid logical instruction\n", opcode);
            vm->_status_running = 0;
            break;

    }
}


///conditional operations [10 operations]
void conditional_operations(){
    int addr = vm->$pc;
    switch (opcode)
    {
        case BEQ:       ///BEQ  branch if equal -->pc relative
            if(vm->Registers[operands[0]] == vm->Registers[operands[1]])
            {
                addr += (operands[2] << 2);
                vm->$pc = addr;
            }
            break;

        case BNE:       ///BNE  branch if not equal  ->pc relative
            if(vm->Registers[operands[0]] != vm->Registers[operands[1]])
            {
                addr += (operands[2] << 2);
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
            printf("[-]ERROR::opecode %d doesn't represent a valid conditional instruction\n", opcode);
            vm->_status_running = 0;
            break;
    }

}

///basic unconditional branch operations
/// 3 operations
void unconditional_branch_operations(){
    Word address;
    switch(opcode)
    {
        case J:         ///jump to a certain address
            address = operands[0] << 2;
            address &= 0x0ffffff0;   ///clean up the address
            vm->$pc &= 0xf0000000;  ///clean up the pc for writing
            vm->$pc |= address;
            break;
        case JR:        ///jump to address stored in register
            address = vm->Registers[operands[0]] << 2;
            address &= 0x0ffffff0;   ///clean up the address
            vm->$pc &= 0xf0000000;  ///clean up the pc for writing
            vm->$pc |= address;
            break;

        case JAL:       ///jump and link to certain address
            vm->Registers[$ra] = vm->$pc;
            address = vm->Registers[operands[0]] << 2;
            address &= 0x0ffffff0;   ///clean up the address
            vm->$pc &= 0xf0000000;  ///clean up the pc for writing
            vm->$pc |= address;
            break;
        default:///something must have gone sideways
            printf("[-]ERROR::opcode %d doesn't represent a valid unconditional branch instruction\n", opcode);
            vm->_status_running = 0;
            break;
    }
}


/// very simple inmplementation of halt function
///only 1 operation
void halt(){
    if(opcode == HALT)
        printf("[+]Halting ....");
    else{
        ///something must have gone sideways
        printf("[-]ERROR::opcode %d doesn't represent a valid halt instruction\n", opcode);
    }
    vm->_status_running = 0;
}

void interrupt() {
    ///handle instruction based on instruction

    int type;
    unsigned int address;
    Word Data;

    ///parse the arguments of the interrupt
    address = vm->Registers[$a0];
    type = vm->Registers[$a1];

    if (operands[0] == INT_IO_WRITE)        ///write
    {
        if (type == DATA_TYPE_INT)       ///write integer
        {
            Data = vm->RAM[address];
            printf("%d", Data);

        } else if (type == DATA_TYPE_CHAR) {
            char _word_buffer[4];
            int flag = 0;           ///flags if null byte has been found
            while (!flag)
            {
                Data = vm->RAM[address];
                parse_word_to_char(Data, _word_buffer);
                for (int i = 0; i < 4 && !flag; i++) {
                    if (_word_buffer[i] != 0x00) {
                        printf("%c", _word_buffer[i]);
                    }else{
                        flag = 1;
                        break;
                    }
                }
                address++;
            }
        } else {      ///Uknown data type
            printf("[-]ERROR::Invalid data type %d for read and write. {Int: 0 | Char_Seq: 1} \n", opcode);
            vm->_status_running = 0;
            return;
        }
    } else if (operands[1] == INT_IO_READ)      ///read
    {
        if (type == DATA_TYPE_INT)       //read integer
        {
            scanf("%d", &Data);
            vm->RAM[address] = Data;

        } else if (type == DATA_TYPE_CHAR)    ///read char [seq]
        {
            char buffer[512];
            char _word_buffer[4];
            char d;
            int flag = 0;       ///flags if end of line has been read
            for (int i = 0; i < 128 && flag == 0; i++) {
                for (int j = 0; j < 4 && flag == 0; j++) {
                    scanf("%c", &d);
                    _word_buffer[j] = d;
                    if ((d == '\n') || (d == '\r')) {
                        flag = 1;
                        vm->Registers[$v0] = ((i * 4) + (j + 1));     ///return the number of chars read
                        break;
                    }
                }
                parse_word_from_chars(&Data, _word_buffer);
                vm->RAM[address + i] = Data;

                ///clean the buffer data
                _word_buffer[0] = 0x00;
                _word_buffer[1] = 0x00;
                _word_buffer[2] = 0x00;
                _word_buffer[3] = 0x00;
                Data = 0;

                if (flag == 1) {
                    break;
                }
            }
        } else {  ///Uknown data type
            printf("[-]ERROR::Invalid data type %d for read and write. {Int: 0 | Char_Seq: 1} \n", opcode);
            vm->_status_running = 0;
            return;
        }
    } else {      ////something must have gone sideways
        printf("[-]ERROR::interrupt code %d doesn't represent a valid interrupt routine\n", opcode);
        vm->_status_running = 0;
        return;
    }
}
void parse_word_to_char(Word Data, char *array){
    array[3] = (unsigned char )Data & 0x000000ff;
    array[2] = (unsigned char )((Data & 0x0000ff00) >> 8);
    array[1] = (unsigned char )((Data & 0x00ff0000) >> 16);
    array[0] = (unsigned char )((Data & 0xff000000) >> 24);
}

void parse_word_from_chars(Word *Data, char arr[]){
    *Data = 0x00;
    *Data = (((((*Data | arr[0]) >> 8) | arr[1]) >> 8 | arr[2]) >> 8) | arr[3];
}

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



///helper function to help the loader and linker load the progrma file
int binary_size(FILE *handle){
    int size;
    fseek(handle, 0L, SEEK_END);
    size = ftell(handle);
    rewind(handle);
    return size;
}