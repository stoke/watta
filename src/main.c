#include "main.h"

int main(int argc, char **argv) {
  cpu_state_t *cpu = init_cpu();
  
  process_instructions(cpu, "\x69\xFF", 2);
  assert(cpu->A == 255 && !CARRY(cpu->SR));
  process_instructions(cpu, "\x69\xFF", 2);
  assert(cpu->A == 254 && CARRY(cpu->SR));
  process_instructions(cpu, "\x18", 1);
  assert(!CARRY(cpu->SR));
  process_instructions(cpu, "\x85\x0a", 2);
  assert(
    getmem(cpu, 0x0a) == 0xfe
  );

  status(cpu);

  return 0;
}
