#!/usr/bin/ruby
$LOAD_PATH << '.'

###############################################################################
#           this module check if the given operation is valid or not          #
###############################################################################

require 'op_codes.rb'
require 'argument_validity.rb'

module Instruction_Validity

  def Instruction_Validity.operation_validity(no_label_codeline)

    
    # token_arg is a temporary varible use for tokenization
    # take the string and substitute white spaces by commas
    # token_arg = no_label_codeline.split(' ').join(',')
    
    # now tokenize by commas
    token_arg =  no_label_codeline.split(' ')

    temp=token_arg[0].upcase
    if temp==nil
      op=token_arg[0]
    else
      op=temp
    end

    #remove the instruction and join the arguements
    arg_string = token_arg[1...token_arg.length].join(' ') 
    #  remove the space and tabs from the line
    arg_string.delete!(" \t" )        

    #check if the arguments are valid
    if (Opcodes::Opcode).include?(op)
      # first r-type
      if Opcodes::Op_type["R_type"].include?(op)
        args= ArgumentValidity.argument_validity(1 , arg_string)
        type= "R_type"
      # second i-type
      elsif Opcodes::Op_type["I_type"].include?(op)
        args= ArgumentValidity.argument_validity(2 , arg_string)
        type= "I_type"
      # then j-type
      elsif Opcodes::Op_type["J_type"].include?(op)
        # the statement checks if the jump statement takes in integer or label arguments
#        if (op=="J" and (arg_string!=sprintf("0x%x" , arg_string.hex) and arg_string!=sprintf("0X%x" , arg_string.hex) and arg_string!= sprintf("0%o" , arg_string.oct) and arg_string!=arg_string.to_i.to_s)) and 
#          return ["J_type_label",Opcodes::Opcode[op],arg_string]
#        else
          # for when the arguments are not label type contune to argument validation
          args= ArgumentValidity.argument_validity(3 , arg_string)
          type= "J_type"
#        end
      end

      if args!= 111111

        return args.unshift(Opcodes::Opcode[op]).unshift(type)
      else return 111111
        
      end
      
    else
      return 222222
    end
    
  end
  # left for testing the module individually
#=begin
     puts operation_validity("ADD $t1 ,$t2 , $t3  ")
     puts operation_validity("ADD $Z1 , $T3 , $y1 ")
     puts operation_validity("ass $32 , $32 , $32")
     puts operation_validity("J 23")
#=end
end
