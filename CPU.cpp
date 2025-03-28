#include "CPU.h"

#define SET_ZN() \
    if (accumulator == 0) set_flag(zero); \
    if ( is_accu_neg() ) set_flag(negative);

    #define OP(func_name) \
  void CPU::func_name(operand_type op)


CPU::CPU()
{
    accumulator = 0u;
    status = 0u;
    PC = 0u;
    printf("CPU instanced");
    print_registers();
}

instruction CPU::fetch(FILE* data)
{
    byte bytes[3];
    fseek(data, PC, SEEK_SET);
    size_t success = fread(bytes,1,3,data);
    #ifdef DEBUG
    for (int i = 0; i < 3; i++) printf("bytes:%x\n", bytes[i]);
    printf("successfully read %d bytes\n", success);
    #endif
    for (size_t i = success; i < 3; i++) bytes[i] = 0x0;
    operand_type operand = (bytes[1] << 8) | bytes[2];
    return instruction{bytes[0], operand};
}

void CPU::set_flag(flag _flag)
{
    status |= (0b1 << _flag);
}

bool CPU::check_flag(flag _flag)
{
    return ( (status & _flag) == _flag);
}

bool CPU::is_accu_neg()
{
    return (accumulator & 0b10000000) == 0b10000000;
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
    case immediate: return operand >> 8;
    default: return immediate;
    }
}

//calculate the SP offset based on mode and then step it
void CPU::step_PC_arithm(mode_type mode)
{
    uint8_t offset;
    if (mode == absolute || mode == absolute_x || mode == absolute_y) offset = 3;
    else offset = 2;
    PC += offset;
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
    accumulator = static_cast<byte>(op);
    SET_ZN();
}

OP(CMP)
{
    if (accumulator == op) set_flag(zero);
    if (accumulator >= op) set_flag(carry);
    if ( is_accu_neg() ) set_flag(negative);
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

void CPU::print_registers()
{
    printf("A:%u, PC:%u, SP:%u, Status:%u\n", accumulator, PC, SP, status);
}



void CPU::execute(instruction instr)
{
    #ifdef DEBUG
    printf("before execution:\n");
    print_registers();
    #endif
    //remember to increase PC by used bytes
    opcode_type opcode = instr.opcode;
    mode_type mode = get_mode(opcode);
    uint16_t operand = parse_operand(mode, instr.operand);
    #ifdef DEBUG
    printf("operand: %x\n", operand);
    printf("mode: %x\n", mode);
    #endif
    printf("opcode:%x\n", opcode);

    uint8_t operation = opcode >> 5;
    switch (operation)
    {
        case 0: ORA(operand); break;  
        case 1: AND(operand); break; 
        case 2: EOR(operand); break; 
        case 3: ADC(operand); break; 
        case 4: STA(operand); break; 
        case 5: LDA(operand); break; 
        case 6: CMP(operand); break; 
        case 7: SBC(operand); break;
        default: break; 
    }
    //will have to change this for other funcs
    step_PC_arithm(mode);
    #ifdef DEBUG
    printf("after execution:\n");
    #endif
    print_registers();
}