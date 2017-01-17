#include "Cpu.hpp"
#include "Screen.hpp"

using namespace std;

Cpu::Cpu (Screen * sc) {
    memset (_memory, 0, MEM_SIZE);
    memset (_v_registers, 0, V_REGISTERS_SIZE);
    memset (_stack, 0, STACK_SIZE);

    _pc = START_ADDRESS;
    _I = 0;
    _sp = 0;
    _delay_counter = 0;
    _sound_counter = 0;

    _screen = sc;
}

void Cpu::start () {
    bool stop = false;

    do {
	Uint16 opcode = _getNextOpCode ();
	_pc += 2;
	_exec_opcode (opcode);

	_screen->update ();
	SDL_Delay (FPS);
    } while (!stop);
}

Uint16 Cpu::_getNextOpCode () {
    return (_memory[_pc] << 8) + _memory[_pc + 1];
}

void Cpu::_count () {
    if (_delay_counter > 0)
	_delay_counter--;
    if (_sound_counter > 0)
	_sound_counter--;
}

Uint8 * Cpu::_getRegV (unsigned int i) {
    if (i < 1 || i > 16)
	throw -1;

    return &(_v_registers [i - 1]);
}

void Cpu::print () const {
    cout << "** Register **" << endl;
    cout << "PC : " << _pc << endl;
    cout << "I : " << _I << endl;
    cout << "SP : " << _sp << endl;
    cout << "Delay counter : " << _delay_counter << endl;
    cout << "Sound counter : " << _sound_counter << endl;
}

void Cpu::_exec_opcode (const Uint16 opcode) {
    for (int i = 0; i < NB_OPCODES; i++) {
	opcode_t op = _opcode_list[i];
	if (opcode & op.mask == op.id) {
	    ((*this).*(op.fun_ptr)) (opcode);
	    break;
	}
    }
}

void Cpu::_op_sys (const Uint16 opcode);
void Cpu::_op_cls (const Uint16 opcode) {
    _screen->clear ();
}
void Cpu::_op_ret (const Uint16 opcode);
void Cpu::_op_jp_addr (const Uint16 opcode);
void Cpu::_op_call (const Uint16 opcode);
void Cpu::_op_se_byte (const Uint16 opcode);
void Cpu::_op_sne (const Uint16 opcode);
void Cpu::_op_se_vy (const Uint16 opcode);
void Cpu::_op_ld_vx_byte (const Uint16 opcode);
void Cpu::_op_add_vx_byte (const Uint16 opcode);
void Cpu::_op_ld_vx_vy (const Uint16 opcode);
void Cpu::_op_or (const Uint16 opcode);
void Cpu::_op_and (const Uint16 opcode);
void Cpu::_op_xor (const Uint16 opcode);
void Cpu::_op_add_vx_vy (const Uint16 opcode);
void Cpu::_op_sub (const Uint16 opcode);
void Cpu::_op_shr (const Uint16 opcode);
void Cpu::_op_subn (const Uint16 opcode);
void Cpu::_op_shl (const Uint16 opcode);
void Cpu::_op_sne_vx_vy (const Uint16 opcode);
void Cpu::_op_ld_i_addr (const Uint16 opcode);
void Cpu::_op_jp_v0_addr (const Uint16 opcode);
void Cpu::_op_rnd (const Uint16 opcode);
void Cpu::_op_drw (const Uint16 opcode);
void Cpu::_op_skp (const Uint16 opcode);
void Cpu::_op_sknp (const Uint16 opcode);
void Cpu::_op_ld_vx_dt (const Uint16 opcode);
void Cpu::_op_ld_vx_k (const Uint16 opcode);
void Cpu::_op_ld_dt_vx (const Uint16 opcode);
void Cpu::_op_ld_st_vx (const Uint16 opcode);
void Cpu::_op_add_i_vx (const Uint16 opcode);
void Cpu::_op_ld_f_vx (const Uint16 opcode);
void Cpu::_op_ld_b_vx (const Uint16 opcode);
void Cpu::_op_ld_i_vx (const Uint16 opcode);
void Cpu::_op_ld_vx_i (const Uint16 opcode);

