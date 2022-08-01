#include "arcade_machine/arcade_machine.h"

machine_t* create_machine() {
  machine_t* machine = calloc(1, sizeof(machine_t));

  machine->memory = malloc(I8080_MAX_MEMORY);

  init_i8080(&machine->cpu);
  machine->cpu.external_memory =
      machine->memory;  // set cpu's memory reference to memory of machine

  machine->next_interrupt = 1;
  machine->in_port1 = 1 << 3;  // bit 3 always set
  machine->in_port2 = 0;
  machine->shift0 = 0;
  machine->shift1 = 0;
  machine->shift_offset = 0;

  memset(machine->screen_buffer, 0, sizeof(machine->screen_buffer));

  return machine;
}

void destroy_machine(machine_t* machine) {
  free(machine->memory);
  free(machine);
}

// called every frame executing 2MHz/60fps clock cycles
void machine_update_state(machine_t* machine) {
  int cycle_count = 0;

  while (cycle_count <= MACHINE_CYCLES_PER_FRAME) {
    int start_cycles = machine->cpu.cycles;

    i8080_step(&machine->cpu);

    cycle_count += machine->cpu.cycles - start_cycles;

    uint8_t* opcode = &machine->memory[machine->cpu.pc];

    // handle INPUT/OUTPUT instructions which refer to machine ports
    // i8080_step function skipped these instructions as they reference external
    // hardware
    if (*opcode == 0xdb) {  // IN

      uint8_t port = opcode[1];

      switch (port) {
        case 1:
          machine->cpu.a = machine->in_port1;
          break;

        case 2:
          machine->cpu.a = machine->in_port2;
          break;

        case 3: {
          uint16_t v = (machine->shift1 << 8) | machine->shift0;
          machine->cpu.a = (v >> (8 - machine->shift_offset)) & 0xff;
        } break;
      }

      machine->cpu.pc += 2;
    } else if (*opcode == 0xd3) {  // OUT

      uint8_t port = opcode[1];

      switch (port) {
        case 2:
          machine->shift_offset = machine->cpu.a & 0x7;
          break;

        case 4:
          machine->shift0 = machine->shift1;
          machine->shift1 = machine->cpu.a;
          break;
      }

      machine->cpu.pc += 2;
    }

    // RST 1 (0x08) interrupt when rendering reaches middle of screen
    // RST 2 (0x10) interrupt at end of screen
    if (machine->cpu.cycles >= MACHINE_HALF_CYCLES_PER_FRAME) {
      if (machine->cpu.ie) {
        machine->cpu.ie = 0;
        i8080_rst(&machine->cpu, machine->next_interrupt);
        machine->cpu.cycles += 11;  // cycles taken by an interrupt
      }

      machine->cpu.cycles -= MACHINE_HALF_CYCLES_PER_FRAME;
      machine->next_interrupt = machine->next_interrupt == 1 ? 2 : 1;
    }
  }
}

// called every frame, black and white
void machine_update_screen_buffer(machine_t* machine) {
  for (int x = 0; x < MACHINE_SCREEN_WIDTH; x++) {
    uint16_t offset = 0x241f + (x * 0x20);

    for (int y = 0; y < MACHINE_SCREEN_HEIGHT; y += 8) {
      uint8_t byte = machine->memory[offset];

      for (int bit = 0; bit < 8; bit++) {
        if ((byte << bit) & 0x80) {
          machine->screen_buffer[y + bit][x][0] = 255;
          machine->screen_buffer[y + bit][x][1] = 255;
          machine->screen_buffer[y + bit][x][2] = 255;
        } else {
          machine->screen_buffer[y + bit][x][0] = 0;
          machine->screen_buffer[y + bit][x][1] = 0;
          machine->screen_buffer[y + bit][x][2] = 0;
        }
      }

      offset--;
    }
  }
}

void machine_file_to_mem(machine_t* machine,
                         const char* file_name,
                         uint16_t offset) {
  // try open file
  FILE* file = fopen(file_name, "rb");
  if (!file) {
    printf("Could not read file: %s\n", file_name);
    exit(0);
  }

  // get file size
  fseek(file, 0L, SEEK_END);
  int file_size = ftell(file);
  fseek(file, 0L, SEEK_SET);

  // read file contents into state memory
  uint8_t* buffer = &machine->memory[offset];
  fread(buffer, file_size, 1, file);
  fclose(file);
}

void machine_load_invaders(machine_t* machine) {
  machine_file_to_mem(machine, "res/roms/invaders.h", 0x0);
  machine_file_to_mem(machine, "res/roms/invaders.g", 0x800);
  machine_file_to_mem(machine, "res/roms/invaders.f", 0x1000);
  machine_file_to_mem(machine, "res/roms/invaders.e", 0x1800);
}
