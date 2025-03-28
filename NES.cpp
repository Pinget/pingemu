#include "APU.h"
#include "CPU.h"
// #include "Mapper.h"
#include "PPU.h"
#include <iostream>

//#define DEBUG 1

CPU *cpu;
PPU *ppu;
APU *apu;

int main(int argc, char* argv[])
{
    std::cout << "Hello from Pingemu" << std::endl;
    FILE* file;
    fopen_s(&file, argv[1] /*"lda_test.bin"*/, "r");
    if (!file) 
    {
        std::cout << "Invalid file pointer. Quitting"<< std::endl;
        return 0;
    } else printf("Opened file %s.", argv[1]); 
    
    //length instead of EOF because some instructions detect eof before 1-byte instrs are run.
    fseek(file, 0, SEEK_END);  
    long filength = ftell(file);
    rewind(file);
    //genius

    cpu = new CPU();
    ppu = new PPU();
    apu = new APU();

    while (cpu->get_PC() < filength) 
    {
        cpu->execute(cpu->fetch(file));
        #ifdef DEBUG
        printf("Memory at 0:%u\n", cpu->get_memory()[0]);
        #endif
    } 
}

