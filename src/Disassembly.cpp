#include "Disassembly.hpp"

using namespace std;

Disassembly::Disassembly(const char * file_name) {
	this->loadFile(file_name);
}

void Disassembly::disass() {
	int offset = 512;
	uint16_t opcode;
	int i = 0;

	while (i < (BUF_SIZE - 1) && i < _romSize) {
		opcode = (this->_buffer[i] << 8) | this->_buffer[i + 1];
		if (opcode != 0) {
			opcode = (this->_buffer[i] << 8) | this->_buffer[i + 1];
			this->_disass(opcode, offset);
		}
		offset += 2;
		i += 2;
	}
}

void Disassembly::_disass(const uint16_t opcode, const int offset) {
	uint16_t opcode_id = 0;

	switch (opcode & 0xF000) {
	case 0x0:
		switch (opcode) {
		case 0x00E0:
			/* CLS */
			opcode_id = 0x00E0;
			break;
		case 0x00EE:
			/* RET */
			opcode_id = 0x00EE;
			break;
		case 0x00FB:
			/* SCRR */
			opcode_id = 0x00FB;
			break;
		case 0x00FC:
			/* SCRL */
			opcode_id = 0x00FC;
			break;
		case 0x00FD:
			/* EXIT */
			opcode_id = 0x00FD;
			break;
		case 0x00FE:
			/* MODE 1 */
			opcode_id = 0x00FE;
			break;
		case 0x00FF:
			/* MODE 2 */
			opcode_id = 0x00FF;
			break;
		default:
			if ((opcode & 0x0FFF) & 0x0C0) {
				/* SCD */
				opcode_id = 0x00C0;
			}
		}
		break;
	case 0x1000:
		/* JP addr */
		opcode_id = 0x1000;
		break;
	case 0x2000:
		/* CALL addr */
		opcode_id = 0x2000;
		break;
	case 0x3000:
		/* SE Vx, byte */
		opcode_id = 0x3000;
		break;
	case 0x4000:
		/* SNE Vx, byte */
		opcode_id = 0x4000;
		break;
	case 0x5000:
		/* SE Vx, Vy */
		opcode_id = 0x5000;
		break;
	case 0x6000:
		/* LD Vx, byte */
		opcode_id = 0x6000;
		break;
	case 0x7000:
		/* ADD Vx, byte */
		opcode_id = 0x7000;
		break;
	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0:
			/* LD Vx, Vy */
			opcode_id = 0x8000;
			break;
		case 0x1:
			/* OR Vx, Vy */
			opcode_id = 0x8001;
			break;
		case 0x2:
			/* AND Vx, Vy */
			opcode_id = 0x8002;
			break;
		case 0x3:
			/* XOR Vx, Vy */
			opcode_id = 0x8003;
			break;
		case 0x4:
			/* ADD Vx, Vy */
			opcode_id = 0x8004;
			break;
		case 0x5:
			/* SUB Vx, Vy */
			opcode_id = 0x8005;
			break;
		case 0x6:
			/* SHR Vx, Vy */
			opcode_id = 0x8006;
			break;
		case 0x7:
			/* SUBN Vx, Vy */
			opcode_id = 0x8007;
			break;
		case 0xE:
			/* SHL Vx, Vy */
			opcode_id = 0x800E;
			break;
		}
		break;
	case 0x9000:
		/* SNE Vx, Vy */
		opcode_id = 0x9000;
		break;
	case 0xA000:
		/* LD I, addr */
		opcode_id = 0xA000;
		break;
	case 0xB000:
		/* JP V0, addr */
		opcode_id = 0xB000;
		break;
	case 0xC000:
		/* RND Vx, byte */
		opcode_id = 0xC000;
		break;
	case 0xD000:
		/* DRW Vx, Vy */
		opcode_id = 0xD000;
		break;
	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x9E:
			/* SKP Vx */
			opcode_id = 0xE09E;
			break;
		case 0xA1:
			/* SKNP Vx */
			opcode_id = 0xE0A1;
			break;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x07:
			/* LD Vx, DT */
			opcode_id = 0xF007;
			break;
		case 0x0A:
			/* LD Vx, K */
			opcode_id = 0xF00A;
			break;
		case 0x15:
			/* LD DT, Vx */
			opcode_id = 0xF015;
			break;
		case 0x18:
			/* LD ST, Vx */
			opcode_id = 0xF018;
			break;
		case 0x1E:
			/* ADD I, Vx */
			opcode_id = 0xF01E;
			break;
		case 0x29:
			/* LD F, Vx */
			opcode_id = 0xF029;
			break;
		case 0x30:
			/* LD HF, Vx */
			opcode_id = 0xF030;
			break;
		case 0x33:
			/* LD B, Vx */
			opcode_id = 0xF033;
			break;
		case 0x55:
			/* LD [I] Vx */
			opcode_id = 0xF055;
			break;
		case 0x65:
			/* LD Vx, [I] */
			opcode_id = 0xF065;
			break;
		case 0x75:
			/* LD R, Vx */
			opcode_id = 0xF075;
			break;
		case 0x85:
			/* LD Vx, R */
			opcode_id = 0xF075;
			break;
		}
		break;
	}

	if (opcode != 0) {
		auto opcodes = Opcodes::instance()->getList();
		cout << "[" << offset << "] "
			<< std::showbase << std::internal << std::setfill('0')
			<< std::hex << std::setw(6) << opcode
			<< std::dec << " " << (*opcodes)[opcode_id]->disassemble(opcode) << endl;
	}
	else {
		cout << "[" << offset << "] unknown opcode" << endl;
	}
}

void Disassembly::loadFile(const char * file_name) {
	cout << "> Loading binary file..." << endl;
	FILE * game = NULL;
	fopen_s(&game, file_name, "rb");

	if (game != NULL) {
		fseek(game, 0, SEEK_END);
		_romSize = ftell(game);
		rewind(game);

		fread(&this->_buffer, sizeof(uint8_t) * BUF_SIZE, 1, game);
		fclose(game);
	}
	else {
		throw LoadFileError(file_name);
	}
}
