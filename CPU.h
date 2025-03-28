#include <stdint.h>
#include <stdio.h>
#include <iostream>

//#define DEBUG 0 

typedef uint8_t byte;
typedef uint16_t addr;
typedef uint16_t operand_type;
typedef uint8_t opcode_type;

#define OP_DEC(func_name) \
  void func_name(mode_type mode, operand_type op)
#define OP(func_name) \
  void CPU::func_name(mode_type mode, operand_type op)

#define SET_ZN() \
  if (accumulator == 0) set_flag(zero); \
  if ( is_accu_neg() ) set_flag(negative);

struct instruction
{
  opcode_type opcode;
  operand_type operand;
  instruction(opcode_type opc, operand_type opr)
  {
    opcode = opc;
    operand = opr;
  }
};

enum flag
{
  carry,
  zero,
  interr_disable,
  decimal,
  nil1,
  nil2,
  overflow,
  negative
};

enum mode_type
{
  immediate,
  zero_page,
  zero_page_x,
  zero_page_y,
  absolute,
  absolute_y,
  absolute_x,
  accumul
  // indirect_x,
  // indirect_y
};

class CPU
{
private:
  // registers
  byte accumulator; // accumulator, for arithmetic and data transfer
  byte status;
  uint16_t PC = 0;
  byte SP = 0; //very much wrong
  byte reg_X = 0; // indexing memory and loop counters
  byte reg_Y = 0; // indexing memory and loop counters

  byte memory[64000] = {0};

  mode_type get_mode(byte opgroup, opcode_type code);
  uint16_t get_operand_value(mode_type mode, operand_type operand);

  // operators

  bool check_flag(flag _flag);
  void set_flag(flag _flag);
  void unset_flag(flag _flag);
  bool is_accu_neg();
  void step_PC(mode_type mode);
  void print_registers();
  void run_cc_01(mode_type mode, uint8_t operation, uint16_t operand);
  void run_cc_10(mode_type mode, uint8_t operation, uint16_t operand);

  // we store this so the ops can use it later without too much spaghettying

  OP_DEC(ORA); // bitwise or, accu.
  OP_DEC(AND);
  OP_DEC(EOR);
  OP_DEC(ADC);
  OP_DEC(STA);
  OP_DEC(LDA);
  OP_DEC(CMP);
  OP_DEC(SBC);


  OP_DEC(ASL);
  OP_DEC(ROL);
  OP_DEC(LSR);
  OP_DEC(ROR);
  OP_DEC(STX);
  OP_DEC(LDX);
  OP_DEC(DEC);
  OP_DEC(INC);

public:
  CPU();
  instruction fetch(FILE *data);
  void execute(instruction instr);
  /* Instruction cycle
  Fetch
  Decode
  Execute
  Wait
  */
};
