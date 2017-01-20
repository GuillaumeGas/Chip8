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
    _delay_timer = 0;
    _sound_timer = 0;

    _screen = sc;

    srand (time (NULL));
    _loadFont ();
}

void Cpu::start () {
    bool stop = false;

    do {
	Uint16 opcode = _getNextOpCode ();
	_pc += 2;
	_exec_opcode (opcode);

	_screen->update ();
	_count ();
	SDL_Delay (FPS);
    } while (!stop);
}

void Cpu::loadProgram (const char * file_name) {
    FILE * game = NULL;
    game = fopen (file_name, "rb");

    if (game != NULL) {
	fread (&_memory[START_ADDRESS], sizeof (Uint8) * (MEM_SIZE - START_ADDRESS), 1, game);
	fclose (game);
    } else {
	cout << "Error, can't read program." << endl;
	throw -1;
    }
}

Uint16 Cpu::_getNextOpCode () {
    return (_memory[_pc] << 8) + _memory[_pc + 1];
}

void Cpu::_count () {
    if (_delay_timer > 0)
	_delay_timer--;
    if (_sound_timer > 0)
	_sound_timer--;
}

Uint8 Cpu::_getRegV (Uint16 i) {
    if (i > 16)
	throw -1;

    return _v_registers [i];
}

void Cpu::_setRegV (Uint16 i, Uint8 val) {
    if (i > 16)
	throw -1;

    _v_registers [i] = val;
}

void Cpu::print () const {
    cout << "** Register **" << endl;
    cout << "PC : " << _pc << endl;
    cout << "I : " << _I << endl;
    cout << "SP : " << _sp << endl;
    cout << "Delay counter : " << _delay_timer << endl;
    cout << "Sound counter : " << _sound_timer << endl;
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

param_t Cpu::_getParams (const Uint16 opcode) {
    return {static_cast<Uint16> ((opcode & 0x0F00) >> 8), static_cast<Uint16> ((opcode & 0x0F00) >> 4)};
}

void Cpu::_op_sys (const Uint16 opcode) {
    cout << "Op SYS : impossible !" << endl;
}
void Cpu::_op_cls (const Uint16 opcode) {
    _screen->clear ();
}
void Cpu::_op_ret (const Uint16 opcode) {
    _pc = _stack[_sp];
    if (_sp > 0) _sp--;
}
void Cpu::_op_jp_addr (const Uint16 opcode) {
    _pc = opcode & 0x0FFF;
    _pc -= 2;
}
void Cpu::_op_call (const Uint16 opcode) {
    _sp++;
    _stack[_sp] = _pc;
    _pc = opcode & 0x0FFF;
    _pc -= 2;
}
void Cpu::_op_se_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    if (_getRegV (vx) == (opcode & 0x00FF))
	_pc += 2;
}
void Cpu::_op_sne (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    if (_getRegV (vx) != (opcode & 0x00FF))
	_pc += 2;
}
void Cpu::_op_se_vy (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if (_getRegV (p.vx) != _getRegV (p.vy))
	_pc += 2;
}
void Cpu::_op_ld_vx_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    _setRegV (vx, opcode & 0x00FF);
}
void Cpu::_op_add_vx_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    _setRegV (vx, _getRegV (vx) + (opcode & 0x00FF));
}
void Cpu::_op_ld_vx_vy (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _setRegV (p.vx, _getRegV (p.vy));
}
void Cpu::_op_or (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _setRegV (p.vx, _getRegV (p.vx) | _getRegV (p.vy));
}
void Cpu::_op_and (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _setRegV (p.vx, _getRegV (p.vx) & _getRegV (p.vy));
}
void Cpu::_op_xor (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _setRegV (p.vx, _getRegV (p.vx) ^ _getRegV (p.vy));
}
void Cpu::_op_add_vx_vy (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if (_getRegV (p.vx) + _getRegV (p.vy) > 0xFF) {
	_setRegV (0xF, 1);
    } else {
	_setRegV (0xF, 0);
    }
    _setRegV (p.vx, _getRegV (p.vx) + _getRegV (p.vy));
}
void Cpu::_op_sub (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if (_getRegV (p.vy) < _getRegV (p.vx)) {
	_setRegV (0xF, 1);
    } else {
	_setRegV (0xF, 0);
    }
    _setRegV (p.vx, _getRegV (p.vx) - _getRegV (p.vy));
}
void Cpu::_op_shr (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if ((_getRegV (p.vx) & 0x000F) == 1) {
	_setRegV (0xF, 1);
    } else {
	_setRegV (0xF, 0);
    }
    _setRegV (p.vx, _getRegV (p.vx) / 2);
}
void Cpu::_op_subn (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if (_getRegV (p.vy) > _getRegV (p.vx)) {
	_setRegV (0xF, 1);
    } else {
	_setRegV (0xF, 0);
    }
    _setRegV (p.vx, _getRegV (p.vy) - _getRegV (p.vx));
}
void Cpu::_op_shl (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if ((_getRegV (p.vx) & 0x8000) == 1) {
	_setRegV (0xF, 1);
    } else {
	_setRegV (0xF, 0);
    }
    _setRegV (p.vx, _getRegV (p.vx) * 2);
}
void Cpu::_op_sne_vx_vy (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    if (_getRegV (p.vx) != _getRegV (p.vy))
	_pc += 2;
}
void Cpu::_op_ld_i_addr (const Uint16 opcode) {
    _I = opcode & 0x0FFF;
}
void Cpu::_op_jp_v0_addr (const Uint16 opcode) {
    _pc = (opcode & 0x0FFF) + _getRegV (0);
    _pc -= 2;
}
void Cpu::_op_rnd (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    Uint8 rnd = (Uint8) rand () % 255 + 1;
    _setRegV (p.vx, rnd & (opcode & 0x00FF));
}
void Cpu::_op_drw (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    Uint8 n = opcode & 0x000F;
    Uint8 offset;

    for (int i = 0; i < n; i++) {
	for (int j = 0, offset = 8; j < 8; j++, offset--) {
	    Uint8 mem_pixel = _memory[_I + i] & (0x1 << offset);
	    if (mem_pixel != 0) {
		if (_screen->getPixel (p.vx + j, p.vy + i).color == WHITE) {
		    _screen->setColor (p.vx + j, p.vy + i, BLACK);
		    _setRegV (0xF, 1);
		} else {
		    _screen->setColor (p.vx + j, p.vy + i, WHITE);
		}
	    }
	}
    }
}
void Cpu::_op_skp (const Uint16 opcode) {}
void Cpu::_op_sknp (const Uint16 opcode) {}
void Cpu::_op_ld_vx_dt (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _setRegV (p.vx, _delay_timer);
}
void Cpu::_op_ld_vx_k (const Uint16 opcode) {}
void Cpu::_op_ld_dt_vx (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _delay_timer = _getRegV (p.vx);
}
void Cpu::_op_ld_st_vx (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _sound_timer = _getRegV (p.vx);
}
void Cpu::_op_add_i_vx (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    _I = _I + _getRegV (p.vx);
}
void Cpu::_op_ld_f_vx (const Uint16 opcode) {
    param_t p =_getParams (opcode);
    _I = 5 * _getRegV (p.vx);
}
void Cpu::_op_ld_b_vx (const Uint16 opcode) {
    param_t p = _getParams(opcode);
    _memory[_I] = (_getRegV (p.vx) - _getRegV (p.vx) % 100) / 100;
    _memory[_I + 1] = ((_getRegV (p.vx) - _getRegV (p.vx) % 10) / 10) % 10;
    _memory[_I + 2] = _getRegV (p.vx) - _memory[_I] * 100 - _memory[_I + 1] * 10;
}
void Cpu::_op_ld_i_vx (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    for (Uint8 i = 0; i <= p.vx; i++) {
	_memory[_I + i] = _getRegV (i);
    }
}
void Cpu::_op_ld_vx_i (const Uint16 opcode) {
    param_t p = _getParams (opcode);
    for (Uint8 i = 0; i <= p.vx; i++) {
	_setRegV (i, _memory[_I + i]);
    }
}

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

