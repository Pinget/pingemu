#include "CPU.h"
    


    OP(ORA)
    {
        op = get_operand_value(mode, op);
        reg_accu = reg_accu | op;
        SET_ZN();
    }
  
    OP(AND)
    {
        op = get_operand_value(mode, op);
        reg_accu = reg_accu & op;
        SET_ZN();
    }
  
    OP(EOR)
    {
        op = get_operand_value(mode, op);
        reg_accu = reg_accu ^ op;
        SET_ZN();
    }
  
    OP(ADC)
    {
        op = get_operand_value(mode, op);
        //todo this will most definitely fail
        uint16_t acc_extended = reg_accu + op;
        reg_accu += op;
        if (acc_extended > UINT8_MAX) set_flag(carry, true);
        SET_ZN();
    }
  
    OP(STA) { 
        byte* val = get_value_pointer(mode, op);
        *val = reg_accu; }
  
    OP(LDA)  
    {
        op = get_operand_value(mode, op);
        reg_accu = static_cast<byte>(op);
        SET_ZN();
    }
  
    OP(CMP)
    {
        op = get_operand_value(mode, op);
        if (reg_accu == op) set_flag(zero,true);
        if (reg_accu >= op) set_flag(carry,true);
        if ( is_accu_neg() ) set_flag(negative, true);
    }
  
    OP(SBC)
    {
        //this might fucking fail.:w
  
        // 'clear if overflow in bit 7'
        op = get_operand_value(mode, op);
        uint8_t before = reg_accu;
        reg_accu = reg_accu - op - (1 - (status & 1));
        if (reg_accu > before) status &= 0b11111110;
        SET_ZN();
        //????? overflow flag?? urm idk wtf im doing 
    }

    OP(ASL)
    {
        op = get_operand_value(mode, op);
        set_flag(carry, is_accu_neg());
        reg_accu <<= 1;
        if (is_accu_neg()) set_flag(negative, true);
    }
    OP(ROL)
    {

        byte* val_p = get_value_pointer(mode, op); 
        
        bool old_carry = check_flag(carry);

        set_flag(carry, (*val_p & 0b1000'0000));

        *val_p <<= 1;
        if (old_carry) *val_p |= 0b0000'0001;
        //logic

        //reassignment

    }
    OP(LSR)
    {
        byte* val_p = get_value_pointer(mode, op);
        set_flag(carry,(*val_p & 0b0000'0001));
        *val_p >>= 1;



    }
    OP(ROR)
    {

        byte* val_p = get_value_pointer(mode, op); 
        
        bool old_carry = check_flag(carry);

        set_flag (carry, (*val_p & 0b1));

        *val_p >>= 1;
        if (old_carry) *val_p |= 0b1000'0000;
        //logic

        //reassignment
    }
    OP(STX)
    {
        byte* val_p = get_value_pointer(mode, op);
        *val_p = reg_X;
    }
    OP(LDX)
    {
        byte parsed_operand = static_cast<byte>(get_operand_value(mode, op));
        reg_X = parsed_operand;
    }
    OP(DEC)
    {
        byte* val_p = get_value_pointer(mode, op);
        byte newval = *val_p -1;
        *val_p = newval;
        set_flag(zero, newval == 0);
        set_flag(negative, newval & 0b1000'0000);
    }
    OP(INC)
    {
        byte* val_p = get_value_pointer(mode, op);
        byte newval = *val_p +1;
        *val_p = newval;
        set_flag(zero, newval == 0);
        set_flag(negative, newval & 0b1000'0000);

    }

    OP(BIT)
    {

    }
    OP(JMP)
    OP(JMPABS)
    OP(STY)
    OP(LDY)
    OP(CPY)
    OP(CPX)

