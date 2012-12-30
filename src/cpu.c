#include "main.h"

cpu_state_t *init_cpu() {
  int n;
  
  cpu_state_t *cpu = (cpu_state_t *) malloc(sizeof(cpu_state_t));

  cpu->mem = (unsigned char *) malloc(0xFFFF);
  cpu->zero = cpu->mem + 0x00FF;
  cpu->stack = cpu->mem + 0x0100;
  cpu->PC = 0;
  cpu->SR = 0;
  cpu->S = 1 << 8; // first 8 bits are always 00000001
  cpu->X = cpu->Y = cpu->A = 0;
  cpu->code = NULL;
  
  for (n = 0; n < 0xFF; n++)
    opcodes[n] = (opcode_f) n;

  opcodes[0x69] = adc_immidiate;
  opcodes[0x18] = clc;
  opcodes[0x85] = sta_zeropage;
  opcodes[0x95] = sta_zeropage;
  opcodes[0x8D] = sta_absolute;
  opcodes[0x9D] = sta_absolute;
  opcodes[0x99] = sta_absolute;
  opcodes[0xE8] = inx;
  opcodes[0xC8] = iny;
  opcodes[0x88] = dey;
  opcodes[0xCA] = dex;
  

  return cpu;
}

void process_instructions(cpu_state_t *cpu, char *code, int length) {
  cpu->code = (unsigned char *) code;
  cpu->PC = 0;

  while (cpu->PC < length) 
    opcodes[cpu->code[cpu->PC]](cpu);
}

void status(cpu_state_t *cpu) {
  printf("--- DEBUG ---\n"
         "A: 0x%x\n"
         "Y: 0x%x\n"
         "X: 0x%x\n"
         "SR: 0x%x\n"
         "PC: %d\n"
         "S: 0x%x\n"
         "NEGATIVE: %d\n"
         "OVERFLOW: %d\n"
         "ZERO: %d\n"
         "CARRY: %d\n"
         "--- DEBUG ---\n", cpu->A, cpu->Y, cpu->X, cpu->SR, cpu->PC, cpu->S, NEGATIVE(cpu->SR), OVERFLOW(cpu->SR), ZERO(cpu->SR), CARRY(cpu->SR));
}

unsigned char getmem(cpu_state_t *cpu, unsigned short addr) {
  return cpu->mem[addr];
}

unsigned char getmem_indirect(cpu_state_t *cpu, unsigned short addr) {
  unsigned short ind_addr = *((unsigned short *) (cpu->mem + addr));

  return cpu->mem[ind_addr];
}

bool clc(cpu_state_t *cpu) {
  cpu->SR = ZERO_ZERO(cpu->SR);
  cpu->PC++;

  return true;
}

bool inx(cpu_state_t *cpu) {
  cpu->X++;
  cpu->PC++;
  
  if (!cpu->X)
    cpu->SR = SET_ZERO(cpu->SR);

  if (cpu->X < 0)
    cpu->SR = SET_NEGATIVE(cpu->SR);

  return true;
}

bool iny(cpu_state_t *cpu) {
  cpu->Y++;
  cpu->PC++;
  
  if (!cpu->Y)
    cpu->SR = SET_ZERO(cpu->SR);

  if (cpu->Y < 0)
    cpu->SR = SET_NEGATIVE(cpu->SR);

  return true;
}

bool dex(cpu_state_t *cpu) {
  cpu->X--;
  cpu->PC++;

  if (!cpu->X)
    cpu->SR = SET_ZERO(cpu->SR);

  if (cpu->X < 0)
    cpu->SR = SET_NEGATIVE(cpu->SR);

  return true;
}

bool dey(cpu_state_t *cpu) {
  cpu->Y--;
  cpu->PC++;
  
  if (!cpu->Y)
    cpu->SR = SET_ZERO(cpu->SR);

  if (cpu->Y < 0)
    cpu->SR = SET_NEGATIVE(cpu->SR);

  return true;
}

bool adc_immidiate(cpu_state_t *cpu) {
  char arg = cpu->code[cpu->PC + 1];
  int A = cpu->A;
  int res = A + arg;

  if (res > 0xFF / 2 || res < - (0xFF / 2))
    cpu->SR = SET_CARRY(cpu->SR);

  if (res < 0) 
    cpu->SR = SET_NEGATIVE(cpu->SR);

  cpu->A = (char) res;

  cpu->PC += 2;

  return true;
}

bool sta_zeropage(cpu_state_t *cpu) {
  unsigned char arg = cpu->code[cpu->PC + 1];
  unsigned char opcode = cpu->code[cpu->PC];

  if (opcode == 0x95)
    arg += cpu->X;

  cpu->mem[arg] = cpu->A;
  cpu->PC += 2;

  return true;
}

bool sta_absolute(cpu_state_t *cpu) {
  unsigned short arg = *((unsigned short *) (cpu->code + cpu->PC + 1));
  unsigned char opcode = cpu->code[cpu->PC];

  switch(opcode) {
    case 0x9D:
      arg += cpu->X;
      break;

    case 0x99:
      arg += cpu->Y;
      break;
  }
  
  cpu->mem[arg] = cpu->A;
  cpu->PC += 3;

  return true;
}
