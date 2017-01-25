#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <SDL/SDL.h>

#include "Opcodes.hpp"

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define STACK_SIZE 16
#define CPU_SPEED 4
#define FPS 16
#define NB_OPCODES 35

class Cpu {
public:
    Cpu (Screen * sc, bool debug);

    void start ();
    void dump () const;
    void loadProgram (const char* file_name);
    void shutdown ();

    void count ();
    Uint16 getNextOpCode ();
    void exec_opcode (const Uint16 opcode);
    void debug_inst (uint16_t opcode, Opcode * op);

    // void init_opcodes ();
    void loadFont ();

    bool started;
    Uint8 memory[MEM_SIZE];
    /* program counter */
    Uint16 pc;
    /* 16 registers V0 -> VF */
    Uint8 reg[V_REGISTERS_SIZE];
    /* Usually used to store memory adresses */
    Uint16 I;
    /* Stack pointer */
    Uint8 sp;
    Uint16 stack[STACK_SIZE];
    /* delay timer */
    Uint8 delay_timer;
    /* sound timer */
    Uint8 sound_timer;
    /* Pointer on the screen */
    Screen * screen;

    bool debug;
};
