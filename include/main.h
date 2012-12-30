#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define TWO_COMPLEMENT(N) ~N + 1

#define MASK(N) 1 << N

#define NEGATIVE(SR) !!(SR & MASK(7))
#define OVERFLOW(SR) !!(SR & MASK(6))
#define BREAK(SR) !!(SR & MASK(4))
#define DECIMAL(SR) !!(SR & MASK(3))
#define INTERRUPT(SR) !!(SR & MASK(2))
#define ZERO(SR) !!(SR & MASK(1))
#define CARRY(SR) !!(SR & MASK(0))

#define SET_NEGATIVE(SR) SR | MASK(7)
#define SET_OVERFLOW(SR) SR | MASK(6)
#define SET_BREAK(SR) SR | MASK(4)
#define SET_DECIMAL(SR) SR | MASK(3)
#define SET_INTERRUPT(SR) SR | MASK(2)
#define SET_ZERO(SR) SR | MASK(1)
#define SET_CARRY(SR) SR | MASK(0)

#define ZERO_NEGATIVE(SR) SR & ~MASK(7)
#define ZERO_OVERFLOW(SR) SR & ~MASK(6)
#define ZERO_BREAK(SR) SR & ~MASK(4)
#define ZERO_DECIMAL(SR) SR & ~MASK(3)
#define ZERO_INTERRUPT(SR) SR & ~MASK(2)
#define ZERO_ZERO(SR) SR & ~MASK(1)
#define ZERO_CARRY(SR) SR & ~MASK(0)

typedef struct {
  unsigned char *mem;
  unsigned char *zero; // buf + 0x00FF
  unsigned char *stack; // buf + 0x01FF
  unsigned char *code;

  /* Registers */
  unsigned char A; // Accumulator
  unsigned char Y; 
  unsigned char X; // Index register
  unsigned char SR; // Status register
  unsigned short int PC; // Program counter
  unsigned short int S; // Stack pointer
} cpu_state_t;

typedef bool (*opcode_f)(cpu_state_t*);
opcode_f opcodes[0xFF];

/* General */
cpu_state_t *init_cpu();
void status(cpu_state_t*);
void process_instructions(cpu_state_t*, char*, int);
unsigned char getmem(cpu_state_t *, unsigned short);
unsigned char getmem_indirect(cpu_state_t *, unsigned short, unsigned char);
unsigned char setmem_indirect(cpu_state_t *, unsigned short, unsigned char, unsigned char);

/* Opcodes */
bool adc_immidiate(cpu_state_t *);
bool clc(cpu_state_t *);
bool sta_zeropage(cpu_state_t *);
bool sta_absolute(cpu_state_t *);
bool inx(cpu_state_t *);
bool iny(cpu_state_t *);
bool dex(cpu_state_t *);
bool dey(cpu_state_t *);
bool sta_indirect(cpu_state_t *);
