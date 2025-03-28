#include "APU.h"
#include "CPU.h"
// #include "Mapper.h"
#include "PPU.h"
#include <iostream>

CPU *cpu;
PPU *ppu;
APU *apu;

int main(int argc, char* argv[])
{
    std::cout << "Hello from Pingemu" << std::endl;
    FILE* file;
    fopen_s(&file, argv[1], "r");
    if (!file) 
    {
        std::cout << "Invalid file pointer. Quitting"<< std::endl;
        return 0;
    } else std::cout << "Opened file." << std::endl; 

    cpu = new CPU();
    ppu = new PPU();
    apu = new APU();

    while (!feof (file)) 
    {
        cpu->execute(cpu->fetch(file));
    }
    
    
}

