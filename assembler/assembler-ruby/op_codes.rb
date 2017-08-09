#!/usr/bin/ruby

###############################################################################
#  this file contains a list of all the valid instructions for our processor  #
###############################################################################

# module defined op_codes.rb file

module Opcodes

  Opcode = Hash["ADD"   =>  "000000" ,
                "SUB"   =>  "000001" ,
                "MUL"   =>  "000010" ,
                "DIV"   =>  "000011" ,
                "AND"   =>  "000100" ,
                "OR"    =>  "000101" ,
                "NOR"   =>  "000110" ,
                "XOR"   =>  "000111" ,
                "SLT"   =>  "001000" ,
                "SLTU"  =>  "001001" ,
                "SGT"   =>  "001010" ,
                "SGTU"  =>  "001011" ,
                "SLL"   =>  "010010" ,
                "SRL"   =>  "010011" ,
                
                "ADDI"  =>  "001100" ,
                "ANDI"  =>  "001101" ,
                "ORI"   =>  "001110" ,
                "LW"    =>  "001111" ,
                "SW"    =>  "010000" ,
                "LUI"   =>  "010001" ,
                "BEQ"   =>  "010100" ,
                "BNE"   =>  "010101" ,
                "SLTI"  =>  "010110" ,
                "SLTIU" =>  "010111" ,
                "SGTI"  =>  "011000" ,
                "STGIU" =>  "011001" ,
                "JR"    =>  "011010" ,
                
                "J"     =>  "011011" ,
                "JAL"   =>  "011100",
                "HALT"  =>  "011101",
                "INT"   =>  "011110" ,
                "UNDEFINED" =>"011111" ]

  Op_type = Hash["R_type" => ["ADD" ,
                              "SUB" ,
                              "MUL" ,
                              "DIV" ,
                              "AND" ,
                              "OR"  ,
                              "NOR" ,
                              "XOR" ,
                              "SLT" ,
                              "SLTU",
                              "SGT" ,
                              "SGTU",
                              "SLL" ,
                              "SRL" ] ,
                 "I_type" =>["ADDI",
                             "ANDI" ,
                             "ORI"   ,
                             "LW"    ,
                             "SW"    ,
                             "LUI"   ,
                             "BEQ"   ,
                             "BNE"   ,
                             "SLTI"  ,
                             "SLTIU" ,
                             "SGTI"  ,
                             "STGIU" ,
                             "JR"    ],
                 "J_type" => ["J" ,
                              "JAL" ,
                              "HALT" ,
                              "INT" ,
                              "UNDEFINED" ]]

  

end

