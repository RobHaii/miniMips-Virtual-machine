# miniMips Software Defined Processor 
*(A.K.A Virtual Machine)*

## 1. Introduction

This project demostartes implemetation of a simple 32-bit miniature virtual machine in c. It uses a simplified version of the MIPS Instruction set architecture (**ISA**). Our simplified ISA follows most of the basic conventions and philosophies behind the design of MIPS-32 ISA. There are, however, a few *non-MIPS* features we have inlcluded just to make our virtual machine do some cool stuff, like read/write to/from terminal with out the need for operating system, but these changes are *very* minor and it should all be easy for anybody to get the hang of it, especially if you already know **MIPS-32**.

## 2. ISA Description
The instruction set architectue we used is really very much like MIPS. We kept all the core conventions and philosophies of MIPS design intact while attempting to reduce the ISA. We mentained data encoding conventions, simplicity and elegance of the Instruction set.

Our ISA has 32 registers (thus needing 5 bits to address them) and a program counter that is not part of the register file. All instructions are orthogonal interms of register access, you can do what ever you want to which ever register of your liking. Here are some pointers to keep in mind:
	
    1. All data manipulation instructions are regiter based, as it is the case with MIPS-32.
    
    2. we use load/store approach.
       we are not intel after all. you cannot read data from memory, add to it and save it in memory all
       in one instrution like add [mem], EAX. you want to add to it? then load it to register first... 
       remember pointer number one???
       
    3. All instructions are orthogonal interms of register access. 
       do what you will with them... just dont complain when you overwrite your own data.
       
    4. The RAM is an array of Words (32-bits) not bytes(8 bits).
    	This has a very weird and unussual implication... memory offsets need not be multiple of four (4). 
        Data and instruction in memory is aligned, wether you want it to be or not. so PC (program 
        counter) relative offset of 1 is actually a valid offset.
        
        Trust me, it is going to make your life a whole lot easier.
    
    5. All branches, be it near jump or "far jump", its all pc relative.
       we dont plan to make an os for the VM so we dont expect it to have a RAM grater than 1GB.
       
    6. We only support integer operations. and strings too... only for reading and writing them though.
       you can implement your own string libraries if you want



Every instruction supported by our virtual machine belongs to one of the seven (7) instruction classes: 

1. **Arithmetic operations**: imple arithmetic operations ADD, ADDI, SUB, MUL, DIV..

2. **Logical operations**: simple logical ops AND, OR, NOR, XOR
 
3. **Memory access operations**: memory access instructions LW, SW, LUI..

4. **Conditional operations**: used for testing conditions BEQ, BNE, SLT

5. **Unconditional operations**: ... J, JR, JAL

6. **Halt**:  used to terminate the machine after end of execution of program 

7. **Interrupts**: used to perform simple non primitive ops like read/write to/from terminal

    
    please see the detailed insruction set list below for further info!!
    

These instruction classes are classified based on the type of operations they perform, not based on the encoding methods used. We followed the MIPS-32 instruction encoding format which has three instruction encoding schemes, R-type, I-type, J-type. every instruction is 32 bits long and each encoding scheme is as close to the other encoding schemes to reduce decoding hardware compelexity.  

figure below demostrates the three schemes:

	R-type: for register based instruction 
	| Opcode (6 bits) |	Rs (5 bits) | Rt (5bits) | Rd (5 bits) | shamt (5 bits) | function (6 bits) |
    
    *We only used the opcode eventhough mips uses both opcode and function fileds.
    
    	opcode: determines the type of instruction
        Rs: source register one
        Rt: source register two
        Rd: destination register
        shamt: shift ammoun (for logical shift)
        function: not used in our implementation
    
    I-type: for immediate based instruction
    | Opcode (6 bits) |	Rs (5 bits) | Rd (5 bits) | Immediate value (16 bits) |
    
    
    	opcode, Rs, Rd: same as above
        Immediate value: immediate value
    
    J-type: For Jump instructions
    | Opcode (6 bits) | PC realtive offset (26 bit) |
    
    	PC relative offset: all branches are PC realtive. remeber pointer 5??
        
 ##### *** Here is the complete list of resgiters and their encoding:
 
