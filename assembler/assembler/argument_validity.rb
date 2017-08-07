#!/usr/bin/ruby
$LOAD_PATH << '.'

############################################################################################
# this function is used to validate and
# tokenize the arguments provided after instruction
############################################################################################

require 'registers'

module ArgumentValidity
 

  def ArgumentValidity.argument_validity(instruction_type , arg_string)     #takes argument type and the string as argument

    

    if instruction_type == 1                   #i.e. Rtype

      
      # token_arg is a temporary varible use for tokenization
      # take the string and substitute white spaces by commas
      token_arg = arg_string.delete!(" ")
      puts token_arg

      # now tokenize by commas
      token_arg = arg_string.split(',')
      #puts token_arg.length

      # if we have 3 valid registers
      if (token_arg.length == 3 \
          and (Registers::REG).include?(token_arg[0])   \
          and (Registers::REG).include?(token_arg[1])   \
          and (Registers::REG).include?(token_arg[2])
         ) 
        

        
        return_value = [Registers::REG[token_arg[0]],Registers::REG[token_arg[1]],Registers::REG[token_arg[2]]]

      else return_value = 111111

      end

    elsif instruction_type == 2

      # do simmilar clean up to the string
      token_arg = arg_string.delete!(" ")
      puts token_arg
      token_arg = arg_string.split(',')

      # if we have two valid registers ...
      if (token_arg.length == 3 \
          and (Registers::REG).include?(token_arg[0])   \
          and (Registers::REG).include?(token_arg[1])   \
         )                                   

        return_value=[Registers::REG[token_arg[0]],Registers::REG[token_arg[1]]]

        
        # ... and a valid number be it in octa hex or decimal
        if token_arg[2][0]!="0"
          return_value.push(sprintf("%016b",token_arg[2].to_i))
        elsif token_arg[2][2]=="X" or token_arg[2][2]=="x"
          return_value.push(sprintf("%016b", token_arg[2].hex))
        else
          return_value.push(sprintf("%016b", token_arg[2].oct))
        end

        return return_value
      else return_value = 111111
      end

    #similar checks for the j-type instructions
    elsif instruction_type == 3
      return_value=[arg_string]
      return return_value 
    end
  end
end

=begin
      if arg_string!="0"
        return_value = [sprintf("%026b", arg_string.to_i)]
      elsif arg_string=="X" or arg_string=="x"
        return_value = [sprintf("%026b",arg_string.hex)]        
      else
        return_value = [sprintf("%026b",arg_string.oct)]       
      end

    end
    # make sure address size does not exceed 26 bits
    if return_value[0].size<=26
      return return_value
    else
      return 333333
    end

=end

  # code snippet fro testing the module individually
#==begin
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 1 ,  "$v0 , $v1 , $zero")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 1 ,  "$z3,$z2,$42") 
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 2 ,  "$r2,$r1,2312312")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 2 ,  "$t2,$t3,22")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 2 ,  "$t2,$t3,022")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 2 ,  "$t2,$t3,0x22")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 2 ,  "$t2,$t3,0X22")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 3 ,  "123132132")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 3 ,  "0x22")
     print "\n================================================\n"
     print ArgumentValidity.argument_validity( 3 ,  "022")
     print "\n================================================\n"
#==end
