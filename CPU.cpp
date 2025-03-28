#include "CPU.h"

CPU::CPU()
{
    reg_accu = 0u;
    status = 0u;
    PC = 0u;
    printf("CPU instanced");
    print_registers();
    //memory[111] = 69u;
}

instruction CPU::fetch(FILE* data)
{
    byte bytes[3];
    fseek(data, PC, SEEK_SET);
    size_t success = fread(bytes,1,3,data);
    #ifdef DEBUG
    printf("bytes:%x%x%x\n", bytes[0], bytes[1], bytes[2]);
    #endif
    for (size_t i = success; i < 3; i++) bytes[i] = 0x0;
    operand_type operand = (bytes[1] << 8) | bytes[2];
    return instruction{bytes[0], operand};
}

void CPU::set_flag(flag _flag, bool value)
{
    byte code = (0b1 << _flag);
    if (value) status |= code;
    else status &= ~code;
}

bool CPU::check_flag(flag _flag)
{
    return ( (status & _flag) == _flag);
}

bool CPU::is_accu_neg()
{
    return (reg_accu & 0b10000000) == 0b10000000;
}

mode_type CPU::get_mode(byte cc, opcode_type aaabbbcc)
{

    //opcode: aaab bbcc
    uint8_t addr_mode = (aaabbbcc & 0b00011100) >> 2; //addr_mode = bbb
        //not sure if returning immediate as default is a good idea but it shouldnt cause any harm unless i fuck smth up so we'll see
    switch (cc)
    {
        case 0b01:
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
        case 0b10:
            switch (addr_mode)
            {
                case 0: return immediate;
                case 1: return zero_page;
                case 2: return implied;
                case 3: return absolute;
                case 4: return zero_page_x;
                case 5: return absolute_x;
                default: return immediate;
            }
        default: return immediate;
    }
    

}
//this is used for most arithmetic operations; but some operations might still need the original operand and not the parsed one.
uint16_t CPU::get_operand_value(mode_type mode, operand_type operand)
{
    switch (mode) //probably not meant to access memory like that so ummm ummm umm eventually use mappers i guess?
    {
    case absolute: return memory[operand];
    case absolute_x: return memory[operand +reg_X];
    case absolute_y: return memory[operand +reg_Y];
    case zero_page: return memory[operand >> 8];
    case zero_page_x: return memory[(operand >> 8) + reg_X];
    case zero_page_y: return memory[(operand >> 8) + reg_Y];
    default:
    case immediate: return operand >> 8;
    case implied: return reg_accu; //probably not necessary
    }
}

//calculate the SP offset based on mode and then step it
void CPU::step_PC(mode_type mode)
{
    uint8_t offset = 0;
    switch (mode)
    {
        case absolute:      offset = 3; break;
        case absolute_x:    offset = 3; break;              
        case absolute_y:    offset = 3; break;                   
        case zero_page:     offset = 2; break;              
        case zero_page_x:   offset = 2; break;              
        case zero_page_y:   offset = 2; break;              
        case immediate:     offset = 2; break;
        default:            offset = 1; break;
    }
    PC += offset;
}

void CPU::print_registers()
{
    printf("A:%u, PC:%u, SP:%u, Status:%u X:%u, Y:%u\n", reg_accu, PC, SP, status, reg_X, reg_Y);
}

byte* CPU::get_value_pointer(mode_type mode, operand_type op)
{
    byte* val_p = nullptr; 
        switch (mode)
        {
        case implied:
            val_p = &reg_accu;
            break;
        case zero_page:
            val_p = memory + ((op) >> 8);
            break;
        case zero_page_x:
            val_p = memory + ((op >> 8) + reg_X);
            break;
        case zero_page_y:
            val_p = memory + ((op >> 8) + reg_Y);
            break;
        default:
            //throw("unknown mode");
            break; 
        }
    return val_p;
}



void CPU::execute(instruction instr)
{
    #ifdef DEBUG_BEFORE
    printf("before execution:\n");
    print_registers();
    #endif

    opcode_type opcode = instr.opcode;
    uint8_t cc = opcode & 0b00000011;
    uint8_t aaa = opcode >> 5;
    //remember to increase PC by used bytes
    mode_type mode = get_mode(cc, opcode);
    //uint16_t operand = parse_operand(mode, instr.operand);
    #ifdef DEBUG
    printf("mode: %x\n", mode);
    printf("opcode_aaa:%x, %u\n", aaa, aaa);
    printf("opcode:%x, %u\n", opcode, opcode);
    #endif
    //printf("opgroup_cc:%x, %u\n", cc, cc);
    //printf("addressing mode: %u\n", mode);

    switch (cc)
    {
        case 0b01: 
            run_cc_01(mode, aaa, instr.operand);
            break;
        case 0b10:
            run_cc_10(mode,aaa, instr.operand);
            break;
        default:break;
    }

    step_PC(mode);

    //will have to change this for other funcs
    #ifdef DEBUG
    printf("after execution:\n");
    #endif
    print_registers();
}

void CPU::run_cc_10(mode_type mode, uint8_t operation, uint16_t operand)
{
    switch(operation)
    {
        case 0:
            ASL(mode, operand);
            break;
        case 1:
            ROL(mode, operand);
            break;
        case 2:
            LSR(mode, operand);
            break;
        case 3:
            ROR(mode, operand);
            break;
        case 4:
            STX(mode, operand);
            break;
        case 5:
            LDX(mode, operand);
            break;
        case 6:
            DEC(mode, operand);
            break;
        case 7:
            INC(mode, operand);
            break; 
    } 
}

void CPU::run_cc_01(mode_type mode, uint8_t operation, uint16_t operand)
{
    switch (operation)
    {
    case 0:
        ORA(mode, operand);
        break;
    case 1:
        AND(mode, operand);
        break;
    case 2:
        EOR(mode, operand);
        break;
    case 3:
        ADC(mode, operand);
        break;
    case 4:
        STA(mode, operand);
        break;
    case 5:
        LDA(mode, operand);
        break;
    case 6:
        CMP(mode, operand);
        break;
    case 7:
        SBC(mode, operand);
        break;
    default:
        break;
    }
}