void Cpu::_init_opcodes () {
    _opcode_list[0].id = 0x0FFF; _opcode_list[0].mask = 0x0000; _opcode_list[0].fun_ptr = &Cpu::_op_sys;           /* 0NNN */
    _opcode_list[1].id = 0x00E0; _opcode_list[1].mask = 0xFFFF; _opcode_list[1].fun_ptr = &Cpu::_op_cls;           /* 00E0 */
    _opcode_list[2].id = 0x00EE; _opcode_list[2].mask = 0xFFFF; _opcode_list[2].fun_ptr = &Cpu::_op_ret;           /* 00EE */
    _opcode_list[3].id = 0x1000; _opcode_list[3].mask = 0xF000; _opcode_list[3].fun_ptr = &Cpu::_op_jp_addr;       /* 1NNN */
    _opcode_list[4].id = 0x2000; _opcode_list[4].mask = 0xF000; _opcode_list[4].fun_ptr = &Cpu::_op_call;          /* 2NNN */
    _opcode_list[5].id = 0x3000; _opcode_list[5].mask = 0xF000; _opcode_list[5].fun_ptr = &Cpu::_op_se_byte;       /* 3XNN */
    _opcode_list[6].id = 0x4000; _opcode_list[6].mask = 0xF000; _opcode_list[6].fun_ptr = &Cpu::_op_sne;           /* 4XNN */
    _opcode_list[7].id = 0x5000; _opcode_list[7].mask = 0xF00F; _opcode_list[7].fun_ptr = &Cpu::_op_se_vy;         /* 5XY0 */
    _opcode_list[8].id = 0x6000; _opcode_list[8].mask = 0xF000; _opcode_list[8].fun_ptr = &Cpu::_op_ld_vx_byte;    /* 6XNN */
    _opcode_list[9].id = 0x7000; _opcode_list[9].mask = 0xF000; _opcode_list[9].fun_ptr = &Cpu::_op_add_vx_byte;   /* 7XNN */
    _opcode_list[10].id = 0x8000; _opcode_list[10].mask = 0xF00F; _opcode_list[10].fun_ptr = &Cpu::_op_ld_vx_vy;   /* 8XY0 */
    _opcode_list[11].id = 0x8001; _opcode_list[11].mask = 0xF00F; _opcode_list[11].fun_ptr = &Cpu::_op_or;         /* 8XY1 */
    _opcode_list[12].id = 0x8002; _opcode_list[12].mask = 0xF00F; _opcode_list[12].fun_ptr = &Cpu::_op_and;        /* 8XY2 */
    _opcode_list[13].id = 0x8003; _opcode_list[13].mask = 0xF00F; _opcode_list[13].fun_ptr = &Cpu::_op_xor;        /* 8XY3 */
    _opcode_list[14].id = 0x8004; _opcode_list[14].mask = 0xF00F; _opcode_list[14].fun_ptr = &Cpu::_op_add_vx_vy;  /* 8XY4 */
    _opcode_list[15].id = 0x8005; _opcode_list[15].mask = 0xF00F; _opcode_list[15].fun_ptr = &Cpu::_op_sub;        /* 8XY5 */
    _opcode_list[16].id = 0x8006; _opcode_list[16].mask = 0xF00F; _opcode_list[16].fun_ptr = &Cpu::_op_shr;        /* 8XY6 */
    _opcode_list[17].id = 0x8007; _opcode_list[17].mask = 0xF00F; _opcode_list[17].fun_ptr = &Cpu::_op_subn;       /* 8XY7 */
    _opcode_list[18].id = 0x800E; _opcode_list[18].mask = 0xF00F; _opcode_list[18].fun_ptr = &Cpu::_op_shl;        /* 8XYE */
    _opcode_list[19].id = 0x9000; _opcode_list[19].mask = 0xF00F; _opcode_list[19].fun_ptr = &Cpu::_op_sne_vx_vy;  /* 9XY0 */
    _opcode_list[20].id = 0xA000; _opcode_list[20].mask = 0xF000; _opcode_list[20].fun_ptr = &Cpu::_op_ld_i_addr;  /* ANNN */
    _opcode_list[21].id = 0xB000; _opcode_list[21].mask = 0xF000; _opcode_list[21].fun_ptr = &Cpu::_op_jp_v0_addr; /* BNNN */
    _opcode_list[22].id = 0xC000; _opcode_list[22].mask = 0xF000; _opcode_list[22].fun_ptr = &Cpu::_op_rnd;        /* CXNN */
    _opcode_list[23].id = 0xD000; _opcode_list[23].mask = 0xF000; _opcode_list[23].fun_ptr = &Cpu::_op_drw;        /* DXYN */
    _opcode_list[24].id = 0xE09E; _opcode_list[24].mask = 0xF0FF; _opcode_list[24].fun_ptr = &Cpu::_op_skp;        /* EX9E */
    _opcode_list[25].id = 0xE0A1; _opcode_list[25].mask = 0xF0FF; _opcode_list[24].fun_ptr = &Cpu::_op_sknp;       /* EXA1 */
    _opcode_list[26].id = 0xF007; _opcode_list[26].mask = 0xF0FF; _opcode_list[26].fun_ptr = &Cpu::_op_ld_vx_dt;   /* FX07 */
    _opcode_list[27].id = 0xF00A; _opcode_list[27].mask = 0xF0FF; _opcode_list[27].fun_ptr = &Cpu::_op_ld_vx_k;    /* FX0A */
    _opcode_list[28].id = 0xF015; _opcode_list[28].mask = 0xF0FF; _opcode_list[28].fun_ptr = &Cpu::_op_ld_dt_vx;   /* FX15 */
    _opcode_list[29].id = 0xF018; _opcode_list[29].mask = 0xF0FF; _opcode_list[29].fun_ptr = &Cpu::_op_ld_st_vx;   /* FX18 */
    _opcode_list[30].id = 0xF01E; _opcode_list[30].mask = 0xF0FF; _opcode_list[30].fun_ptr = &Cpu::_op_add_i_vx;   /* FX1E */
    _opcode_list[31].id = 0xF029; _opcode_list[31].mask = 0xF0FF; _opcode_list[31].fun_ptr = &Cpu::_op_ld_f_vx;    /* FX29 */
    _opcode_list[32].id = 0xF033; _opcode_list[32].mask = 0xF0FF; _opcode_list[32].fun_ptr = &Cpu::_op_ld_b_vx;    /* FX33 */
    _opcode_list[33].id = 0xF055; _opcode_list[33].mask = 0xF0FF; _opcode_list[33].fun_ptr = &Cpu::_op_ld_i_vx;    /* FX55 */
    _opcode_list[34].id = 0xF065; _opcode_list[34].mask = 0xF0FF; _opcode_list[34].fun_ptr = &Cpu::_op_ld_vx_i;    /* FX65 */
}
