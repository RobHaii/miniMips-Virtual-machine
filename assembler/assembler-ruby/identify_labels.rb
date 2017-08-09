#!/usr/bin/ruby

module Identify_Labels
  def Identify_Labels.identify()
    label_array=Hash.new
    counter =1
    File.open("assemblycode.txt", "r") do |infile|
      
      while (line = infile.gets)
        
        token_l0 = "#{line}".split(":")

        #label_array=Hash.new()
        
        # identify if a line is a label by its self
        if token_l0.length==2 and token_l0[1]=="\n"
          if label_array.include?(token_l0[0])
            label_array=nil
            label_array=44444
          else
            label_array[token_l0]=counter
          end
        # identify if a line has a label at the beining
        elsif token_l0.length==2 and  token_l0[1]!="\n"
          if label_array.include?(token_l0[0])
            label_array=nil
            label_array = 444444
            counter+=1
          else
            label_array[token_l0[0]] = counter
            counter+=1
          end
        end
                
       
        
      end
    end
    return label_array #, counter
  end
end
begin
puts Identify_Labels.identify()
end
