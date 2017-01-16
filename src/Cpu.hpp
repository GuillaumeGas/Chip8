#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <SDL/SDL.h>

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define STACK_SIZE 16

struct Cpu {
    Uint8 _memory[MEM_SIZE];
    /* program counter */
    Uint16 _pc;
    /* 16 registers V0 -> VF */
    Uint8 _v_registers[V_REGISTERS_SIZE];
    /* Usually used to store memory adresses */
    Uint16 _I;
    /* Stack pointer */
    Uint8 _sp;
    Uint16 _stack[STACK_SIZE];
    /* delay timer */
    Uint8 _delay_counter;
    /* sound timer */ 
    Uint8 _sound_counter;

    Cpu ();
    void count ();

    Uint8 * getRegV (unsigned int i);

    void print () const;
};
