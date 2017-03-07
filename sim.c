/***********************************************
Name: Cole McKnight, Luke Morrow
File: sim.c
Description: simulates a basic CPU.
Date: 3/7/17
***********************************************/
 
#include "sim.h"

int main() 
{
    
    //main memory
    struct MainMemory mem; //Main Memory 512 x 12-bit memory
    //registers
    struct Registers mRegisters= Registers_default;
    struct ControlStoreEntry currentCS; //CSIR, CSAR, and Or_addr
    struct ControlStore mControlStore; //Control Store 32 x 22-bit 
    struct CSAR mCSAR;          
    //data bus?
    //unsigned int BUS = 0;
    //initialize Control Store values
    mControlStore= fillControlStore();

    unsigned int word, opcode, addr;
    int counter=0;
    //scan the input into the main memory
    while(scanf("%x", &word) != EOF && word != -1){   
        opcode = word >> 9;            //calc opcode
        mem.mainMem[counter].opcode= opcode; 
        unsigned int mask = 0x1FF;     
        
        addr = word & mask;            //calc addr
        mem.mainMem[counter].addr=addr;
		counter++;                          //incr pc
    }
    //set halt point
    mem.mainMem[counter].opcode=7;


    int i;
	printf("Op code and address");
    for(i=0; i< counter; i++){
    	printf("\n%d: %d -> %d", i, mem.mainMem[i].opcode, mem.mainMem[i].addr);
    }
   	
   	//print the contents of main memory
	printf("\nlow memory\n");
    for(i=0; i< counter; i++){
    	printf("%x\t", ((mem.mainMem[i].opcode << 9) | mem.mainMem[i].addr));
    	if(i==9)
    		printf("\n");
    }
    printf("\n");
     
    //begin processing the instructions in mainmem
    int cycle=1;
    printf("cycle\tPC\tIR\tMAR\tMDR\tACC\tTMP\tCSAR\t\tCISR\t\tcntl signals\n");
    while(mCSAR.csar < 28) {
    	currentCS=indexCS(mControlStore, mCSAR.csar);
    	printf("%d:\t", cycle); printRegisters(mRegisters); printf("%x\t", mCSAR.csar); printCSIR(currentCS);

    	if(currentCS.controlSignals.ACC_IN==1){
    		if(currentCS.controlSignals.TMP_OUT==1){
    			mRegisters.ACC=mRegisters.TMP;
    		}
    		if(currentCS.controlSignals.MDR_OUT==1){
    			mRegisters.ACC=mRegisters.MDR;
    		}
    	}
		if(currentCS.controlSignals.IR_IN==1){
			if(currentCS.controlSignals.TMP_OUT==1){
    			mRegisters.IR=mRegisters.TMP;
    		}
    		if(currentCS.controlSignals.MDR_OUT==1){
    			mRegisters.IR=mRegisters.MDR;
    		}
		}
		if(currentCS.controlSignals.READ==1){
			mRegisters.MDR=((mem.mainMem[mRegisters.MAR].opcode << 9) | mem.mainMem[mRegisters.MAR].addr);
			opcode= mem.mainMem[mRegisters.MAR].opcode;
		}
		if(currentCS.controlSignals.WRITE==1){
			mem.mainMem[mRegisters.MAR].opcode=mRegisters.MDR >> 9;
			mem.mainMem[mRegisters.MAR].addr=mRegisters.MDR & 0x1FF;
		}
		if(currentCS.controlSignals.PC_INCR==1) {
			mRegisters.PC++;
		}
		if(currentCS.controlSignals.MDR_IN==1) {
			if(currentCS.controlSignals.ACC_OUT==1){
    			mRegisters.MDR=mRegisters.ACC;
    		}
    		if(currentCS.controlSignals.PC_OUT==1){
    			mRegisters.MDR=mRegisters.PC;
    		}
		}
		if(currentCS.controlSignals.MAR_IN==1) {
			if(currentCS.controlSignals.IR_OUT==1){
    			mRegisters.MAR=mRegisters.IR & 0x1FF;
    		}
    		if(currentCS.controlSignals.PC_OUT==1){
    			mRegisters.MAR=mRegisters.PC;
    		}

		}
		if(currentCS.controlSignals.PC_IN==1) {
			if(currentCS.controlSignals.IR_OUT==1){
    			mRegisters.PC=mRegisters.IR;
    		}
    		if(currentCS.controlSignals.MDR_OUT==1){
    			mRegisters.PC=mRegisters.MDR;
    		}
		}
		if(currentCS.controlSignals.BRTABLE==1) {
			mCSAR.csar=decodeTable(mem.mainMem[mRegisters.MAR].opcode);
		} else {
			mCSAR=currentCS.nextAddr;
		}
		if(currentCS.controlSignals.ALU_ADD==1) {
			mRegisters.TMP=mRegisters.ACC+mRegisters.MDR;
		}
		if(currentCS.controlSignals.ALU_SUB==1) {
			mRegisters.TMP=mRegisters.ACC-mRegisters.MDR;
		}
		if(currentCS.orAddr.or_addr==1){
			if(mRegisters.ACC==0){
				mCSAR.csar=1;
			}
		}
		cycle++;

    }
    printf("\nlow memory\n");
    for(i=0; i< counter; i++){
    	printf("%x\t", ((mem.mainMem[i].opcode << 9) | mem.mainMem[i].addr));
    	if(i==9)
    		printf("\n");
    }
    printf("\n");
    
    return 0;
}

