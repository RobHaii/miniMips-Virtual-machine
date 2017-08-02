//
// Created by x3r0 on 8/1/17.
//

#include <stdio.h>
#include "../common-lib/common-utils.h"
#include "vm.h"


int main(int argc, char *argv[])
{
//    if(argc < 2)
//        print_usage(argv[0], "program-file");

    ///ini the machine
    init_vm();
///    load_program()

    ///run
    ///dont forget to always deallocate the memory used by the machine
    delete_vm();
    return 0;
}