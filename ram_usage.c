#include "ram_usage.h"

void printmemsize(char *str, unsigned long ramsize) 
{
        printf("%s: %ld in MB\n",str, (ramsize/1024)/1024);
}

void ram_print_free()
{
    struct sysinfo info;
    sysinfo(&info);
    printmemsize("Freeram", info.freeram);
}