struct ControlStore fillControlStore(){
	struct ControlStore mControlStore;
	//ifetch 
	mControlStore.instructionFetch[0].controlSignals=CSIR_default;
    mControlStore.instructionFetch[0].controlSignals.MAR_IN=1; mControlStore.instructionFetch[0].controlSignals.PC_OUT=1; 
    mControlStore.instructionFetch[0].nextAddr.csar=2; 
    mControlStore.instructionFetch[1].controlSignals=CSIR_default;
    mControlStore.instructionFetch[1].controlSignals.PC_IN=1; mControlStore.instructionFetch[1].controlSignals.IR_OUT=1; 
    mControlStore.instructionFetch[1].nextAddr.csar=0; 
    mControlStore.instructionFetch[2].controlSignals=CSIR_default;
    mControlStore.instructionFetch[2].controlSignals.PC_INCR=1; mControlStore.instructionFetch[2].controlSignals.READ=1; 
    mControlStore.instructionFetch[2].nextAddr.csar=3; 
    mControlStore.instructionFetch[3].controlSignals=CSIR_default;
    mControlStore.instructionFetch[3].controlSignals.IR_IN=1; mControlStore.instructionFetch[3].controlSignals.MDR_OUT=1; 
    mControlStore.instructionFetch[3].nextAddr.csar=4; 
    mControlStore.instructionFetch[4].controlSignals=CSIR_default;
    mControlStore.instructionFetch[4].controlSignals.BRTABLE=1; 
    mControlStore.instructionFetch[4].nextAddr.csar=0; 
    //load
    mControlStore.load[0].controlSignals=CSIR_default;
    mControlStore.load[0].controlSignals.MAR_IN=1; mControlStore.load[0].controlSignals.IR_OUT=1;
    mControlStore.load[0].nextAddr.csar=6; 
    mControlStore.load[1].controlSignals=CSIR_default;
    mControlStore.load[1].controlSignals.READ=1;
    mControlStore.load[1].nextAddr.csar=7;
    mControlStore.load[2].controlSignals=CSIR_default;
    mControlStore.load[2].controlSignals.ACC_IN=1; mControlStore.load[2].controlSignals.MDR_OUT=1;
    mControlStore.load[2].nextAddr.csar=0;
    //add
    mControlStore.add[0].controlSignals=CSIR_default;
    mControlStore.add[0].controlSignals.MAR_IN=1; mControlStore.add[0].controlSignals.IR_OUT=1;
    mControlStore.add[0].nextAddr.csar=9;
    mControlStore.add[1].controlSignals=CSIR_default;
    mControlStore.add[1].controlSignals.READ=1; 
    mControlStore.add[1].nextAddr.csar=10;
    mControlStore.add[2].controlSignals=CSIR_default;
    mControlStore.add[2].controlSignals.ACC_OUT=1; mControlStore.add[2].controlSignals.ALU_ADD=1;
    mControlStore.add[2].nextAddr.csar=11;
    mControlStore.add[3].controlSignals=CSIR_default;
    mControlStore.add[3].controlSignals.ACC_IN=1; mControlStore.add[3].controlSignals.TMP_OUT=1;
    mControlStore.add[3].nextAddr.csar=0;
    //store
    mControlStore.store[0].controlSignals=CSIR_default;
    mControlStore.store[0].controlSignals.MAR_IN=1; mControlStore.store[0].controlSignals.IR_OUT=1;
    mControlStore.store[0].nextAddr.csar=13;
    mControlStore.store[1].controlSignals=CSIR_default;
    mControlStore.store[1].controlSignals.MDR_IN=1; mControlStore.store[1].controlSignals.ACC_OUT=1;
    mControlStore.store[1].nextAddr.csar=14;
    mControlStore.store[2].controlSignals.WRITE=1;
    //branch
    mControlStore.brz[0].controlSignals=CSIR_default;
    mControlStore.brz[0].orAddr.or_addr=1;
    //sub
    mControlStore.sub[0].controlSignals=CSIR_default;
    mControlStore.sub[0].controlSignals.MAR_IN=1; mControlStore.sub[0].controlSignals.IR_OUT=1;
    mControlStore.sub[0].nextAddr.csar=17;
    mControlStore.sub[1].controlSignals=CSIR_default;
    mControlStore.sub[1].controlSignals.READ=1; 
    mControlStore.sub[1].nextAddr.csar=18;
    mControlStore.sub[2].controlSignals=CSIR_default;
    mControlStore.sub[2].controlSignals.ACC_OUT=1; mControlStore.sub[2].controlSignals.ALU_SUB=1;
    mControlStore.sub[2].nextAddr.csar=19;
    mControlStore.sub[3].controlSignals=CSIR_default;
    mControlStore.sub[3].controlSignals.ACC_IN=1; mControlStore.sub[3].controlSignals.TMP_OUT=1;
    mControlStore.sub[3].nextAddr.csar=0;
    //jsub
    mControlStore.jsub[0].controlSignals=CSIR_default;
    mControlStore.jsub[0].controlSignals.MAR_IN=1; mControlStore.jsub[0].controlSignals.IR_OUT=1;
    mControlStore.jsub[0].nextAddr.csar=21;
    mControlStore.jsub[1].controlSignals=CSIR_default;
    mControlStore.jsub[1].controlSignals.PC_INCR=1; 
    mControlStore.jsub[1].nextAddr.csar=22;
    mControlStore.jsub[2].controlSignals=CSIR_default;
    mControlStore.jsub[2].controlSignals.MDR_IN=1; mControlStore.jsub[2].controlSignals.PC_OUT=1;
    mControlStore.jsub[2].nextAddr.csar=23;
    mControlStore.jsub[3].controlSignals=CSIR_default;
    mControlStore.jsub[3].controlSignals.WRITE=1;
    mControlStore.jsub[3].nextAddr.csar=0;
    //jmpi
    mControlStore.jmpi[0].controlSignals=CSIR_default;
    mControlStore.jmpi[0].controlSignals.MAR_IN=1; mControlStore.jmpi[0].controlSignals.IR_OUT=1;
    mControlStore.jmpi[0].nextAddr.csar=25;
    mControlStore.jmpi[1].controlSignals=CSIR_default;
    mControlStore.jmpi[1].controlSignals.READ=1; 
    mControlStore.jmpi[1].nextAddr.csar=26;
    mControlStore.jmpi[2].controlSignals=CSIR_default;
    mControlStore.jmpi[2].controlSignals.MDR_OUT=1; mControlStore.jmpi[2].controlSignals.PC_IN=1;
    mControlStore.jmpi[2].nextAddr.csar=27;
    mControlStore.jmpi[3].controlSignals=CSIR_default;
    mControlStore.jmpi[3].controlSignals.WRITE=1;
    mControlStore.jmpi[3].nextAddr.csar=0;
    return mControlStore;
}

