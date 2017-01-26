#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <SDL/SDL.h>

#include "Opcodes.hpp"
#include "Screen.hpp"

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define STACK_SIZE 16
#define CPU_SPEED 4
#define FPS 16
#define NB_OPCODES 35
#define KEYBOARD_SIZE 16
#define KEY_DOWN 1
#define KEY_UP 0

class Chip8;
class Cpu {
public:
    Cpu (Chip8 * ctrl, Screen * sc, bool debug);

    void dump () const;
    void loadProgram (const char* file_name);
    bool emulateCycle ();
    Uint16 getNextOpCode ();
    void exec_opcode (const Uint16 opcode);
    void count ();
    void debug_inst (uint16_t opcode, Opcode * op);

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
    /* Pointer on the controler */
    Chip8 * ctrl;

    bool debug;

    /**
       Keyboard 4x4
       Originally like this :
            1 2 3 C
            4 5 6 D
            7 8 9 E
            A 0 B F
       Now mapped on the numeric keypad
            7 8 9 *
            4 5 6 -
            1 2 3 +
            _ 0 . _  (the first '_' is the right arrow and the second the enter button of the keypad.)

     */
    uint8_t keyboard[KEYBOARD_SIZE];
};
