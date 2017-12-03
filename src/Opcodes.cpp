#include "Cpu.hpp"
#include "Chip8.hpp"

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

Opcodes::Opcodes() {}

Opcodes * Opcodes::instance() {
	if (inst == NULL)
		inst = new Opcodes();
	return inst;
}

map<uint16_t, Opcode*> * Opcodes::getList() {
	return &opcodes_list;
}

uint16_t Opcode::getX(const uint16_t opcode) const {
	return (opcode & 0x0F00) >> 8;
}

uint16_t Opcode::getY(const uint16_t opcode) const {
	return (opcode & 0x00F0) >> 4;
}

uint16_t Opcode::getZ(const uint16_t opcode) const {
	return (opcode & 0x000F);
}

uint16_t Opcode::getData(const uint16_t opcode) const {
	return (opcode & 0x0FFF);
}

void OpCls::execute(const uint16_t opcode, Chip8 * chip) {
	chip->screen->clear();
}

string OpCls::disassemble(const uint16_t opcode) {
	return "cls";
}

void OpRet::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->sp--;
	chip->cpu->pc = chip->cpu->stack[chip->cpu->sp];
}

string OpRet::disassemble(const uint16_t opcode) {
	return "ret";
}

void OpJpAddr::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->pc = opcode & 0x0FFF;
	chip->cpu->pc -= 2;
}

string OpJpAddr::disassemble(const uint16_t opcode) {
	return "jp " + Utils::htos(opcode & 0x0FFF);
}

void OpCallAddr::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->stack[chip->cpu->sp] = chip->cpu->pc;
	chip->cpu->sp++;
	chip->cpu->pc = opcode & 0x0FFF;
	chip->cpu->pc -= 2;
}

string OpCallAddr::disassemble(const uint16_t opcode) {
	return "call " + Utils::htos(opcode & 0x0FFF);
}

void OpSeVxByte::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->reg[getX(opcode)] == (opcode & 0x00FF))
		chip->cpu->pc += 2;
}

string OpSeVxByte::disassemble(const uint16_t opcode) {
	return "se v" + std::to_string((int)getX(opcode)) + " " + std::to_string((int)(opcode & 0x00FF));
}

void OpSneVxByte::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->reg[getX(opcode)] != (opcode & 0x00FF))
		chip->cpu->pc += 2;
}

string OpSneVxByte::disassemble(const uint16_t opcode) {
	return "sne v" + std::to_string((int)getX(opcode)) + " " + std::to_string((int)(opcode & 0x00FF));
}

void OpSeVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->reg[getX(opcode)] == chip->cpu->reg[getY(opcode)])
		chip->cpu->pc += 2;
}

string OpSeVxVy::disassemble(const uint16_t opcode) {
	return "se v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpLdVxByte::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] = (opcode & 0x00FF);
}

string OpLdVxByte::disassemble(const uint16_t opcode) {
	return "ld v" + std::to_string((int)getX(opcode)) + " " + std::to_string((int)(opcode & 0x00FF));
}

void OpAddVxByte::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] = chip->cpu->reg[getX(opcode)] + (opcode & 0x00FF);
}

string OpAddVxByte::disassemble(const uint16_t opcode) {
	return "add v" + std::to_string((int)getX(opcode)) + " " + std::to_string((int)(opcode & 0x00FF));
}

void OpLdVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] = chip->cpu->reg[getY(opcode)];
}

string OpLdVxVy::disassemble(const uint16_t opcode) {
	return "ld v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpOrVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] |= chip->cpu->reg[getY(opcode)];
}

string OpOrVxVy::disassemble(const uint16_t opcode) {
	return "or v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpAndVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] &= chip->cpu->reg[getY(opcode)];
}

string OpAndVxVy::disassemble(const uint16_t opcode) {
	return "and v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}
void OpXorVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] ^= chip->cpu->reg[getY(opcode)];
}

string OpXorVxVy::disassemble(const uint16_t opcode) {
	return "xor v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpAddVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	uint16_t y = getY(opcode);
	uint16_t sum = chip->cpu->reg[x] + chip->cpu->reg[y];
	if (sum > 255) {
		chip->cpu->reg[0xF] = 1;
	}
	else {
		chip->cpu->reg[0xF] = 0;
	}
	chip->cpu->reg[x] = sum & 0x00FF;
}

string OpAddVxVy::disassemble(const uint16_t opcode) {
	return "add v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpSubVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	uint16_t y = getY(opcode);
	if (chip->cpu->reg[x] > chip->cpu->reg[y]) {
		chip->cpu->reg[0xF] = 1;
	}
	else {
		chip->cpu->reg[0xF] = 0;
	}
	chip->cpu->reg[x] = chip->cpu->reg[x] - chip->cpu->reg[y];
}

