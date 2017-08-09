#!/usr/bin/env python

import argparse
import re
import pickle
import struct
import array

parser = argparse.ArgumentParser()
parser.add_argument('-o', '--output', type=str)
parser.add_argument('-i', "--inputfile", type=str)
arguments = parser.parse_args()

R_TYPE = 0
I_TYPE = 1
J_TYPE = 2

Entry = 0

opcodes = {
    'ADD' :     0,
    'SUB' :     1,
    'MUL' :     2,
    'DIV' :     3,
    'AND' :     4,
    'OR'  :     5,
    'NOR' :     6,
    'XOR' :     7,
    'SLT' :     8,
    'SLTU' :    9,
    'SGT' :     10,
    'SGTU' :    11,
    'SLL' :     12,
    'SRL' :     13,
    'ADDI' :    14,
    'ANDI' :    15,
    'ORI' :     16,
    'LW' :      17,
    'SW' :      18,
    'LUI' :     19,
    'BEQ' :     20,
    'BNE' :     21,
    'SLTI' :    22,
    'SLTIU' :   23,
    'SGTI' :    24,
    'SGTIU' :   25,
    'JR' :      26,
    'J' :       27,
    'JAL' :     28,
    'HALT' :    29,
    'INT' :     30,
    'UNDEFINED' :      31

}

regsiters = {
    "$zero" :   0,
    "$at" :     1,
    "$v0" :     2,
    "$v1" :     3,
    "$a0" :     4,
    "$a1" :     5,
    "$a2" :     6,
    "$a3" :     7,
    "$t0" :     8,
    "$t1" :     9,
    "$t2" :     10,
    "$t3" :     11,
    "$t4" :     12,
    "$t5" :     13,
    "$t6" :     14,
    "$t7" :     15,
    "$s0" :     16,
    "$s1" :     17,
    "$s2" :     18,
    "$s3" :     19,
    "$s4" :     20,
    "$s5" :     21,
    "$s6" :     22,
    "$s7" :     23,
    "$t8" :     24,
    "$t9" :     25,
    "$k0" :     26,
    "$k1" :      27,
    "$gp" :     28,
    "$sp" :     29,
    "$fp" :     20,
    "$ra" :     31
}

symbols = {}    #symbol table

instructions = []   #instructions buffer
patches = {}    #label patching table

inst_ = []      #buffer to hold the instruction segments

Data = {}

def lex(line):
    global inst_
    inst_ = re.split(' ,|, |,| | ,', line.strip('\n'))
    inst_ = [i for i in inst_ if i is not " " and i is not ""]

def lex_decl(line):
    pattern = '.string ([A-Za-z0-9_]*) = "([^"]*)"'
    res = re.findall(pattern, line)
    ret = [res[0][0], res[0][1]]
    return ret

def lex_entry(line):
    pattern = '.entry ([A-Za-z0-9_ -]*)'
    res = re.findall(pattern, line)
    return [res[0]]

def get_opcode(word):
    return opcodes.get(word, 31)

def get_register_code(word):
    return regsiters.get(word, -1)

def resolve(label):
    return symbols.get(label, 0)

def unroll(roll):
    pattern = "([\-]?[0-9]+)\((\$[A-Za-z]{1,4}[0-9]*)\)"
    res = re.findall(pattern, roll)
    return [res[0][0], res[0][1]]

def string_to_byte_stream(text):
    el = array.array('B', text)
    bArray = []
    #parse the byte sequence and store it in memory
    # print len(text)
    slices = []
    for i in range(0, len(text), 4):
        slices.append(el[i:i+4])

    if len(text) % 4 != 0:
        slices.pop()
        pad_len = 4 - (len(text) % 4)
        pad = [0x00] * pad_len
        last_slice = el[(pad_len - 4):]
        last_slice.extend(pad)
        slices.append(last_slice)

    # print "Len slices: ", len(slices)
    for sli in slices:
        # print sli
        data = 0x00
        data = (data | (sli[0])) << 8
        data = (data | (sli[1])) << 8
        data = (data | (sli[2])) << 8
        data = (data | (sli[3]))
        bArray.append(data)
    bArray.append(0x000000)
    return bArray

def retrive_Immediate(word):
    if word[:2].lower() == "0x" or word[:3].lower() == "-0x" :
        # print "converting from hex to int...", int(word, 16)
        return int(word, 16)
    elif Data.has_key(word):
        keys = Data.keys()
        index = keys.index(word)
        counter = 0
        # print Data
        for i in keys:
            if i == word:
                break
            counter += len(Data[i])
        # print "Offset: ", counter
        return counter
    else:
        return int(word)

#return 16bit two's somplement of a number
def parse_int_to_16b_signed(num):
    if num < 0:
        return 0xffff - ((-1 *num) - 1)
    else:
        return num

def extract_label(line):
    global symbols
    # extract lable
    if line.find(":") > 0:
        # print "this statement contains a label"
        index = line.find(":")
        label = line[:index]
        line = line[index + 1:]
        symbols[label] = len(instructions)
    return line

def extract_comment(line):
    if line[0] == "#":
        # print "Comment: ", line
        return True
    else:
        return False

