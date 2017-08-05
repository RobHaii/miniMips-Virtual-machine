//
// Created by x3r0 on 8/1/17.
//

#include <stdio.h>
#include "../common-lib/common-utils.h"
#include "vm.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{


    /// parse the argumets to recieve program filename
    if(argc < 2)
        print_usage(argv[0], "program-file");

    ///init the machine
    init_vm();

    ///run
    run(argv[1]);

    ///dont forget to always deallocate the memory used by the machine
    delete_vm();
    return 0;
}