string OpSubVxVy::disassemble(const uint16_t opcode) {
	return "sub v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpShrVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	uint16_t y = getY(opcode);
	if ((chip->cpu->reg[x] & 0x000F) == 1) {
		chip->cpu->reg[0xF] = 1;
	}
	else {
		chip->cpu->reg[0xF] = 0;
	}
	chip->cpu->reg[x] = chip->cpu->reg[x] / 2;
}

string OpShrVxVy::disassemble(const uint16_t opcode) {
	return "shr v" + std::to_string((int)getX(opcode));
}

void OpSubnVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	uint16_t y = getY(opcode);
	if (chip->cpu->reg[y] > chip->cpu->reg[x]) {
		chip->cpu->reg[0xF] = 1;
	}
	else {
		chip->cpu->reg[0xF] = 0;
	}
	chip->cpu->reg[x] = chip->cpu->reg[y] - chip->cpu->reg[x];
}

string OpSubnVxVy::disassemble(const uint16_t opcode) {
	return "subn v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpShlVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	chip->cpu->reg[0xF] = (chip->cpu->reg[x] & 0x80) >> 7;
	chip->cpu->reg[x] <<= 1;
}

string OpShlVxVy::disassemble(const uint16_t opcode) {
	return "shl v" + std::to_string((int)getX(opcode));
}
void OpSneVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->reg[getX(opcode)] != chip->cpu->reg[getY(opcode)])
		chip->cpu->pc += 2;
}

string OpSneVxVy::disassemble(const uint16_t opcode) {
	return "sne v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpLdIAddr::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->I = opcode & 0x0FFF;
}

string OpLdIAddr::disassemble(const uint16_t opcode) {
	return "ld I " + std::to_string((int)(opcode & 0x0FFF));
}

void OpJpV0Addr::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->pc = (opcode & 0x0FFF) + chip->cpu->reg[0];
	chip->cpu->pc -= 2; //because of the +=2 in the cpu's code
}

string OpJpV0Addr::disassemble(const uint16_t opcode) {
	return "jp v0 " + std::to_string((int)(opcode & 0x0FFF));
}

void OpRndVxByte::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] = (rand() % 255 + 1) & (opcode & 0x00FF);
}

string OpRndVxByte::disassemble(const uint16_t opcode) {
	return "rnd v" + std::to_string((int)getX(opcode)) + " " + std::to_string((int)(opcode & 0x00FF));
}

void OpDrwVxVy::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	uint16_t y = getY(opcode);
	uint16_t height = opcode & 0x000F;
	uint16_t pixel;

	chip->cpu->reg[0xF] = 0;
	for (int i = 0; i < height; i++) {
		pixel = chip->cpu->memory[chip->cpu->I + i];
		for (int j = 0; j < 8; j++) {
			if ((pixel & (0x80 >> j)) != 0) {
				if (chip->gameScreen->getPixel(chip->cpu->reg[x] + j, chip->cpu->reg[y] + i).color == WHITE) {
					chip->cpu->reg[0xF] = 1;
					chip->gameScreen->setColor(chip->cpu->reg[x] + j, chip->cpu->reg[y] + i, BLACK);
				}
				else {
					chip->gameScreen->setColor(chip->cpu->reg[x] + j, chip->cpu->reg[y] + i, WHITE);
				}
			}
		}
	}
}

string OpDrwVxVy::disassemble(const uint16_t opcode) {
	return "drw v" + std::to_string((int)getX(opcode)) + " v" + std::to_string((int)getY(opcode));
}

void OpSkpVx::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->keyboard[chip->cpu->reg[getX(opcode)]] == 1)
		chip->cpu->pc += 2;
}

string OpSkpVx::disassemble(const uint16_t opcode) {
	return "skp v" + std::to_string((int)getX(opcode));
}

void OpSknpVx::execute(const uint16_t opcode, Chip8 * chip) {
	if (chip->cpu->keyboard[chip->cpu->reg[getX(opcode)]] == 0)
		chip->cpu->pc += 2;
}

string OpSknpVx::disassemble(const uint16_t opcode) {
	return "sknp v" + std::to_string((int)getX(opcode));
}

void OpLdVxDt::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->reg[getX(opcode)] = chip->cpu->delay_timer;
}

string OpLdVxDt::disassemble(const uint16_t opcode) {
	return "ld v" + std::to_string((int)getX(opcode)) + " dt";
}

