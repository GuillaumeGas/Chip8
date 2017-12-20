#include "Cpu.hpp"
#include "Chip8.hpp"

using namespace std;

Cpu::Cpu(GameScreen * screen) 
{
	cout << "> CPU is starting..." << endl;

	_init();

	this->screen = screen;

	srand((unsigned int)(time(NULL)));
	loadFont();
}

void Cpu::_init()
{
	this->running = true;

	memset(this->memory, 0, MEM_SIZE);
	memset(this->reg, 0, V_REGISTERS_SIZE);
	memset(this->rpl, 0, RPL_SIZE);
	memset(this->stack, 0, STACK_SIZE);
	memset(this->keyboard, 0, KEYBOARD_SIZE);

	this->pc = START_ADDRESS;
	this->I = 0;
	this->sp = 0;
	this->delay_timer = 0;
	this->sound_timer = 0;
}

bool Cpu::emulateCycle() 
{
	if ((this->pc >= (MEM_SIZE - 1) && this->pc < _romSize) || !this->running)
		return false;

	execOpcode(getNextOpCode());
	this->pc += 2;
	count();
	return true;
}

uint16_t Cpu::getNextOpCode() 
{
	return (this->memory[this->pc] << 8) | this->memory[this->pc + 1];
}

void Cpu::count() 
{
	if (this->delay_timer > 0)
		this->delay_timer--;
	if (this->sound_timer > 0)
		this->sound_timer--;
}

void Cpu::loadProgram(const char * file_name) 
{
	_init();
	cout << "> Loading binary file..." << endl;
	FILE * game = NULL;
	fopen_s(&game, file_name, "rb");

	if (game != NULL) {
		fseek(game, 0, SEEK_END);
		_romSize = ftell(game);
		rewind(game);

		fread(&this->memory[START_ADDRESS], sizeof(uint8_t) * (MEM_SIZE - START_ADDRESS), 1, game);
		fclose(game);
	}
	else {
		throw LoadFileError(file_name);
	}
}

bool Cpu::isRunning() const 
{
	return this->running;
}

void Cpu::loadFont() 
{
	this->memory[0] = 0xF0; this->memory[1] = 0x90; this->memory[2] = 0x90; this->memory[3] = 0x90; this->memory[4] = 0xF0;      // O 
	this->memory[5] = 0x20; this->memory[6] = 0x60; this->memory[7] = 0x20; this->memory[8] = 0x20; this->memory[9] = 0x70;       // 1 
	this->memory[10] = 0xF0; this->memory[11] = 0x10; this->memory[12] = 0xF0; this->memory[13] = 0x80; this->memory[14] = 0xF0; // 2 
	this->memory[15] = 0xF0; this->memory[16] = 0x10; this->memory[17] = 0xF0; this->memory[18] = 0x10; this->memory[19] = 0xF0;  // 3 
	this->memory[20] = 0x90; this->memory[21] = 0x90; this->memory[22] = 0xF0; this->memory[23] = 0x10; this->memory[24] = 0x10;  // 4 
	this->memory[25] = 0xF0; this->memory[26] = 0x80; this->memory[27] = 0xF0; this->memory[28] = 0x10; this->memory[29] = 0xF0;  // 5 
	this->memory[30] = 0xF0; this->memory[31] = 0x80; this->memory[32] = 0xF0; this->memory[33] = 0x90; this->memory[34] = 0xF0;  // 6 
	this->memory[35] = 0xF0; this->memory[36] = 0x10; this->memory[37] = 0x20; this->memory[38] = 0x40; this->memory[39] = 0x40;  // 7 
	this->memory[40] = 0xF0; this->memory[41] = 0x90; this->memory[42] = 0xF0; this->memory[43] = 0x90; this->memory[44] = 0xF0;  // 8 
	this->memory[45] = 0xF0; this->memory[46] = 0x90; this->memory[47] = 0xF0; this->memory[48] = 0x10; this->memory[49] = 0xF0;  // 9 
	this->memory[50] = 0xF0; this->memory[51] = 0x90; this->memory[52] = 0xF0; this->memory[53] = 0x90; this->memory[54] = 0x90;  // A 
	this->memory[55] = 0xE0; this->memory[56] = 0x90; this->memory[57] = 0xE0; this->memory[58] = 0x90; this->memory[59] = 0xE0;  // B 
	this->memory[60] = 0xF0; this->memory[61] = 0x80; this->memory[62] = 0x80; this->memory[63] = 0x80; this->memory[64] = 0xF0;  // C 
	this->memory[65] = 0xE0; this->memory[66] = 0x90; this->memory[67] = 0x90; this->memory[68] = 0x90; this->memory[69] = 0xE0;  // D 
	this->memory[70] = 0xF0; this->memory[71] = 0x80; this->memory[72] = 0xF0; this->memory[73] = 0x80; this->memory[74] = 0xF0;  // E 
	this->memory[75] = 0xF0; this->memory[76] = 0x80; this->memory[77] = 0xF0; this->memory[78] = 0x80; this->memory[79] = 0x80;  // F 
}

