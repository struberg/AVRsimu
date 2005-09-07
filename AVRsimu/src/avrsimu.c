/*
 *  AVRsimu
 *
 *  Copyright (C) 2005 Jonas Diemer,  diemer@gmx.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Street #330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>

#include "avr.h"
#include "instructions.h"

int load_ihex (struct avrmcu * avr, const char * filename);
void printHelp();
void print_registers( struct avrmcu* avr );
void print_sram(struct avrmcu * avr);
void print_flash(struct avrmcu * avr);

int run(struct avrmcu * avr);




int main(int argc, const char* argv[])
{
	struct avrmcu * avr;
	int    bCont = 1;
	unsigned char   cCmd  = 0;
	
	avr = create_avrmcu();
	instructions = init_instructions_array();	
	//init_opcodes();
	
	if( argc != 2 ) {
		printf( "usage: %s hexfilename\n", argv[0] );
		return 0;
	}	
	clear_flash(avr);
	load_ihex(avr, argv[1]);
	//print_flash(&avr);
	
	reset_avr(avr);
	//print_sram(&avr);
	
	while( bCont ) {
		cCmd = getchar();
		switch( cCmd) {
			case '?': 
				printHelp();
				break;
			case 's':
				print_sram(avr);
				break;
			case 'r':
				print_registers(avr);
				break;
			case 'f':
				print_flash(avr);
				break;
			case 'n':
				step(avr);
				break;
			case 'x':
				bCont=0;
				break;
		}
	}
	
	printf("\n");
	
}

void printHelp() {
	printf( "?   help\n");
	printf( "n   next step\n");
	printf( "s   show SRAM\n");
	printf( "r   show Registers\n");
	printf( "f   show Flash\n");
	printf( "x   exit\n");
}


void print_sram(struct avrmcu * avr)
{
	unsigned int i;
	printf( " SRAM:    +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F" );
	for (i=0;i<=RAMEND;i++){
		if (i%16==0){
			printf("\n %06x: ",i);
		}
		printf(" %02x",avr->sram[i]);
	}
	printf("\n");
}


void print_registers( struct avrmcu* avr ) {
	unsigned int i;
	for (i=0;i<=31;i++){
		printf( "R%02d =%02X(%03d)%c", i, avr->sram[i], avr->sram[i], (i+1)%8?' ':'\n' );
	}
	unsigned short int x = (avr->registers[27]<<8) | avr->registers[26];
	unsigned short int y = (avr->registers[29]<<8) | avr->registers[28];
	unsigned short int z = (avr->registers[31]<<8) | avr->registers[30];
	printf( "X: %04X (%d)   Y: %04X (%d)   Z: %04X (%d)\n"
	      ,  x, x, y, y, z, z );
}

void print_flash(struct avrmcu * avr)
{
	unsigned int i;
	for (i=0;i<=avr->lastinstruction;i++){
		if (i%8==0){
			printf("\n %06x: ",i);
		}
		printf(" %04x",avr->flash[i]);
	}
	printf("\n");
}

int run(struct avrmcu * avr)
{
 	unsigned short int opcode;
	int i;
	
	
	while (avr->PC<=FLASHEND){	//the avr never stops?
		step(avr);
	}		
}
