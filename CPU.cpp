#include "CPU.h"

#define SET_ZN() \
    if (accumulator == 0) set_flag(zero); \
    if (accumulator & negative) set_flag(negative);

CPU::CPU()
{
    accumulator = 0;
    status = 0;
    PC = 0;
}

instruction CPU::fetch(FILE* data)
{
    byte bytes[3];
    fseek(data, PC, SEEK_SET);
    fread(bytes,1,3,data);
    return {bytes[0], (bytes[1] << 4) | bytes[2] };
}

void CPU::set_flag(flag flag)
{
    status &= (0b1 << flag);
}

mode_type CPU::get_mode(opcode_type opcode)
{

    //opcode: aaab bbcc
    uint8_t addr_mode = (opcode << 3) >> 5;
    switch (addr_mode)
    {
        case 0: return zero_page_x;
        case 1: return zero_page;
        case 2: return immediate;
        case 3: return absolute;
        case 4: return zero_page_y;
        case 5: return zero_page_x;
        case 6: return absolute_y;
        case 7: return absolute_x;
        default: return immediate;
    }

}

uint16_t CPU::parse_operand(mode_type mode, operand_type operand)
{
    switch (mode) //probably not meant to access memory like that so ummm ummm umm eventually use mappers i guess?
    {
    case absolute: return memory[operand];
    case absolute_x: return memory[operand +reg_X];
    case absolute_y: return memory[operand +reg_Y];
    case zero_page: return memory[operand >> 4];
    case zero_page_x: return memory[(operand >> 4) + reg_X];
    case zero_page_y: return memory[(operand >> 4) + reg_Y];
    case immediate: return operand;
    default: immediate;
    }
}


//u dum fuck you now have to calculate how much the program counter is increased
OP(ORA)
{
    accumulator = accumulator | op;
    SET_ZN();
}

OP(AND)
{
    accumulator = accumulator & op;
    SET_ZN();
}

OP(EOR)
{
    accumulator = accumulator ^ op;
    SET_ZN();
}

OP(ADC)
{
    //todo this will most definitely fail
    uint16_t acc_extended = accumulator + op;
    accumulator += op;
    if (acc_extended > UINT8_MAX) set_flag(carry);
    SET_ZN();
}

OP(STA) { memory[op] = accumulator; }

OP(LDA) 
{
    accumulator = op;
    SET_ZN();
}

OP(CMP)
{
    if (accumulator == op) set_flag(zero);
    if (accumulator >= op) set_flag(carry);
    if (accumulator & negative) set_flag(negative);
}

OP(SBC)
{
    //this might fucking fail.:w

    // 'clear if overflow in bit 7'
    uint8_t before = accumulator;
    accumulator = accumulator - op - (1 - (status & 1));
    if (accumulator > before) status &= 0b11111110;
    SET_ZN();
    //????? overflow flag?? urm idk wtf im doing 
}



void CPU::execute(instruction instr)
{
    //remember to increase PC by used bytes
    opcode_type opcode = instr.opcode;
    mode_type mode = get_mode(opcode);
    uint16_t operand = parse_operand(mode, instr.operand);

    uint8_t operation = opcode >> 5;
    switch (operation)
    {
        case 0: ORA(operand);
        case 1: AND(operand);
        case 2: EOR(operand); 
        case 3: ADC(operand); 
        case 4: STA(operand); 
        case 5: LDA(operand); 
        case 6: CMP(operand); 
        case 7: SBC(operand); 
    }
}