void printRegisters(struct Registers temp){
	printf("%x\t", temp.PC); 
  	printf("%x\t", temp.IR); 
	printf("%x\t", temp.MAR); 
	printf("%x\t", temp.MDR); 
	printf("%x\t", temp.ACC); 
	printf("%x\t", temp.TMP); 
	
}

void printCSIR(struct ControlStoreEntry temp){
	printf("%d", temp.controlSignals.ACC_IN); //bit  0 == ACC_in
  	printf("%d", temp.controlSignals.ACC_OUT); //bit  1 == ACC_out
	printf("%d", temp.controlSignals.ALU_ADD); //bit  2 == alu_add
	printf("%d", temp.controlSignals.ALU_SUB); //bit  3 == alu_sub
	printf("%d", temp.controlSignals.IR_IN); //bit  4 == IR_in
	printf("%d", temp.controlSignals.IR_OUT); //bit  5 == IR_out
	printf("%d", temp.controlSignals.MAR_IN); //bit  6 == MAR_in
	printf("%d", temp.controlSignals.MDR_IN); //bit  7 == MDR_in
	printf("%d", temp.controlSignals.MDR_OUT); //bit  8 == MDR_out
	printf("%d", temp.controlSignals.PC_IN); //bit  9 == PC_in
	printf("%d", temp.controlSignals.PC_OUT); //bit 10 == PC_out
	printf("%d", temp.controlSignals.PC_INCR); //bit 11 == pc_incr
	printf("%d", temp.controlSignals.READ); //bit 12 == read
	printf("%d", temp.controlSignals.TMP_OUT); //bit 13 == TMP_out
	printf("%d", temp.controlSignals.WRITE); //bit 14 == write
	printf("%d", temp.controlSignals.BRTABLE); //bit 15 == br_table
	printf("|%02x|", temp.nextAddr.csar); //next address in hex form
	printf("%d\t", temp.orAddr.or_addr); //or addr
	if(temp.controlSignals.ACC_IN==1)
		printf("ACC_in ");
	if(temp.controlSignals.ACC_OUT==1)
		printf("ACC_out ");
	if(temp.controlSignals.ALU_ADD==1) 
		printf("alu_add ");
	if(temp.controlSignals.ALU_SUB==1)
		printf("alu_sub ");
	if(temp.controlSignals.IR_IN==1)
		printf("IR_in ");
	if(temp.controlSignals.IR_OUT==1) 
		printf("IR_out ");
	if(temp.controlSignals.MAR_IN==1) 
		printf("MAR_in ");
	if(temp.controlSignals.MDR_IN==1) 
		printf("MDR_in ");
	if(temp.controlSignals.MDR_OUT==1) 
		printf("MDR_out ");
	if(temp.controlSignals.PC_IN==1) 
		printf("PC_in ");
	if(temp.controlSignals.PC_OUT==1) 
		printf("PC_out ");
	if(temp.controlSignals.PC_INCR==1) 
		printf("pc_incr ");
	if(temp.controlSignals.READ==1) 
		printf("read ");
	if(temp.controlSignals.TMP_OUT==1) 
		printf("TMP_out ");
	if(temp.controlSignals.WRITE==1)
		printf("write ");
	if(temp.controlSignals.BRTABLE==1) 
		printf("br_table ");
	if(temp.orAddr.or_addr==1)
		printf("or_addr");
	printf("\n");
}