void OpLdVxK::execute(const uint16_t opcode, Chip8 * chip) {
	bool found = false;
	for (int i = 0; i < KEYBOARD_SIZE; i++) {
		if (chip->cpu->keyboard[i] == KEY_DOWN) {
			chip->cpu->reg[getX(opcode)] = i;
			found = true;
		}
	}
	if (!found)
		chip->cpu->pc -= 2;
}

string OpLdVxK::disassemble(const uint16_t opcode) {
	return "ld v" + std::to_string((int)getX(opcode)) + " k";
}

void OpLdDtVx::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->delay_timer = chip->cpu->reg[getX(opcode)];
}

string OpLdDtVx::disassemble(const uint16_t opcode) {
	return "ld dt v" + std::to_string((int)getX(opcode));
}

void OpLdStVx::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->sound_timer = chip->cpu->reg[getX(opcode)];
}

string OpLdStVx::disassemble(const uint16_t opcode) {
	return "ld st v" + std::to_string((int)getX(opcode));
}

void OpAddIVx::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->I = chip->cpu->I + chip->cpu->reg[getX(opcode)];
}

string OpAddIVx::disassemble(const uint16_t opcode) {
	return "add I v" + std::to_string((int)getX(opcode));
}

void OpLdFVx::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->I = chip->cpu->reg[getX(opcode)] * 0x5;
}

string OpLdFVx::disassemble(const uint16_t opcode) {
	return "ld F v" + std::to_string((int)getX(opcode));
}

void OpLdBVx::execute(const uint16_t opcode, Chip8 * chip) {
	uint16_t x = getX(opcode);
	chip->cpu->memory[chip->cpu->I] = chip->cpu->reg[x] / 100;
	chip->cpu->memory[chip->cpu->I + 1] = (chip->cpu->reg[x] / 10) % 10;
	chip->cpu->memory[chip->cpu->I + 2] = (chip->cpu->reg[x] % 10);
}

string OpLdBVx::disassemble(const uint16_t opcode) {
	return "ld B v" + std::to_string((int)getX(opcode));
}

void OpLdIVx::execute(const uint16_t opcode, Chip8 * chip) {
	for (uint16_t i = 0; i <= getX(opcode); i++)
		chip->cpu->memory[chip->cpu->I + i] = chip->cpu->reg[i];
	// I += ((opcode & 0x0F00) >> 8) + 1; ??
}

string OpLdIVx::disassemble(const uint16_t opcode) {
	return "ld I v" + std::to_string((int)getX(opcode));
}

void OpLdVxI::execute(const uint16_t opcode, Chip8 * chip) {
	for (uint16_t i = 0; i <= getX(opcode); i++)
		chip->cpu->reg[i] = chip->cpu->memory[chip->cpu->I + i];
	// I += ((opcode & 0x0F00) >> 8) + 1; ??
}

string OpLdVxI::disassemble(const uint16_t opcode) {
	return "ld v" + std::to_string((int)getX(opcode)) + " I";
}

void OpSys::execute(const uint16_t opcode, Chip8 * chip) {
}

string OpSys::disassemble(const uint16_t opcode) {
	return "sys";
}

void OpDown::execute(const uint16_t opcode, Chip8 * chip) {}

string OpDown::disassemble(const uint16_t opcode) {
	return "down";
}

void OpRight::execute(const uint16_t opcode, Chip8 * chip) {}

string OpRight::disassemble(const uint16_t opcode) {
	return "right";
}

void OpLeft::execute(const uint16_t opcode, Chip8 * chip) {

}

string OpLeft::disassemble(const uint16_t opcode) {
	return "left";
}

void OpLow::execute(const uint16_t opcode, Chip8 * chip) {
	cout << "[CPU] Low operation not implemented !" << endl;
}

string OpLow::disassemble(const uint16_t opcode) {
	return "low";
}

void OpHigh::execute(const uint16_t opcode, Chip8 * chip) {
	cout << "[CPU] High operation not implemented !" << endl;
}

string OpHigh::disassemble(const uint16_t opcode) {
	return "high";
}

void OpExit::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->shutdown();
}

string OpExit::disassemble(const uint16_t opcode) {
	return "exit";
}

void OpLdHFVx::execute(const uint16_t opcode, Chip8 * chip) {
	chip->cpu->I = (chip->cpu->reg[getX(opcode)] * 10) + (16 * 5);
}

string OpLdHFVx::disassemble(const uint16_t opcode) {
	return "ld HF v" + std::to_string(getX(opcode));
}