void Cpu::execOpcode(const uint16_t opcode) 
{
	uint16_t opcode_id = GetOpcodeId(opcode);;
	auto opcodes = Opcodes::instance()->getList();
	(*opcodes)[opcode_id]->execute(opcode, this, screen);
}

void Cpu::shutdown() 
{
	this->running = false;
}

uint16_t Cpu::GetOpcodeId(const uint16_t opcode) 
{
	switch (opcode & 0xF000) 
	{
	case 0x0:
		switch (opcode) {
		case 0x00E0:
			/* CLS */
			return 0x00E0;
			break;
		case 0x00EE:
			/* RET */
			return 0x00EE;
			break;
		case 0x00FB:
			/* SCRR */
			return 0x00FB;
			break;
		case 0x00FC:
			/* SCRL */
			return 0x00FC;
			break;
		case 0x00FD:
			/* EXIT */
			return 0x00FD;
			break;
		case 0x00FE:
			/* MODE 1 */
			return 0x00FE;
			break;
		case 0x00FF:
			/* MODE 2 */
			return 0x00FF;
			break;
		default:
			if ((opcode & 0x0FFF) & 0x0C0) 
			{
				/* SCD */
				return 0x00C0;
			}
			else 
			{
				throw OpcodeNotFound(opcode, this->pc);
			}
		}
		break;
	case 0x1000:
		/* JP addr */
		return 0x1000;
		break;
	case 0x2000:
		/* CALL addr */
		return 0x2000;
		break;
	case 0x3000:
		/* SE Vx, byte */
		return 0x3000;
		break;
	case 0x4000:
		/* SNE Vx, byte */
		return 0x4000;
		break;
	case 0x5000:
		/* SE Vx, Vy */
		return 0x5000;
		break;
	case 0x6000:
		/* LD Vx, byte */
		return 0x6000;
		break;
	case 0x7000:
		/* ADD Vx, byte */
		return 0x7000;
		break;
	case 0x8000:
		switch (opcode & 0x000F) 
		{
		case 0x0:
			/* LD Vx, Vy */
			return 0x8000;
			break;
		case 0x1:
			/* OR Vx, Vy */
			return 0x8001;
			break;
		case 0x2:
			/* AND Vx, Vy */
			return 0x8002;
			break;
		case 0x3:
			/* XOR Vx, Vy */
			return 0x8003;
			break;
		case 0x4:
			/* ADD Vx, Vy */
			return 0x8004;
			break;
		case 0x5:
			/* SUB Vx, Vy */
			return 0x8005;
			break;
		case 0x6:
			/* SHR Vx, Vy */
			return 0x8006;
			break;
		case 0x7:
			/* SUBN Vx, Vy */
			return 0x8007;
			break;
		case 0xE:
			/* SHL Vx, Vy */
			return 0x800E;
			break;
		default:
			throw OpcodeNotFound(opcode, this->pc);
		}
		break;
	case 0x9000:
		/* SNE Vx, Vy */
		return 0x9000;
		break;
	case 0xA000:
		/* LD I, addr */
		return 0xA000;
		break;
	case 0xB000:
		/* JP V0, addr */
		return 0xB000;
		break;
	case 0xC000:
		/* RND Vx, byte */
		return 0xC000;
		break;
	case 0xD000:
		/* DRW Vx, Vy */
		return 0xD000;
		break;
	case 0xE000:
		switch (opcode & 0x00FF) 
		{
		case 0x9E:
			/* SKP Vx */
			return 0xE09E;
			break;
		case 0xA1:
			/* SKNP Vx */
			return 0xE0A1;
			break;
		default:
			throw OpcodeNotFound(opcode, this->pc);
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF) 
		{
		case 0x07:
			/* LD Vx, DT */
			return 0xF007;
			break;
		case 0x0A:
			/* LD Vx, K */
			return 0xF00A;
			break;
		case 0x15:
			/* LD DT, Vx */
			return 0xF015;
			break;
		case 0x18:
			/* LD ST, Vx */
			return 0xF018;
			break;
		case 0x1E:
			/* ADD I, Vx */
			return 0xF01E;
			break;
		case 0x29:
			/* LD F, Vx */
			return 0xF029;
			break;
		case 0x30:
			/* LD HF, Vx */
			return 0xF030;
			break;
		case 0x33:
			/* LD B, Vx */
			return 0xF033;
			break;
		case 0x55:
			/* LD [I] Vx */
			return 0xF055;
			break;
		case 0x65:
			/* LD Vx, [I] */
			return 0xF065;
			break;
		case 0x75:
			/* LD R, Vx */
			return 0xF075;
			break;
		case 0x85:
			/* LD Vx, R */
			return 0xF075;
			break;
		default:
			throw OpcodeNotFound(opcode, this->pc);
		}
		break;
	default:
		throw OpcodeNotFound(opcode, this->pc);
	}
}
