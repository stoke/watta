#include "main.h"

int main(int argc, char **argv) {
  cpu_state_t *cpu = init_cpu();
  
  process_instructions(cpu, "\x69\xFF", 2);
  assert(cpu->A == 255 && !CARRY(cpu->SR));
  
  process_instructions(cpu, "\x69\xFF", 2);
  assert(cpu->A == 254 && CARRY(cpu->SR));
  
  process_instructions(cpu, "\x18", 1);
  assert(!CARRY(cpu->SR));

  process_instructions(cpu, "\x85\x0A", 2);
  assert(
    getmem(cpu, 0x0a) == cpu->A
  );

  process_instructions(cpu, "\x8D\x00\x20", 3);
  assert(
    getmem(cpu, 0x2000) == cpu->A
  );

  process_instructions(cpu, "\xE8\xC8", 2);
  assert(cpu->X && cpu->Y);

  process_instructions(cpu, "\x88\xCA", 2);
  assert(!cpu->X && !cpu->Y && ZERO(cpu->SR));
  
  process_instructions(cpu, "\x88\xCA", 2);
  assert((char) cpu->X < 0 && (char) cpu->Y < 0 && NEGATIVE(cpu->SR));

  process_instructions(cpu, "\xE8\xE8\x95\x0A", 4);
  assert(cpu->X == 1 && getmem(cpu, 0x0B) == cpu->A);
  
  process_instructions(cpu, "\xC8\xC8\x9D\x00\x20\x99\x00\x21", 8);
  assert(cpu->Y == 1 && getmem(cpu, 0x2001) == cpu->A && getmem(cpu, 0x2101) == cpu->A);

  cpu->A = 0xC0;
  cpu->mem[0x9000] = 0x00;
  cpu->mem[0x9001] = 0x20;
  process_instructions(cpu, "\x81\xFF\x8F", 3);
  assert(getmem(cpu, 0x2000) == 0xC0);
  
  cpu->A = 0xFA;
  cpu->mem[0x9000] = 0x00;
  cpu->mem[0x9001] = 0x1F;
  process_instructions(cpu, "\x91\x00\x90", 3);
  assert(getmem(cpu, 0x1F01) == 0xFA);

  status(cpu);

  return 0;
}
