#include "Cpu.hpp"
#include "Chip8.hpp"

using namespace std;

Cpu::Cpu (Chip8 * ctrl, Screen * sc, bool debug) {
    this->debug = debug;
    this->started = true;

    if (debug) {
	cout << "> CPU starting on debug mode..." << endl;
    } else {
	cout << "> CPU starting..." << endl;
    }
    memset (this->memory, 0, MEM_SIZE);
    memset (this->reg, 0, V_REGISTERS_SIZE);
    memset (this->stack, 0, STACK_SIZE);
    memset (this->keyboard, 0, KEYBOARD_SIZE);

    this->pc = START_ADDRESS;
    this->I = 0;
    this->sp = 0;
    this->delay_timer = 0;
    this->sound_timer = 0;

    this->ctrl = ctrl;
    this->screen = sc;

    srand (time (NULL));
    this->loadFont ();
}

bool Cpu::emulateCycle () {
    if (this->pc >= (MEM_SIZE - 1))
	return false;

    this->exec_opcode (getNextOpCode ());
    this->pc += 2;
    this->count ();
}

void Cpu::loadProgram (const char * file_name) {
    cout << "> Loading binary file..." << endl;
    FILE * game = NULL;
    game = fopen (file_name, "rb");

    if (game != NULL) {
	fread (&this->memory[START_ADDRESS], sizeof (Uint8) * (MEM_SIZE - START_ADDRESS), 1, game);
	fclose (game);
    } else {
	cout << "Error, can't read program." << endl;
	throw -1;
    }
}

Uint16 Cpu::getNextOpCode () {
    return (this->memory[this->pc] << 8) | this->memory[this->pc + 1];
}

void Cpu::count () {
    if (this->delay_timer > 0)
	this->delay_timer--;
    if (this->sound_timer > 0)
	this->sound_timer--;
}

void Cpu::dump () const {
    cout << "** Register **" << endl;
    cout << "PC : " << this->pc << endl;
    cout << "I : " << this->I << endl;
    cout << "SP : " << (int) this->sp << endl;
    cout << "Delay counter : " << (int) this->delay_timer << endl;
    cout << "Sound counter : " << (int) this->sound_timer << endl;
    printf ("Current instruction : %#04x\n", this->memory[this->pc]);
    for (int i = 0; i < V_REGISTERS_SIZE; i++)
	cout << "V" << i << " = " << (int) this->reg[i] << endl;
}

void Cpu::exec_opcode (const Uint16 opcode) {
    for (int i = 0; i < NB_MASK; i++) {
	auto it = Opcodes::instance ()->getList ()->find (opcode & mask[i]);

	if (it != Opcodes::instance ()->getList ()->end ()) {
	    if (this->debug)
		this->debug_inst (opcode, it->second);
	    it->second->execute (opcode, this, this->screen);
	    break;
	}
    }
}

void Cpu::loadFont() { 
    this->memory[0]=0xF0;this->memory[1]=0x90;this->memory[2]=0x90;this->memory[3]=0x90; this->memory[4]=0xF0;      // O 
    this->memory[5]=0x20;this->memory[6]=0x60;this->memory[7]=0x20;this->memory[8]=0x20;this->memory[9]=0x70;       // 1 
    this->memory[10]=0xF0;this->memory[11]=0x10;this->memory[12]=0xF0;this->memory[13]=0x80; this->memory[14]=0xF0; // 2 
    this->memory[15]=0xF0;this->memory[16]=0x10;this->memory[17]=0xF0;this->memory[18]=0x10;this->memory[19]=0xF0;  // 3 
    this->memory[20]=0x90;this->memory[21]=0x90;this->memory[22]=0xF0;this->memory[23]=0x10;this->memory[24]=0x10;  // 4 
    this->memory[25]=0xF0;this->memory[26]=0x80;this->memory[27]=0xF0;this->memory[28]=0x10;this->memory[29]=0xF0;  // 5 
    this->memory[30]=0xF0;this->memory[31]=0x80;this->memory[32]=0xF0;this->memory[33]=0x90;this->memory[34]=0xF0;  // 6 
    this->memory[35]=0xF0;this->memory[36]=0x10;this->memory[37]=0x20;this->memory[38]=0x40;this->memory[39]=0x40;  // 7 
    this->memory[40]=0xF0;this->memory[41]=0x90;this->memory[42]=0xF0;this->memory[43]=0x90;this->memory[44]=0xF0;  // 8 
    this->memory[45]=0xF0;this->memory[46]=0x90;this->memory[47]=0xF0;this->memory[48]=0x10;this->memory[49]=0xF0;  // 9 
    this->memory[50]=0xF0;this->memory[51]=0x90;this->memory[52]=0xF0;this->memory[53]=0x90;this->memory[54]=0x90;  // A 
    this->memory[55]=0xE0;this->memory[56]=0x90;this->memory[57]=0xE0;this->memory[58]=0x90;this->memory[59]=0xE0;  // B 
    this->memory[60]=0xF0;this->memory[61]=0x80;this->memory[62]=0x80;this->memory[63]=0x80;this->memory[64]=0xF0;  // C 
    this->memory[65]=0xE0;this->memory[66]=0x90;this->memory[67]=0x90;this->memory[68]=0x90;this->memory[69]=0xE0;  // D 
    this->memory[70]=0xF0;this->memory[71]=0x80;this->memory[72]=0xF0;this->memory[73]=0x80;this->memory[74]=0xF0;  // E 
    this->memory[75]=0xF0;this->memory[76]=0x80;this->memory[77]=0xF0;this->memory[78]=0x80;this->memory[79]=0x80;  // F 
}

void Cpu::debug_inst (uint16_t opcode, Opcode * op) {
    cout << "[" << this->pc << "] " << op->disassemble (opcode) << endl;

    char rep = -1;
    while (rep != 'c') {
	cout << " >> ";
	cin >> rep;
	if (rep == 'p') {
	    this->dump ();
	}
    }
}