void Cpu::_loadFont() { 
    _memory[0]=0xF0;_memory[1]=0x90;_memory[2]=0x90;_memory[3]=0x90; _memory[4]=0xF0; // O 
    _memory[5]=0x20;_memory[6]=0x60;_memory[7]=0x20;_memory[8]=0x20;_memory[9]=0x70; // 1 
    _memory[10]=0xF0;_memory[11]=0x10;_memory[12]=0xF0;_memory[13]=0x80; _memory[14]=0xF0; // 2 
    _memory[15]=0xF0;_memory[16]=0x10;_memory[17]=0xF0;_memory[18]=0x10;_memory[19]=0xF0; // 3 
    _memory[20]=0x90;_memory[21]=0x90;_memory[22]=0xF0;_memory[23]=0x10;_memory[24]=0x10; // 4 
    _memory[25]=0xF0;_memory[26]=0x80;_memory[27]=0xF0;_memory[28]=0x10;_memory[29]=0xF0; // 5 
    _memory[30]=0xF0;_memory[31]=0x80;_memory[32]=0xF0;_memory[33]=0x90;_memory[34]=0xF0; // 6 
    _memory[35]=0xF0;_memory[36]=0x10;_memory[37]=0x20;_memory[38]=0x40;_memory[39]=0x40; // 7 
    _memory[40]=0xF0;_memory[41]=0x90;_memory[42]=0xF0;_memory[43]=0x90;_memory[44]=0xF0; // 8 
    _memory[45]=0xF0;_memory[46]=0x90;_memory[47]=0xF0;_memory[48]=0x10;_memory[49]=0xF0; // 9 
    _memory[50]=0xF0;_memory[51]=0x90;_memory[52]=0xF0;_memory[53]=0x90;_memory[54]=0x90; // A 
    _memory[55]=0xE0;_memory[56]=0x90;_memory[57]=0xE0;_memory[58]=0x90;_memory[59]=0xE0; // B 
    _memory[60]=0xF0;_memory[61]=0x80;_memory[62]=0x80;_memory[63]=0x80;_memory[64]=0xF0; // C 
    _memory[65]=0xE0;_memory[66]=0x90;_memory[67]=0x90;_memory[68]=0x90;_memory[69]=0xE0; // D 
    _memory[70]=0xF0;_memory[71]=0x80;_memory[72]=0xF0;_memory[73]=0x80;_memory[74]=0xF0; // E 
    _memory[75]=0xF0;_memory[76]=0x80;_memory[77]=0xF0;_memory[78]=0x80;_memory[79]=0x80; // F 
}
