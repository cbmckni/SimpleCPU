/***********************************************
Name: Cole McKnight, Luke Morrow
File: sim.h
Description: simulates a basic CPU.
Date: 3/7/17
***********************************************/
#ifndef SIM_H
#define SIM_H 
#include <stdio.h>

struct CSIR { 
  unsigned int ACC_IN:1; //bit  0 == ACC_in
  unsigned int ACC_OUT:1; //bit  1 == ACC_out
  unsigned int ALU_ADD:1; //bit  2 == alu_add
  unsigned int ALU_SUB:1; //bit  3 == alu_sub
  unsigned int IR_IN:1; //bit  4 == IR_in
  unsigned int IR_OUT:1; //bit  5 == IR_out
  unsigned int MAR_IN:1; //bit  6 == MAR_in
  unsigned int MDR_IN:1; //bit  7 == MDR_in
  unsigned int MDR_OUT:1; //bit  8 == MDR_out
  unsigned int PC_IN:1; //bit  9 == PC_in
  unsigned int PC_OUT:1; //bit 10 == PC_out
  unsigned int PC_INCR:1; //bit 11 == pc_incr
  unsigned int READ:1; //bit 12 == read
  unsigned int TMP_OUT:1; //bit 13 == TMP_out
  unsigned int WRITE:1; //bit 14 == write
  unsigned int BRTABLE:1; //bit 15 == br_table
} CSIR_default = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct CSAR {
  unsigned int csar:5; //bits 16-20 == 5-bit next control store address field
};

struct OR_ADDR{
  unsigned int or_addr:1; //bit 21 == or_addr
};

struct ControlStoreEntry{
  struct CSIR controlSignals; //first 16 bits of the control store
  struct CSAR nextAddr; //5 bits for the next address
  struct OR_ADDR orAddr; //1 bit for the or_addr
};

struct ControlStore{
  struct ControlStoreEntry instructionFetch[5]; //5 time cycles for IF
  struct ControlStoreEntry load[3]; //3 time cycles for load instruction
  struct ControlStoreEntry add[4]; //4 time cycles for add instruction
  struct ControlStoreEntry store[3]; //3 time cycles for store instructions
  struct ControlStoreEntry brz[1]; //1 time cycle for branch zero
  struct ControlStoreEntry sub[4]; //4 time cycles for sub instruction
  struct ControlStoreEntry jsub[4]; //4 time cycles for jsub instruction
  struct ControlStoreEntry jmpi[4]; //4 time cycles for jmpi instruction
  struct ControlStoreEntry halt[1]; //1 time cycle for halt
};

struct Registers {
	unsigned int PC:9;      //Program Counter 9 bits
    unsigned int MAR:9;     //Memory Address Register 9 bits
    unsigned int IR:12;      //Instruction Register 12 bits
    unsigned int MDR:12;     //Memory Data Register 12 bits
    unsigned int ACC:12;     //Accumulator 12 bits
    unsigned int TMP:12;     //Temp 12 bits
} Registers_default = {0,0,0,0,0,0};

struct MemoryWord {
	unsigned int opcode:3; //3 bits for opcode, 9 bits for address field
	signed int addr:9;
};

struct MainMemory {
	struct MemoryWord mainMem[512]; //max size of 512
};

struct ControlStore fillControlStore();
void printRegisters(struct Registers temp);
void printCSIR(struct ControlStoreEntry temp);
struct ControlStoreEntry indexCS(struct ControlStore temp, int csar);
int decodeTable(int opcode);
#endif