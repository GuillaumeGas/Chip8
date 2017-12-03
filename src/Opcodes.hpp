#pragma once

#include <iostream>
#include <map>
#include <cstdint>

#include "utils/Utils.hpp"

// JE ROX / I ROCK !! 
#define STATIC_THIS(o)				\
    static struct _init_ {			\
    _init_ () {					\
	o;					\
	}					\
    } _initializer;						

#define NB_MASK 5

class Cpu;
class Opcode;
class Chip8;

class Opcodes {
public:
	static Opcodes * instance();
	std::map<uint16_t, Opcode*> * getList();

private:
	Opcodes();
	static Opcodes * inst;
	std::map<uint16_t, Opcode*> opcodes_list;
};

class Opcode {
public:
	virtual void execute(const uint16_t opcode, Chip8 * chip) = 0;
	virtual std::string disassemble(const uint16_t opcode) = 0;

	uint16_t getX(const uint16_t opcode) const;
	uint16_t getY(const uint16_t opcode) const;
	uint16_t getZ(const uint16_t opcode) const;
	uint16_t getData(const uint16_t opcode) const;
};

/**
   00E0 - CLS
   Clear the display.
 */
class OpCls : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00E0] = new OpCls());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   00EE - RET
   Return from a subroutine.

   The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
 */
class OpRet : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00EE] = new OpRet());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   1nnn - JP 5addr
   Jump to location nnn.

   The interpreter sets the program counter to nnn.
 */
class OpJpAddr : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x1000] = new OpJpAddr());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   2nnn - CALL addr
   Call subroutine at nnn.

   The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 */
class OpCallAddr : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x2000] = new OpCallAddr());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   3xkk - SE Vx, byte
   Skip next instruction if Vx = kk.

   The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
 */
class OpSeVxByte : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x3000] = new OpSeVxByte());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   4xkk - SNE Vx, byte
   Skip next instruction if Vx != kk.

   The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
 */
class OpSneVxByte : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x4000] = new OpSneVxByte());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   5xy0 - SE Vx, Vy
   Skip next instruction if Vx = Vy.

   The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
 */
class OpSeVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x5000] = new OpSeVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   6xkk - LD Vx, byte
   Set Vx = kk.

   The interpreter puts the value kk into register Vx.
 */
class OpLdVxByte : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x6000] = new OpLdVxByte());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   7xkk - ADD Vx, byte
   Set Vx = Vx + kk.

   Adds the value kk to the value of register Vx, then stores the result in Vx.
 */
class OpAddVxByte : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x7000] = new OpAddVxByte());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy0 - LD Vx, Vy
   Set Vx = Vy.

   Stores the value of register Vy in register Vx.
 */
class OpLdVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8000] = new OpLdVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy1 - OR Vx, Vy
   Set Vx = Vx OR Vy.

   Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A bitwise OR compares the corrseponding bits from two values, and if either bit is 1, then the same bit in the result is also 1. Otherwise, it is 0.
 */
class OpOrVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8001] = new OpOrVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy2 - AND Vx, Vy
   Set Vx = Vx AND Vy.

   Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A bitwise AND compares the corrseponding bits from two values, and if both bits are 1, then the same bit in the result is also 1. Otherwise, it is 0.
 */
class OpAndVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8002] = new OpAndVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy3 - XOR Vx, Vy
   Set Vx = Vx XOR Vy.

   Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits are not both the same, then the corresponding bit in the result is set to 1. Otherwise, it is 0.
 */
class OpXorVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8003] = new OpXorVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy4 - ADD Vx, Vy
   Set Vx = Vx + Vy, set VF = carry.

   The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
 */
class OpAddVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8004] = new OpAddVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy5 - SUB Vx, Vy
   Set Vx = Vx - Vy, set VF = NOT borrow.

   If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
 */
class OpSubVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8005] = new OpSubVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy6 - SHR Vx {, Vy}
   Set Vx = Vx SHR 1.

   If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
 */
class OpShrVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8006] = new OpShrVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xy7 - SUBN Vx, Vy
   Set Vx = Vy - Vx, set VF = NOT borrow.

   If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
 */
class OpSubnVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x8007] = new OpSubnVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   8xyE - SHL Vx {, Vy}
   Set Vx = Vx SHL 1.

   If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
 */
class OpShlVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x800E] = new OpShlVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   9xy0 - SNE Vx, Vy
   Skip next instruction if Vx != Vy.

   The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
 */
class OpSneVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x9000] = new OpSneVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Annn - LD I, addr
   Set I = nnn.

   The value of register I is set to nnn.
 */
class OpLdIAddr : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xA000] = new OpLdIAddr());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Bnnn - JP V0, addr
   Jump to location nnn + V0.

   The program counter is set to nnn plus the value of V0.
 */
class OpJpV0Addr : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xB000] = new OpJpV0Addr());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Cxkk - RND Vx, byte
   Set Vx = random byte AND kk.

   The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
 */
class OpRndVxByte : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xC000] = new OpRndVxByte());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Dxyn - DRW Vx, Vy, nibble
   Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

   The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
 */
class OpDrwVxVy : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xD000] = new OpDrwVxVy());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Ex9E - SKP Vx
   Skip next instruction if key with the value of Vx is pressed.

   Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
 */
class OpSkpVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xE09E] = new OpSkpVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   ExA1 - SKNP Vx
   Skip next instruction if key with the value of Vx is not pressed.

   Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
 */
class OpSknpVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xE0A1] = new OpSknpVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx07 - LD Vx, DT
   Set Vx = delay timer value.

   The value of DT is placed into Vx.
 */
class OpLdVxDt : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF007] = new OpLdVxDt());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx0A - LD Vx, K
   Wait for a key press, store the value of the key in Vx.

   All execution stops until a key is pressed, then the value of that key is stored in Vx.
 */
class OpLdVxK : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF00A] = new OpLdVxK());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx15 - LD DT, Vx
   Set delay timer = Vx.

   DT is set equal to the value of Vx.
 */
class OpLdDtVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF015] = new OpLdDtVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx18 - LD ST, Vx
   Set sound timer = Vx.

   ST is set equal to the value of Vx.
 */
class OpLdStVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF018] = new OpLdStVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx1E - ADD I, Vx
   Set I = I + Vx.

   The values of I and Vx are added, and the results are stored in I.
 */
class OpAddIVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF01E] = new OpAddIVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**

   Fx29 - LD F, Vx
   Set I = location of sprite for digit Vx.

   The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
 */
class OpLdFVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF029] = new OpLdFVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx33 - LD B, Vx
   Store BCD representation of Vx in memory locations I, I+1, and I+2.

   The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
 */
class OpLdBVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF033] = new OpLdBVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx55 - LD [I], Vx
   Store registers V0 through Vx in memory starting at location I.

   The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.
 */
class OpLdIVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF055] = new OpLdIVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};


/**
   Fx65 - LD Vx, [I]
   Read registers V0 through Vx from memory starting at location I.

   The interpreter reads values from memory starting at location I into registers V0 through Vx.
 */
class OpLdVxI : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF065] = new OpLdVxI());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   0nnn - Sys addr
   Jump to a machine code routine at nnn.

   This instruction is only used on the old computers on which Chip-8 was originally implemented. It is ignored by modern interpreters.
*/
class OpSys : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x0000] = new OpSys());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00CX - Scroll down X lines
 */
class OpDown : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00C0] = new OpDown());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00FB - Scroll right 4 pixels
 */
class OpRight : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00FB] = new OpRight());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00FC - Scroll left 4 pixels
 */
class OpLeft : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00FC] = new OpLeft());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00FE - disable extended screen mode
 */
class OpLow : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00FE] = new OpLow());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00FF - enable extended screen mode
 */
class OpHigh : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00FF] = new OpHigh());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   00FD - exit
 */
class OpExit : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0x00FD] = new OpExit());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};

/**
   SUPER CHIP 8

   F030 - LD HF Vx
 */
class OpLdHFVx : public Opcode {
public:
	STATIC_THIS((*(Opcodes::instance()->getList()))[0xF033] = new OpLdHFVx());

	void execute(const uint16_t opcode, Chip8 * chip);
	std::string disassemble(const uint16_t opcode);
};