|Regiter name	| id			|
| --------------|:-------------:|
| $zero			| 0 			|  
| $at  			| 1 			|  
| $v0  			| 2 			|
| $v1  			| 3 			|
| $a0  			| 4 			|
| $a1  			| 5 			|
| $a2  			| 6 			|
| $a3  			| 7 			|
| $t0  			| 8 			|
| $t1  			| 9 			|
| $t2  			| 10			| 
| $t3  			| 11			| 
| $t4  			| 12			| 
| $t5  			| 13			| 
| $t6  			| 14			| 
| $t7  			| 15			| 
| $s0  			| 16			| 
| $s1  			| 17			|  
| $s2  			| 18			| 
| $s3  			| 19			| 
| $s4  			| 20			| 
| $s5  			| 21			| 
| $s6  			| 22			| 
| $s7  			| 23			| 
| $t8  			| 24			| 
| $t9  			| 25			| 
| $k0  			| 26			| 
| $k1  			| 27			|
| $gp  			| 28			|  
| $sp  			| 29			| 
| $fp  			| 20			| 
| $ra  			| 31			| 


##### *** Here is the complete list of instructions and their encoding:
| opcode    |    Mnemonics          |   Description                         |    
| --------- | --------------------- | ------------------------------------- |
|           |         **R-Type**        |                                      					|       
| 0         | ADD                   |	Addition 	eg. ``` add $t0, $t1, $t2```	 => $t0 = $t1 + $t2 |           
| 1         | SUB                   |   subtraction eg. ``` sub $t0, $t1, $t2``` 	=> $t0 = $t1 - $t2  	|       
| 2         | MUL                   |   Multiplication eg. ``` mul $t0, $t1, $t2```	 => [$t8][$t0] = $t1 * $t2  	|       
| 3         | DIV                   |   devision eg. ``` div $t0, $t1, $t2``` 	=> $t0 = [$t8] [$t1] / $t2  	|       
| 4         | AND                   |   logical and eg. ``` and $t0, $t1, $t2```	 => $t0 = $t1 & $t2  	|       
| 5         | OR                    |   logical or eg. ``` and $t0, $t1, $t2``` 	=> $t0 = $t1 | $t2  	|      
| 6         | NOR                   |  	logical nor eg. ``` and $t0, $t1, $t2``` 	=> $t0 = ~($t1 | $t2)  	|      
| 7         | XOR                   |   logical xor eg. ``` and $t0, $t1, $t2``` 	=> $t0 = $t1 ^ $t2  	|      
| 8         | SLT                   |  	set less than (signed comparison) eg: ``` slt $t0, $t1, $t2``` 	=> $t0 = 1 iff $t1 < $t2, else $t0 = 0  	|      
| 9         | SLTU                  | set less than (unsigned comparison) eg: ``` slt $t0, $t1, $t2``` 	=> $t0 = 1 iff $t1 < $t2, else $t0 = 0  	|     
| 10        | SGT                   | set greater than (signed comparison) eg: ``` slt $t0, $t1, $t2``` 	=> $t0 = 1 iff $t1 > $t2, else $t0 = 0  	|      
| 11        | SGTU                  | set greater than (unsigned comparison) eg: ``` slt $t0, $t1, $t2``` 	=> $t0 = 1 iff $t1 > $t2, else $t0 = 0  	|      
| 12        | SLL                   | shift left logical   eg: ``` sll $t0, $t1, #shamt (shift ammount)```  	=> $t0 = $t1 << #shamt          |      
| 13        | SRL                   | shift right logical   eg: ``` srl $t0, $t1, #shamt (shift ammount)```  	=> $t0 = $t1 >> #shamt           |  
|			|						|										|
|           |       **I-TYPE**                                                  |       
| 14        | ADDI                  |  Addition with immediate data	eg: ``` addi $t0, $t1, #Immediate```	 => $t0 = $t1 + #Immediate |      
| 15        | ANDI                  | AND with immediate MASK	eg: ``` andi $t0, $t1, #Immediate```	 => $t0 = $t1 & #mask |      
| 16        | ORI                   | OR with immediate MASK	eg: ``` ori $t0, $t1, #Immediate```	 => $t0 = $t1 | #mask |        
| 17        | LW                    | load word rto register from memory address (offset + reference address in register) eg:  ``` lw $t0, 1($gp) ``` => [$t0] <- memory[[$gp]+1] |       
| 18        | SW                    |store word from regsiter to memory address (offset + reference address in register) eg:  ``` lw $t0, 1($gp) ``` => [$t0] -> memory[register[$gp]+1]|       
| 19        | LUI                   |load half word to upper nibble of register with immediate value (offset + reference address in register) eg:  ``` lui $t0, #immediate ``` => $t0 = (#immediate << 16) 0xff00        
| 20        | BEQ                   | Branch if equal eg:   ``` beq $t0, $t1, label ``` => jump to label if $t0 == $t1 |       
| 21        | BNE                   | Branch if not equal eg:   ``` bne $t0, $t1, label ``` => jump to label if $t0 != $t1  |       
| 22        | SLTI                  | set less than Immediate (signed comparison) eg: ``` slti $t0, $t1, #immediate``` 	=> $t0 = 1 iff $t1 < #immediate, else $t0 = 0  	|       
| 23        | SLTIU                 |set less than Immediate (unsigned comparison) eg: ``` slti $t0, $t1, #immediate``` 	=> $t0 = 1 iff $t1 < #immediate, else $t0 = 0  	|       
| 24        | SGTI                  | set greater than Immediate (signed comparison) eg: ``` slti $t0, $t1, #immediate``` 	=> $t0 = 1 iff $t1 > #immediate, else $t0 = 0   |       
| 25        | SGTIU                 |set greater than Immediate (unsigned comparison) eg: ``` slti $t0, $t1, #immediate``` 	=> $t0 = 1 iff $t1 > #immediate, else $t0 = 0  	|       
| 26        | JR                    | Jump to address in register eg: ``` jr $t0``` => PC <- $t0  |       
|           |                       |                                       |       
|           |   **J-TYPE**                                                      |       
| 27        | J                     | jump to label eg:``` j end ``` => jump to address represented by the label |       
| 28        | JAL                   | jump and link eg:``` jal end ``` => save the return address in register $ra (31) and jump to address represented by the label |       
| 29        | HALT                  | terminate program execution and halt eg:```halt ``` |       
| 31        | UNDEFINED             |        [not used yet... perhaps we can make it a NOP operation     |

##
      
      
##

## 3. Virtual Machine Description

Here is a c code defination of our virtual machine. It has a register file, a program counter that is not in the register file, Random Access Memory (RAM) and a status flag that indicates the machine state as ON/OFF.

```c

struct Vitrual_Machine{
    Word $pc;
    Word *Registers;
    Word *RAM;
    int _status_running;
};

typedef struct Vitrual_Machine VM;
```
The Data type 'Word' is defined in the common-libs.h file as:

```c
	//define the data types
	typedef unsigned char Byte;         //8bits
	typedef unsigned short HalfWord;    //16 bits
  ->typedef unsigned int Word;          //32bits
	typedef unsigned long DWord;        //64 bits

```

The virtual machine runs on an infinite loop of Fetch, Decode and Execute operations with the status flag  *_status_running* used as a flag for machine termination (Halting). Code segment below demonstrates this:

```C
	...
	load_program(filename);		//load the program in binary file given
    while(vm->_status_running)
    {
        fetch();
        decode();
        if(vm->_status_running)		//see if decode instruction went well... decode error is 'FATAL'
            execute();
        else break;
    }
```

### Memory layout

We used a flat memory model with three segments: Reserved (for boot image), Text area and Stack area. Image below describes the memroy scheme used.
```
+-------------------+
|    Reserved       |
+-------------------+   <- 0x0040 $pc
|       Text        |
+-------------------+
|   Static Data     |   <- 0x1000 $gp
+-------------------+
|                   |
|   Dynamic data    |
|                   |
|                   |
|                   |
|                   |
|                   |
|   Stack area      |
+-------------------+   <- 0xffff  $fp, $sp
```


### Object file format

We designed our own object file format with minimal features to support all our primitive instructions. The figure below demonstarates the object format we used:

```c
+---------------------------+
|    Header (4 Words)		|
+---------------------------+	<---- text_start
|       Text Section   		|
|        					|	<---- entry_point
|                   		|
|   						|
|                   		|
|                   		|
+---------------------------+	<--- data_start
|         Data Section 		|	
|                   		|
|              		  		|
+---------------------------+  


//object header structure
struct Obj_Header{
    int text_size;			// size of the text area
    int text_start;			// begining of the text section	--ironiclly this is always 4 (Fifth word)
    int entry_point;		// entry point for the executable -- location of the 'main'
    int data_start;			// starting point of data section
}__attribute__((__packed__));

typedef struct Obj_Header O_Header;
```

* _The program loder and linker copies the entire text section to memory area begining at the current value of PC. it then loads the Data section to the static data section of the memory just below the current value of global pointer ($gp)_

## 4. Assembler 

We have developed two assemblers in python and ruby. These assemblers generate an object file for assembly files given as argument. 

usage (for assembler in python):
```bash
./Assembler.py -i <assemblyfile.404> -o <output-object.o>
```
##
_The Assembler doesnt really care if the file format of the assembly file really is *.404 or not. it will work as long as you give it a valid ASCII file. but stick to giving it a file with .404 file format.... show respect to the deelopers and the terms of agreement._


##  5. VM Usage
 ```bash
	#navigate to the virtual machine directory
    #then compile the virtual machine using the makefile provided as:
  	$ make
    
    #run the virtual machine wkith the target program object file as argument
    
    $ ./vm prog_file.o
  
  ```