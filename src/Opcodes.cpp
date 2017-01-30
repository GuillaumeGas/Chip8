#include "Cpu.hpp"

#include "Opcodes.hpp"

using namespace std;

OpCls::_init_ OpCls::_initializer;
OpRet::_init_ OpRet::_initializer;
OpJpAddr::_init_ OpJpAddr::_initializer;
OpCallAddr::_init_ OpCallAddr::_initializer;
OpSeVxByte::_init_ OpSeVxByte::_initializer;
OpSneVxByte::_init_ OpSneVxByte::_initializer;
OpSeVxVy::_init_ OpSeVxVy::_initializer;
OpLdVxByte::_init_ OpLdVxByte::_initializer;
OpAddVxByte::_init_ OpAddVxByte::_initializer;
OpLdVxVy::_init_ OpLdVxVy::_initializer;
OpOrVxVy::_init_ OpOrVxVy::_initializer;
OpAndVxVy::_init_ OpAndVxVy::_initializer;
OpXorVxVy::_init_ OpXorVxVy::_initializer;
OpAddVxVy::_init_ OpAddVxVy::_initializer;
OpSubVxVy::_init_ OpSubVxVy::_initializer;
OpShrVxVy::_init_ OpShrVxVy::_initializer;
OpSubnVxVy::_init_ OpSubnVxVy::_initializer;
OpShlVxVy::_init_ OpShlVxVy::_initializer;
OpSneVxVy::_init_ OpSneVxVy::_initializer;
OpLdIAddr::_init_ OpLdIAddr::_initializer;
OpJpV0Addr::_init_ OpJpV0Addr::_initializer;
OpRndVxByte::_init_ OpRndVxByte::_initializer;
OpDrwVxVy::_init_ OpDrwVxVy::_initializer;
OpSkpVx::_init_ OpSkpVx::_initializer;
OpSknpVx::_init_ OpSknpVx::_initializer;
OpLdVxDt::_init_ OpLdVxDt::_initializer;
OpLdVxK::_init_ OpLdVxK::_initializer;
OpLdDtVx::_init_ OpLdDtVx::_initializer;
OpLdStVx::_init_ OpLdStVx::_initializer;
OpAddIVx::_init_ OpAddIVx::_initializer;
OpLdFVx::_init_ OpLdFVx::_initializer;
OpLdBVx::_init_ OpLdBVx::_initializer;
OpLdIVx::_init_ OpLdIVx::_initializer;
OpLdVxI::_init_ OpLdVxI::_initializer;
OpSys::_init_ OpSys::_initializer;
OpDown::_init_ OpDown::_initializer;
OpRight::_init_ OpRight::_initializer;
OpLeft::_init_ OpLeft::_initializer;
OpLow::_init_ OpLow::_initializer;
OpHigh::_init_ OpHigh::_initializer;

Opcodes * Opcodes::inst = NULL;

Opcodes::Opcodes () {}

Opcodes * Opcodes::instance () {
    if (inst == NULL)
	inst = new Opcodes ();
    return inst;
}

map<uint16_t, Opcode*> * Opcodes::getList () {
    return &opcodes_list;
}

uint16_t Opcode::getX (const uint16_t opcode) const {
    return (opcode & 0x0F00) >> 8;
}

uint16_t Opcode::getY (const uint16_t opcode) const {
    return (opcode & 0x00F0) >> 4;
}

uint16_t Opcode::getZ (const uint16_t opcode) const {
    return (opcode & 0x000F);
}

uint16_t Opcode::getData (const uint16_t opcode) const {
    return (opcode & 0x0FFF);
}

void OpCls::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    screen->clear ();
}

string OpCls::disassemble (const uint16_t opcode) {
    return "cls";
}

void OpRet::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->sp--;
    cpu->pc = cpu->stack[cpu->sp];
}

string OpRet::disassemble (const uint16_t opcode) {
    return "ret";
}

void OpJpAddr::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->pc = opcode & 0x0FFF;
    cpu->pc -= 2;
}

string OpJpAddr::disassemble (const uint16_t opcode) {
    return "jp " + htos (opcode & 0x0FFF);
}

void OpCallAddr::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->stack[cpu->sp] = cpu->pc;
    cpu->sp++;
    cpu->pc = opcode & 0x0FFF;
    cpu->pc -= 2;
}

string OpCallAddr::disassemble (const uint16_t opcode) {
    return "call " + htos (opcode & 0x0FFF);
}

void OpSeVxByte::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->reg[getX (opcode)] == (opcode & 0x00FF))
	cpu->pc += 2;
}

string OpSeVxByte::disassemble (const uint16_t opcode) {
    return "se v" + std::to_string ((int) getX (opcode)) + " " + std::to_string ((int) (opcode & 0x00FF));
}

void OpSneVxByte::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->reg[getX (opcode)] != (opcode & 0x00FF))
	cpu->pc += 2;
}

string OpSneVxByte::disassemble (const uint16_t opcode) {
    return "sne v" + std::to_string ((int) getX (opcode)) + " " + std::to_string ((int) (opcode & 0x00FF));
}

void OpSeVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->reg[getX (opcode)] == cpu->reg[getY (opcode)])
	cpu->pc += 2;
}

string OpSeVxVy::disassemble (const uint16_t opcode) { 
    return "se v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpLdVxByte::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] = (opcode & 0x00FF);
}

string OpLdVxByte::disassemble (const uint16_t opcode) { 
    return "ld v" + std::to_string ((int) getX (opcode)) + " " + std::to_string ((int) (opcode & 0x00FF));
}

void OpAddVxByte::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] = cpu->reg[getX (opcode)] + (opcode & 0x00FF);
}

string OpAddVxByte::disassemble (const uint16_t opcode) { 
    return "add v" + std::to_string ((int) getX (opcode)) + " " + std::to_string ((int) (opcode & 0x00FF));
}

void OpLdVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] = cpu->reg[getY (opcode)];
}

string OpLdVxVy::disassemble (const uint16_t opcode) { 
    return "ld v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpOrVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] |= cpu->reg[getY (opcode)];
}

string OpOrVxVy::disassemble (const uint16_t opcode) { 
    return "or v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpAndVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] &= cpu->reg[getY (opcode)];
}

string OpAndVxVy::disassemble (const uint16_t opcode) { 
    return "and v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}
void OpXorVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] ^= cpu->reg[getY (opcode)];
}

string OpXorVxVy::disassemble (const uint16_t opcode) { 
    return "xor v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpAddVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    uint16_t y = getY (opcode);
    uint16_t sum = cpu->reg[x] + cpu->reg[y];
    if (sum > 255) {
	cpu->reg[0xF] = 1;
    } else {
	cpu->reg[0xF] = 0;
    }
    cpu->reg[x] = sum & 0x00FF;
}

string OpAddVxVy::disassemble (const uint16_t opcode) { 
    return "add v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpSubVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    uint16_t y = getY (opcode);
    if (cpu->reg[x] > cpu->reg[y]) {
	cpu->reg[0xF] = 1;
    } else {
	cpu->reg[0xF] = 0;
    }
    cpu->reg[x] = cpu->reg[x] - cpu->reg[y];
}

string OpSubVxVy::disassemble (const uint16_t opcode) {
    return "sub v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpShrVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    uint16_t y = getY (opcode);
    if ((cpu->reg[x] & 0x000F) == 1) {
	cpu->reg[0xF] = 1;
    } else {
	cpu->reg[0xF] = 0;
    }
    cpu->reg[x] = cpu->reg[x] / 2;
}

string OpShrVxVy::disassemble (const uint16_t opcode) {
    return "shr v" + std::to_string ((int) getX (opcode));
}

void OpSubnVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    uint16_t y = getY (opcode);
    if (cpu->reg[y] > cpu->reg[x]) {
	cpu->reg[0xF] = 1;
    } else {
	cpu->reg[0xF] = 0;
    }
    cpu->reg[x] = cpu->reg[y] - cpu->reg[x];
}

string OpSubnVxVy::disassemble (const uint16_t opcode) { 
    return "subn v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpShlVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    cpu->reg[0xF] = (cpu->reg[x] & 0x80) >> 7;
    cpu->reg[x] <<= 1;
}

string OpShlVxVy::disassemble (const uint16_t opcode) { 
    return "shl v" + std::to_string ((int) getX (opcode));
}
void OpSneVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->reg[getX (opcode)] != cpu->reg[getY (opcode)])
	cpu->pc += 2;
}

string OpSneVxVy::disassemble (const uint16_t opcode) { 
    return "sne v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpLdIAddr::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->I = opcode & 0x0FFF;
}

string OpLdIAddr::disassemble (const uint16_t opcode) { 
    return "ld I " + std::to_string ((int) (opcode & 0x0FFF));
}

void OpJpV0Addr::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->pc = (opcode & 0x0FFF) + cpu->reg[0];
    cpu->pc -= 2; //because of the +=2 in the cpu's code
}

string OpJpV0Addr::disassemble (const uint16_t opcode) { 
    return "jp v0 " + std::to_string ((int) (opcode & 0x0FFF));
}

void OpRndVxByte::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] = (rand () % 255 + 1) & (opcode & 0x00FF);
}

string OpRndVxByte::disassemble (const uint16_t opcode) { 
    return "rnd v" + std::to_string ((int) getX (opcode)) + " " + std::to_string ((int) (opcode & 0x00FF));
}

void OpDrwVxVy::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    uint16_t y = getY (opcode);
    uint16_t height = opcode & 0x000F;
    uint16_t pixel;

    cpu->reg[0xF] = 0;
    for (int i = 0; i < height; i++) {
	pixel = cpu->memory[cpu->I + i];
	for (int j = 0; j < 8; j++) {
	    if ((pixel & (0x80 >> j)) != 0) {
		if (screen->getPixel (cpu->reg[x] + j, cpu->reg[y] + i).color == WHITE) {
		    cpu->reg[0xF] = 1;
		    screen->setColor (cpu->reg[x] + j, cpu->reg[y] + i, BLACK);
		} else {
		    screen->setColor (cpu->reg[x] + j, cpu->reg[y] + i, WHITE);
		}
	    }
	}
    }
}

