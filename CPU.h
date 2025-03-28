#include <stdint.h>
#include <stdio.h>
#include <iostream>

//#define DEBUG 0 

typedef uint8_t byte;
typedef uint16_t addr;
typedef uint16_t operand_type;
typedef uint8_t opcode_type;

#define OP_DEC(func_name) \
  void func_name(operand_type op)

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
  absolute_x
  // indirect_x,
  // indirect_y
};

class CPU
{
private:
  // registers
  byte accumulator; // accumulator, for arithmetic and data transfer
  byte status;
  uint16_t PC;
  byte SP;
  byte reg_X; // indexing memory and loop counters
  byte reg_Y; // indexing memory and loop counters

  byte memory[0x64000];

  mode_type get_mode(opcode_type code);
  uint16_t parse_operand(mode_type mode, operand_type operand);

  // operators

  bool check_flag(flag _flag);
  bool is_accu_neg();
  void set_flag(flag _flag);
  void step_PC_arithm(mode_type mode);
  void print_registers();

  // we store this so the ops can use it later without too much spaghettying

  OP_DEC(ORA); // bitwise or, accu.
  OP_DEC(AND);
  OP_DEC(EOR);
  OP_DEC(ADC);
  OP_DEC(STA);
  OP_DEC(LDA);
  OP_DEC(CMP);
  OP_DEC(SBC);

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