struct ControlStoreEntry indexCS(struct ControlStore temp, int csar){
	switch(csar) {
		case 0:
			return temp.instructionFetch[0];
		case 1:
			return temp.instructionFetch[1];
		case 2:
			return temp.instructionFetch[2];
		case 3:
			return temp.instructionFetch[3];
		case 4:
			return temp.instructionFetch[4];
		case 5:
			return temp.load[0];
		case 6:
			return temp.load[1];
		case 7:
			return temp.load[2];
		case 8:
			return temp.add[0];
		case 9:
			return temp.add[1];
		case 10:
			return temp.add[2];
		case 11:
			return temp.add[3];
		case 12:
			return temp.store[0];
		case 13:
			return temp.store[1];
		case 14:
			return temp.store[2];
		case 15:
			return temp.brz[0];
		case 16:
			return temp.sub[0];
		case 17:
			return temp.sub[1];
		case 18:
			return temp.sub[2];
		case 19:
			return temp.sub[3];
		case 20:
			return temp.jsub[0];
		case 21:
			return temp.jsub[1];
		case 22:
			return temp.jsub[2];
		case 23:
			return temp.jsub[3];
		case 24:
			return temp.jmpi[0];
		case 25:
			return temp.jmpi[1];
		case 26:
			return temp.jmpi[2];
		case 27:
			return temp.jmpi[3];

	}
	return temp.halt[0];
}

int decodeTable(int opcode){
	switch(opcode){
		case 0: //load
			return 5;
		case 1: //add
			return 8;
		case 2: //store
			return 12;
		case 3: //brz
			return 15;
		case 4: //sub
			return 16;
		case 5: //jsub
			return 20;
		case 6: //jmpi
			return 25;
		case 7: //halt
			return -1;
	}
	return -1;
}