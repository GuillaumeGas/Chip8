#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <SDL2/SDL.h>

#include "Exceptions.hpp"
#include "Opcodes.hpp"
#include "GameScreen.hpp"

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define RPL_SIZE 8
#define STACK_SIZE 16
#define NB_OPCODES 35
#define KEYBOARD_SIZE 16
#define KEY_DOWN 1
#define KEY_UP 0

class Cpu {
public:
	Cpu(GameScreen * screen);

	void loadProgram(const char* file_name);
	virtual bool emulateCycle(); // virtual because it is overrided in the CpuDebug class
	Uint16 getNextOpCode();
	void execOpcode(const Uint16 opcode);
	void count();
	bool isRunning() const;
	void loadFont();
	void shutdown();
	uint16_t GetOpcodeId(uint16_t opcode);

	bool running;
	uint8_t memory[MEM_SIZE];
	/* program counter */
	uint16_t pc;
	/* 16 registers V0 -> VF */
	uint8_t reg[V_REGISTERS_SIZE];
	/* rpl registers */
	uint8_t rpl[RPL_SIZE];
	/* Usually used to store memory adresses */
	uint16_t I;
	/* Stack pointer */
	uint8_t sp;
	uint16_t stack[STACK_SIZE];
	/* delay timer */
	uint8_t delay_timer;
	/* sound timer */
	uint8_t sound_timer;
	/* Pointer on the screen */
	GameScreen * screen;

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
