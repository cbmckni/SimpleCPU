/***********************************************
Name: Cole McKnight, Luke Morrow
File: sim.c
Description: simulates a basic CPU.
Date: 3/7/17
***********************************************/
 
#define ACC_IN 0
#define ACC_OUT 1
#define ALU_ADD 2
#define ALU_SUB 3
#define IR_IN 4
#define IR_OUT 5
#define MAR_IN 6
#define MDR_IN 7
#define MDR_OUT 8
#define PC_IN 9
#define PC_OUT 10
#define PC_INCR 11
#define READ 12
#define TMP_OUT 13
#define WRITE 14
#define BRTABLE 15
#define NEXT 16
#define OR_ADDR 17
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


int main() 
{
    
    //main memory
    int mem[512][12];
    //registers
    unsigned int PC;      //couldnt get bit fields to work for some reason
    unsigned int MAR;
    unsigned int IR;
    unsigned int MDR;
    unsigned int ACC;
    unsigned int TMP;
    //control store
    int CS[32][18];         
    unsigned int CSAR;
    int CSIR[22]; 
    //data bus?
    unsigned int BUS = 0;

    
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
