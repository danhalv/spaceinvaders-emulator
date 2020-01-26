//representation of the arcade machine
#ifndef MACHINE_H
#define MACHINE_H

#include "i8080/i8080.h"
#include <stdlib.h>
#include <string.h>

#define MACHINE_SCREEN_WIDTH 			224
#define MACHINE_SCREEN_HEIGHT			256
#define MACHINE_FPS						60
#define MACHINE_CLOCK_RATE				2000000	//2MHz
#define MACHINE_CYCLES_PER_FRAME		MACHINE_CLOCK_RATE / MACHINE_FPS	//2x10^6 cpu per second. 60 frames per second
#define MACHINE_HALF_CYCLES_PER_FRAME	MACHINE_CYCLES_PER_FRAME / 2		//used for interrupts 

typedef struct {
	i8080_t cpu;	
	uint8_t *memory;
	uint8_t screen_buffer[MACHINE_SCREEN_HEIGHT][MACHINE_SCREEN_WIDTH][3];

	uint8_t next_interrupt;
	uint8_t in_port1, in_port2;
	uint8_t shift0, shift1, shift_offset;
} machine_t;

machine_t* create_machine();
void destroy_machine(machine_t *machine);

void machine_update_state(machine_t *machine);

void machine_update_screen_buffer(machine_t *machine);

void machine_load_invaders(machine_t *machine);

void machine_file_to_mem(machine_t *machine, const char* file_name, uint16_t offset);

#endif	//MACHINE_H
