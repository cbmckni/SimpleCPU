/***********************************************
Name: Cole McKnight, Luke Morrow
File: sim.c
Description: simulates a basic CPU.
Date: 3/7/17
***********************************************/
 
#include <stdio.h>

/* base inst set */

  /* ifetch entry */
 // cs[ 0][ PC_OUT] = 1; cs[ 0][ MAR_IN] = 1; cs[ 0][NEXT] =  2;
    /* special taken branch entry */
 // cs[ 1][ IR_OUT] = 1; cs[ 1][  PC_IN] = 1; cs[ 1][NEXT] =  0;
    /* rest of ifetch */
 // cs[ 2][PC_INCR] = 1; cs[ 2][   READ] = 1; cs[ 2][NEXT] =  3;
 // cs[ 3][MDR_OUT] = 1; cs[ 3][  IR_IN] = 1; cs[ 3][NEXT] =  4;
 // cs[ 4][BRTABLE] = 1;                      cs[ 4][NEXT] =  0;
struct CSIR{
  unsigned int ACC_IN:1=0; //bit  0 == ACC_in
  unsigned int ACC_OUT:1=0; //bit  1 == ACC_out
  unsigned int ALU_ADD:1=0; //bit  2 == alu_add
  unsigned int ALU_SUB:1=0; //bit  3 == alu_sub
  unsigned int IR_IN:1=0; //bit  4 == IR_in
  unsigned int IR_OUT:1=0; //bit  5 == IR_out
  unsigned int MAR_IN:1=0; //bit  6 == MAR_in
  unsigned int MDR_IN:1=0; //bit  7 == MDR_in
  unsigned int MDR_OUT:1=0; //bit  8 == MDR_out
  unsigned int PC_IN:1=0; //bit  9 == PC_in
  unsigned int PC_OUT:1=0; //bit 10 == PC_out
  unsigned int PC_INCR:1=0; //bit 11 == pc_incr
  unsigned int READ:1=0; //bit 12 == read
  unsigned int TMP_OUT:1=0; //bit 13 == TMP_out
  unsigned int WRITE:1=0; //bit 14 == write
  unsigned int BRTABLE:1=0; //bit 15 == br_table
};

struct CSAR {
  unsigned int CSAR:5=0; //bits 16-20 == 5-bit next control store address field
;

struct OR{
  unsigned int OR_ADDR:1=0; //bit 21 == or_addr
};

struct ControlStoreEntry{
  struct CSIR controlSignals; //first 16 bits of the control store
  struct CSAR nextAddr; //5 bits for the next address
  struct OR orAddr; //1 bit for the or_addr
};

struct ControlStore{
  struct ControlStoreEntry instructionFetch[5]; //5 time cycles for IF
  struct ControlStoreEntry load[3]: //3 time cycles for load instruction
  struct ControlStoreEntry add[4]; //4 time cycles for add instruction
  struct ControlStoreEntry store[3]; //3 time cycles for store instructions
  struct ControlStoreEntry brz[1]; //1 time cycle for branch zero
  struct ControlStoreEntry sub[4]; //4 time cycles for sub instruction
  struct ControlStoreEntry jsub[4]; //4 time cycles for jsub instruction
  struct ControlStoreEntry jmpi[4]; //4 time cycles for jmpi instruction
  struct ControlStoreEntry halt[1]; //1 time cycle for halt
};

int main() 
{
    
    //main memory
    int mem[512][12]; //Main Memory 512 x 12-bit memory
    //registers
    unsigned int PC:9;      //Program Counter 9 bits
    unsigned int MAR:9;     //Memory Address Register 9 bits
    unsigned int IR:12;      //Instruction Register 12 bits
    unsigned int MDR:12;     //Memory Data Register 12 bits
    unsigned int ACC:12;     //Accumulator 12 bits
    unsigned int TMP:12;     //Temp 12 bits
    struct CSIR mCSIR;       //Control Store instruction register 22-bits
    struct CSAR mCSAR;    //Control Store address register 5-bits
    struct ControlStore mControlStore; //Control Store 32 x 22-bit           
    //data bus?
    //unsigned int BUS = 0;

    
    unsigned int word, opcode, addr;
    if(scanf("%x", &word) != -1) 
    {
        PC++;
        while(word != -1)
        {
            PC++;                          //incr pc 
            opcode = word >> 9;            //calc opcode

            unsigned int mask = 0x1FF;     
            addr = word & mask;            //calc addr

            printf("\n %x = %d -> %d", word, opcode, addr); 

            scanf("%x", &word);            //read next inst
        }
        printf("\n HALT\nInstructions:  %d \n", PC);
    }
   
    /*
    while(CSIR != -1)
    {
        printf("Enter 22 bit CSIR(-1 to end):  ");
        scanf("%x", &CSIR);
        if(CSIR[ACC_IN]) ACC = BUS;
        if(CSIR[ACC_OUT]) BUS = ACC;
        //if(CSIR[ALU_ADD]) aluadd;
        //if(CSIR[ALU_SUB]) alusub;
        if(CSIR[IR_IN]) IR = BUS;
        if(CSIR[IR_OUT]) BUS = IR; //supposed to be address portion of IR, not full value
        if(CSIR[MAR_IN]) MAR = BUS;
        if(CSIR[MDR_IN]) MDR = BUS;
        if(CSIR[MDR_OUT]) BUS = MDR;
        if(CSIR[PC_IN]) PC = BUS;
        if(CSIR[PC_OUT]) BUS = PC;
        if(CSIR[PC_INCR]) PC++;
        //if(CSIR[READ]) MDR = mem[MAR];
        if(CSIR[TMP_OUT]) BUS = TMP;
        //if(CSIR[WRITE]) mem[MAR] = MDR;
        //if(CSIR[BRTABLE]) brtable use decoder;
        //if(CSIR[NEXT]) next inst;
        //if(CSIR[OR_ADDR]) override;
    }    
    */
    return 0;
}
/*
  bit  0 == ACC_in
  bit  1 == ACC_out
  bit  2 == alu_add
  bit  3 == alu_sub
  bit  4 == IR_in
  bit  5 == IR_out
  bit  6 == MAR_in
  bit  7 == MDR_in
  bit  8 == MDR_out
  bit  9 == PC_in
  bit 10 == PC_out
  bit 11 == pc_incr
  bit 12 == read
  bit 13 == TMP_out
  bit 14 == write
  bit 15 == br_table
  bits 16-20 == 5-bit next control store address field
  bit 21 == or_addr
*/
/*psuedocode
while(!halt)
{
    CSIR = CS[CSAR];
    
    if(CSIR[PC_INCR])
        PC++;
    if(CSIR[MAR_IN])
        MAR = BUS;
    if(CSIR[PC_OUT])
        BUS = PC;
}

*/
