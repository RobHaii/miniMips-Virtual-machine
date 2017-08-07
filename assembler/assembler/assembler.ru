#!/usr/bin/ruby
$LOAD_PATH << '.'

require 'operation_validity.rb'
require 'registers.rb'
require 'op_codes.rb'
require 'identify_labels.rb'

counter =1
obj_file = File.new("obj_file.ob", "w")
File.open("assemblycode.txt", "r") do |infile|

  identified_labels = Identify_Labels.identify()
  
  while (line = infile.gets)
#	  puts "#{counter}: #{line}"

#      token = "#{counter}: #{line}".split(' ').join(' ').split("," ).join(" ")
#      puts token
      puts "====================================\n"
      puts "line #{counter}"
      puts "=====================================\n"
      token_l0 = "#{line}".split(":")
      puts token_l0
      puts token_l0.length
      temp_couter=0
      pc=1


      last_thing=Array.new()
      
      # identify if a line is a label by its self
      if token_l0.length==2 and token_l0[1]=="\n"
       #label_array.push([token_l0[0],counter])

      # identify if a line has a label at the beining
      elsif token_l0.length==2 and  token_l0[1]!="\n"
       # label_array.push([token_l0[0],counter])
        instructions = Instruction_Validity.operation_validity(token_l0[1])
        if instructions==222222 or instructions==111111 or instructions==333333
          obj_file.syswrite "error "
          obj_file.syswrite instructions
          obj_file.syswrite "\ on line "
          obj_file.syswrite counter
          obj_file.syswrite "\n"

        elsif instructions[1]=="J_type"
          pc+=1
          if identified_labels.include?(instructions[3])
            last_thing.push([instructions[2].to_s, [pc+1]])
            obj_file.syswrite"\n"
            obj_file.syswrite "011011"+sprintf("%026b", pc+1-identified_labels[instruction[2].to_s])
            obj_file.syswrite
          else
            break
          end
          
        else
          for el in 1...instructions.length
            obj_file.syswrite instructions[el].to_s
            temp_couter+= instructions[el].to_s.size
          end
          for el in temp_couter...32
            obj_file.syswrite "0"
          end
          obj_file.syswrite "\n"
          pc+=1          
        end
      else
        instructions=Instruction_Validity.operation_validity(token_l0[0])
        if instructions==222222 or instructions==111111 or instructions==333333
          obj_file.syswrite "error "
          obj_file.syswrite instructions
          obj_file.syswrite "\ on line "
          obj_file.syswrite counter
          obj_file.syswrite "\n"

        elsif instructions[0]=="J_type" and  instructions[1]=="011011"          
          pc+=1

          if identified_labels.include?(instructions[3])
            last_thing.push([instructions[2].to_s, [pc+1]])
          
            obj_file.syswrite"\n"
            obj_file.syswrite "011011"+sprintf("%026b", pc+1-identified_labels[instruction[2].to_s])
            obj_file.syswrite
          else
            break
          end
        else
          pc+=1
          for el in 1...instructions.length
            obj_file.syswrite instructions[el].to_s
            temp_couter+= instructions[el].to_s.size
          end
          if temp_couter<32
            for el in temp_couter...32
              obj_file.syswrite "0"
            end
          end
          obj_file.syswrite "\n"
        end
       # obj_file.puts instructions.type
      end

 #     if instructions==111111 or instructions==222222 or  instructions==333333
 #       obj_file.puts instructions
 #     else
 #       obj_file.puts "error"
#      end
        
      counter+=1
  end

      obj_file.close
end



