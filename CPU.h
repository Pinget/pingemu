#include <stdint.h>
#include <stdio.h>

typedef uint8_t byte;
typedef uint16_t addr;
typedef uint16_t operand_type;
typedef uint8_t opcode_type;

#define OP(func_name) \
  void CPU::func_name(operand_type op)

#define OP_DEC(func_name) \
  void func_name(operand_type op)

struct instruction
{
  opcode_type opcode;
  operand_type operand;
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

  void set_flag(flag _flag);

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