string OpDrwVxVy::disassemble (const uint16_t opcode) {
    return "drw v" + std::to_string ((int) getX (opcode)) + " v" + std::to_string ((int) getY (opcode));
}

void OpSkpVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->keyboard[cpu->reg[getX (opcode)]] == 1)
	cpu->pc += 2;
}

string OpSkpVx::disassemble (const uint16_t opcode) {
    return "skp v" + std::to_string ((int) getX (opcode));
}

void OpSknpVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    if (cpu->keyboard[cpu->reg[getX (opcode)]] == 0)
	cpu->pc += 2;
}

string OpSknpVx::disassemble (const uint16_t opcode) {
    return "sknp v" + std::to_string ((int) getX (opcode));
}

void OpLdVxDt::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->reg[getX (opcode)] = cpu->delay_timer;
}

string OpLdVxDt::disassemble (const uint16_t opcode) {
    return "ld v" + std::to_string ((int) getX (opcode)) + " dt";
}

void OpLdVxK::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    bool found = false;
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
	if (cpu->keyboard[i] == KEY_DOWN) {
	    cpu->reg[getX (opcode)] = i;
	    found = true;
	}
    }
    if (!found)
	cpu->pc -= 2;
}

string OpLdVxK::disassemble (const uint16_t opcode) {
    return "ld v" + std::to_string ((int) getX (opcode)) + " k";
}

void OpLdDtVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->delay_timer = cpu->reg[getX (opcode)];
}

string OpLdDtVx::disassemble (const uint16_t opcode) {
    return "ld dt v" + std::to_string ((int) getX (opcode));
}

void OpLdStVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->sound_timer = cpu->reg[getX (opcode)];
}

string OpLdStVx::disassemble (const uint16_t opcode) { 
    return "ld st v" + std::to_string ((int) getX (opcode));
}

void OpAddIVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->I = cpu->I + cpu->reg[getX (opcode)];
}

string OpAddIVx::disassemble (const uint16_t opcode) { 
    return "add I v" + std::to_string ((int) getX (opcode));
}

void OpLdFVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->I = cpu->reg[getX (opcode)] * 0x5;
}

string OpLdFVx::disassemble (const uint16_t opcode) { 
    return "ld F v" + std::to_string ((int) getX (opcode));
}

void OpLdBVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    uint16_t x = getX (opcode);
    cpu->memory[cpu->I] = cpu->reg[x] / 100;
    cpu->memory[cpu->I + 1] = (cpu->reg[x] / 10) % 10;
    cpu->memory[cpu->I + 2] = (cpu->reg[x] % 10);
}

string OpLdBVx::disassemble (const uint16_t opcode) { 
    return "ld B v" + std::to_string ((int) getX (opcode));
}

void OpLdIVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    for (uint16_t i = 0; i <= getX (opcode); i++)
	cpu->memory[cpu->I + i] = cpu->reg[i];
    // I += ((opcode & 0x0F00) >> 8) + 1; ??
}

string OpLdIVx::disassemble (const uint16_t opcode) { 
    return "ld I v" + std::to_string ((int) getX (opcode));
}

void OpLdVxI::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    for (uint16_t i = 0; i <= getX (opcode); i++)
	cpu->reg[i] = cpu->memory[cpu->I + i];
    // I += ((opcode & 0x0F00) >> 8) + 1; ??
}

string OpLdVxI::disassemble (const uint16_t opcode) { 
    return "ld v" + std::to_string ((int) getX (opcode)) + " I";
}

void OpSys::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
}

string OpSys::disassemble (const uint16_t opcode) { 
    return "sys";
}

void OpDown::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {}

string OpDown::disassemble (const uint16_t opcode) { 
    return "down";
}

void OpRight::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {}

string OpRight::disassemble (const uint16_t opcode) { 
    return "right";
}

void OpLeft::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    
}

string OpLeft::disassemble (const uint16_t opcode) { 
    return "left";
}

void OpLow::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cout << "[CPU] Low operation not implemented !" << endl;
}

string OpLow::disassemble (const uint16_t opcode) { 
    return "low";
}

void OpHigh::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cout << "[CPU] High operation not implemented !" << endl;
}

string OpHigh::disassemble (const uint16_t opcode) { 
    return "high";
}

void OpExit::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->shutdown ();
}

string OpExit::disassemble (const uint16_t opcode) { 
    return "exit";
}

void OpLdHFVx::execute (const uint16_t opcode, Cpu * cpu, Screen * screen) {
    cpu->I = (cpu->reg[getX (opcode)] * 10) + (16 * 5);
}

string OpLdHFVx::disassemble (const uint16_t opcode) { 
    return "ld HF v" + std::to_string (getX (opcode));
}