def R_Encode(op_id):
    shamt = 0x00
    machine_code = 0x00
    rs = rd = rt = 0x00

    # check if the op is SLL SRL
    if op_id == 13 or op_id == 12:
        # print inst_
        shamt = retrive_Immediate(inst_[3])
        rt = get_register_code(inst_[1])
        rd = get_register_code(inst_[2])

    else:  # default
        # print "Opcode: ", op_id, inst_
        rs = get_register_code(inst_[2])
        rt = get_register_code(inst_[3])
        rd = get_register_code(inst_[1])

    machine_code |= op_id
    machine_code = (machine_code << 5) | (rs & 0x001f)
    machine_code = (machine_code << 5) | (rt & 0x001f)
    machine_code = (machine_code << 5) | (rd & 0x001f)
    machine_code = (machine_code << 5) | (shamt & 0x001f)
    machine_code = (machine_code << 6)
    # print("%d %d %d %d %d => 0x%x" % (op_id, rs, rt, rd, shamt, machine_code))
    return machine_code

def I_Encode(op_id):
    rs = rd = 0x00
    machine_code = 0x00
    I = 0x00
    # print op_id
    # check if the ops are LW or SW  R I(R) format
    if op_id == 17 or op_id == 18:
        rd = get_register_code(inst_[1])
        I, rs = unroll(inst_[2])
        rs = get_register_code(rs)
        I = parse_int_to_16b_signed(retrive_Immediate(I))

    elif op_id == 20 or op_id == 21:  # cehck if the ops are branch instructions (R R Label) format
        rs = get_register_code(inst_[1])
        rd = get_register_code(inst_[2])
        patches[inst_[3]] = len(instructions)

    elif op_id == 19:  # check if the op id is LUI (R I) format
        rd = get_register_code(inst_[1])
        I = retrive_Immediate(inst_[2])
    elif op_id == 26:  # check if the op is jump register
        rs = get_register_code(inst_[1])

    else:  # default format (R R I)
        rd = get_register_code(inst_[1])
        rs = get_register_code(inst_[2])
        I = retrive_Immediate(inst_[3])
    # print("%d %d %d %d" % (op_id, rs, rd, I))
    machine_code |= op_id
    machine_code = (machine_code << 5) | (rs & 0x0000001f)
    machine_code = (machine_code << 5) | (rd & 0x0000001f)
    machine_code = (machine_code << 16) | (I & 0x0000ffff)
    return machine_code

def J_Encode(op_id):
    machine_code = 0x00
    machine_code |= op_id
    # print("machine code: 0x%x " % machine_code)
    address = 0

    if op_id == 27 or op_id == 28:
        patches[inst_[1]] = len(instructions)
    elif op_id == 29:
        address = 0
    elif op_id == 30:  ##interrupt
        address = retrive_Immediate(inst_[1])
        # print "interrupt id: ", address
    else:
        address = resolve(inst_[1])
    machine_code = (machine_code << 26) | (address & 0x03ffffff)
    # print("machine code: %x " % machine_code)
    return machine_code

def patch_addresses():
    global instructions, patches, symbols
    for label_name in patches.keys():
        # print patches.get(label_name)
        if symbols.has_key(label_name):
            offset = (symbols.get(label_name) - (patches.get(label_name) + 1))
            pat = (parse_int_to_16b_signed(offset) & 0x0000ffff)
            instructions[patches.get(label_name)] |= pat
        else:
            print "[-]ERROR::No symbol ", label_name
            exit()

    # Patch Data references

def write_to_file(ofile_handle, instructions, Data):
    # ##     Header structure
    # ----Text-size
    # ----Text_start_offset
    # ----entry_point
    # ----Data_start
    # ##

    # write the file header
    header = [len(instructions),        # text size
              4,                        # Text section start
              symbols.get(Entry, 0),    # entry point
              4 + len(instructions)     # data segment
              ]
    print "Header: ", header
    ofile_handle.write(struct.pack(str(len(header)) + 'i', *header))

    # write the text section
    ofile_handle.write(struct.pack(str(len(instructions)) + 'i', *instructions))

    # write the Data segment

    for item in Data.items():
        d = item[1]
        print item
        ofile_handle.write(struct.pack(str(len(d)) + 'i', *d))

def is_data_decl(line):
    global Entry
    # print "Checking"
    if line.find(".string") >= 0:
        li = lex_decl(line)
        byte_seq = string_to_byte_stream(li[1])
        Data[li[0]] = byte_seq
        print "Data decl: ", li[0], " => ", li[1]
        # print Data
        return True
    elif line.find(".entry") >= 0:
        Entry = lex_entry(line)[0]
        print "Entry point: ", Entry
        return True
    else:
        return False

def main():
    global instructions
    file_handle = open(arguments.inputfile, "rw")
    ofile_handle = open(arguments.output, "wb+")

    for line in file_handle.readlines():
        machine_code = 0x00
        line = line.strip("\n")
        line = line.strip()

        if line == "":
            continue

        #commnets start with #
        if extract_comment(line):
            continue

        if is_data_decl(line):
            continue

        #Extract the label
        line = extract_label(line)

        if line == "":
            continue

        lex(line)

        op_id = get_opcode(inst_[0].upper())
        if op_id < 0 or op_id >= 31:
            # print "[ERROR]::Uknown operand " + inst_[0]
            file_handle.close()
        elif op_id <= 13:
            # print "R-type operation..."
            machine_code = R_Encode(op_id)
        elif op_id <= 26:
            # print "I-type operation..."
            machine_code = I_Encode(op_id)
        elif op_id <= 30:
            # print "J-type operation...."
            machine_code = J_Encode(op_id)
        else:
            continue

        instructions.append(machine_code)

#    #patch addresses and symbols
    patch_addresses()

    write_to_file(ofile_handle, instructions, Data)

    # print instructions
#    #clean up
    file_handle.close()
    ofile_handle.close()

    print "Done!!"

if __name__ == "__main__":
    main()