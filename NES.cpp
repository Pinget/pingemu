#include "APU.h"
#include "CPU.h"
// #include "Mapper.h"
#include "PPU.h"

CPU *cpu;
PPU *ppu;
APU *apu;

int main(char** args)
{
    FILE* file = fopen(args[1], "r");

    cpu = new CPU();
    ppu = new PPU();
    apu = new APU();

    while (true) 
    {
        cpu->execute(cpu->fetch(file));
    }
    
    
}

