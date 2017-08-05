//
// Created by x3r0 on 8/1/17.
//

#include "common-utils.h"

void print_usage(char *prog, char *args)
{
    printf("Usage: %s %s\n", prog, args);
    exit(1);
}
void *checked_malloc(int size)
{
    if(size <= 0 )
    {
        printf("[-]ERROR::Invalid request to allocate %d bytes of memory\n", size);
        exit(1);
    }
    void *mem = (void *) malloc(size);
    if(mem == NULL)
    {
        printf("[-]ERROR::Opps.. something just went wrong....Failed to allocate memory.\n");
        exit(1);
    }
    return mem;
}