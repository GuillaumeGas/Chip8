#include "Cpu.hpp"
#include "Screen.hpp"

using namespace std;

Cpu::Cpu (Screen * sc, bool debug) {
    this->debug = debug;
    this->started = true;

    if (debug) {
	cout << "> CPU starting on debug mode..." << endl;
    } else {
	cout << "> CPU starting..." << endl;
    }
    memset (this->memory, 0, MEM_SIZE);
    memset (this->v_registers, 0, V_REGISTERS_SIZE);
    memset (this->stack, 0, STACK_SIZE);

    this->pc = START_ADDRESS;
    this->I = 0;
    this->sp = 0;
    this->delay_timer = 0;
    this->sound_timer = 0;

    this->screen = sc;

    srand (time (NULL));
    this->init_opcodes ();
    this->loadFont ();
}

void Cpu::start () {
    do {
	if (this->pc < (MEM_SIZE - 1)) {
	    Uint16 opcode = getNextOpCode ();
	    exec_opcode (opcode);

	    this->pc += 2;
	    this->screen->update ();
	    this->count ();
	    SDL_Delay (FPS);
	} else {
	    this->started = false;
	}
    } while (this->started);
}

void Cpu::shutdown () {
    cout << "> Stopping CPU..." << endl;
    this->started = false;
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

Uint8 Cpu::getRegV (Uint16 i) {
    if (i > 16)
	throw -1;

    return this->v_registers [i];
}

void Cpu::setRegV (Uint16 i, Uint8 val) {
    if (i > 16)
	throw -1;

    this->v_registers [i] = val;
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
	cout << "V" << i << " = " << (int) this->v_registers[i] << endl;
}

void Cpu::exec_opcode (const Uint16 opcode) {
    for (int i = 0; i < NB_OPCODES; i++) {
	opcode_t op = this->opcode_list[i];
	if ((opcode & op.mask) == op.id) {
	    if (this->debug)
		this->debug_inst (this->opcode_list[i]);
	    ((*this).*(op.fun_ptr)) (opcode);
	    break;
	}
    }
}

param_t Cpu::getParams (const Uint16 opcode) {
    return {static_cast<Uint16> ((opcode & 0x0F00) >> 8), static_cast<Uint16> ((opcode & 0x0F00) >> 4)};
}

void Cpu::_op_sys (const Uint16 opcode) {
    cout << "Op SYS : impossible !" << endl;
}
void Cpu::_op_cls (const Uint16 opcode) {
    this->screen->clear ();
}
void Cpu::_op_ret (const Uint16 opcode) {
    this->pc = this->stack[this->sp];
    if (this->sp > 0) this->sp--;
}
void Cpu::_op_jp_addr (const Uint16 opcode) {
    cout << "test" << endl;
    printf ("jump addr : %d\n", opcode & 0x0FFF);
    this->pc = opcode & 0x0FFF;
    this->pc -= 2;
}
void Cpu::_op_call (const Uint16 opcode) {
    this->stack[this->sp] = this->pc;
    this->sp++;
    this->pc = opcode & 0x0FFF;
    this->pc -= 2;
}
void Cpu::_op_se_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    if (this->getRegV (vx) == (opcode & 0x00FF))
	this->pc += 2;
}
void Cpu::_op_sne (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    if (this->getRegV (vx) != (opcode & 0x00FF))
	this->pc += 2;
}
void Cpu::_op_se_vy (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if (this->getRegV (p.vx) == this->getRegV (p.vy))
	this->pc += 2;
}
void Cpu::_op_ld_vx_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    this->setRegV (vx, opcode & 0x00FF);
}
void Cpu::_op_add_vx_byte (const Uint16 opcode) {
    Uint16 vx = (opcode & 0x0F00) >> 8;
    this->setRegV (vx, this->getRegV (vx) + (opcode & 0x00FF));
}
void Cpu::_op_ld_vx_vy (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->setRegV (p.vx, this->getRegV (p.vy));
}
void Cpu::_op_or (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->setRegV (p.vx, this->getRegV (p.vx) | this->getRegV (p.vy));
}
void Cpu::_op_and (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->setRegV (p.vx, this->getRegV (p.vx) & this->getRegV (p.vy));
}
void Cpu::_op_xor (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->setRegV (p.vx, this->getRegV (p.vx) ^ this->getRegV (p.vy));
}
void Cpu::_op_add_vx_vy (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    Uint8 sum = this->getRegV (p.vx) + this->getRegV (p.vy);
    if (sum > 0xFF) {
	this->setRegV (0xF, 1);
    } else {
	this->setRegV (0xF, 0);
    }
    this->setRegV (p.vx, sum & 0x00FF);
}
void Cpu::_op_sub (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if (this->getRegV (p.vy) < this->getRegV (p.vx)) {
	this->setRegV (0xF, 1);
    } else {
	this->setRegV (0xF, 0);
    }
    this->setRegV (p.vx, this->getRegV (p.vx) - this->getRegV (p.vy));
}
void Cpu::_op_shr (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if ((this->getRegV (p.vx) & 0x0001) == 1) {
	this->setRegV (0xF, 1);
    } else {
	this->setRegV (0xF, 0);
    }
    this->setRegV (p.vx, this->getRegV (p.vx) >> 1);
}
void Cpu::_op_subn (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if (this->getRegV (p.vy) > this->getRegV (p.vx)) {
	this->setRegV (0xF, 1);
    } else {
	this->setRegV (0xF, 0);
    }
    this->setRegV (p.vx, this->getRegV (p.vy) - this->getRegV (p.vx));
}
void Cpu::_op_shl (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if ((this->getRegV (p.vx) & 0x8000) == 1) {
	this->setRegV (0xF, 1);
    } else {
	this->setRegV (0xF, 0);
    }
    this->setRegV (p.vx, this->getRegV (p.vx) << 1);
}
void Cpu::_op_sne_vx_vy (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    if (this->getRegV (p.vx) != this->getRegV (p.vy))
	this->pc += 2;
}
void Cpu::_op_ld_i_addr (const Uint16 opcode) {
    this->I = opcode & 0x0FFF;
}
void Cpu::_op_jp_v0_addr (const Uint16 opcode) {
    // MOUAI PAS SUR DE CA...
    this->stack[this->sp] = this->pc;
    this->sp++;
    this->pc = (opcode & 0x0FFF) + this->getRegV (0);
    this->pc -= 2;
}
void Cpu::_op_rnd (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    Uint8 rnd = (Uint8) rand () % 255 + 1;
    this->setRegV (p.vx, rnd & (opcode & 0x00FF));
}
void Cpu::_op_drw (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    Uint8 n = opcode & 0x000F;
    Uint8 offset;

    for (int i = 0; i < n; i++) {
	for (int j = 0, offset = 8; j < 8; j++, offset--) {
	    Uint8 mem_pixel = this->memory[this->I + i] & (0x1 << offset);
	    if (mem_pixel != 0) {
		if (this->screen->getPixel (p.vx + j, p.vy + i).color == WHITE) {
		    this->screen->setColor (p.vx + j, p.vy + i, BLACK);
		    this->setRegV (0xF, 1);
		} else {
		    this->screen->setColor (p.vx + j, p.vy + i, WHITE);
		}
	    }
	}
    }
}
void Cpu::_op_skp (const Uint16 opcode) {}
void Cpu::_op_sknp (const Uint16 opcode) {}
void Cpu::_op_ld_vx_dt (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->setRegV (p.vx, this->delay_timer);
}
void Cpu::_op_ld_vx_k (const Uint16 opcode) {}
void Cpu::_op_ld_dt_vx (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->delay_timer = this->getRegV (p.vx);
}
void Cpu::_op_ld_st_vx (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->sound_timer = this->getRegV (p.vx);
}
void Cpu::_op_add_i_vx (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    this->I = this->I + this->getRegV (p.vx);
}
void Cpu::_op_ld_f_vx (const Uint16 opcode) {
    param_t p =this->getParams (opcode);
    this->I = 5 * this->getRegV (p.vx);
}
void Cpu::_op_ld_b_vx (const Uint16 opcode) {
    param_t p = this->getParams(opcode);
    this->memory[this->I] = (this->getRegV (p.vx) - this->getRegV (p.vx) % 100) / 100;
    this->memory[this->I + 1] = ((this->getRegV (p.vx) - this->getRegV (p.vx) % 10) / 10) % 10;
    this->memory[this->I + 2] = this->getRegV (p.vx) - this->memory[this->I] * 100 - this->memory[this->I + 1] * 10;
}
void Cpu::_op_ld_i_vx (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    for (Uint8 i = 0; i <= p.vx; i++) {
	this->memory[this->I + i] = this->getRegV (i);
    }
}
void Cpu::_op_ld_vx_i (const Uint16 opcode) {
    param_t p = this->getParams (opcode);
    for (Uint8 i = 0; i <= p.vx; i++) {
	this->setRegV (i, this->memory[this->I + i]);
    }
}

void Cpu::init_opcodes () {
    this->opcode_list[0].id = 0x0FFF;  this->opcode_list[0].mask = 0x0000;  this->opcode_list[0].fun_ptr = &Cpu::_op_sys;         this->opcode_list[0].code = OPCODE::_0NNN_;   /* 0NNN */
    this->opcode_list[1].id = 0x00E0;  this->opcode_list[1].mask = 0x00F0;  this->opcode_list[1].fun_ptr = &Cpu::_op_cls;         this->opcode_list[1].code = OPCODE::_00E0_;   /* 00E0 */
    this->opcode_list[2].id = 0x00EE;  this->opcode_list[2].mask = 0x00FF;  this->opcode_list[2].fun_ptr = &Cpu::_op_ret;         this->opcode_list[2].code = OPCODE::_00EE_;   /* 00EE */
    this->opcode_list[3].id = 0x1000;  this->opcode_list[3].mask = 0xF000;  this->opcode_list[3].fun_ptr = &Cpu::_op_jp_addr;     this->opcode_list[3].code = OPCODE::_1NNN_;   /* 1NNN */
    this->opcode_list[4].id = 0x2000;  this->opcode_list[4].mask = 0xF000;  this->opcode_list[4].fun_ptr = &Cpu::_op_call;        this->opcode_list[4].code = OPCODE::_2NNN_;   /* 2NNN */
    this->opcode_list[5].id = 0x3000;  this->opcode_list[5].mask = 0xF000;  this->opcode_list[5].fun_ptr = &Cpu::_op_se_byte;     this->opcode_list[5].code = OPCODE::_3XNN_;   /* 3XNN */
    this->opcode_list[6].id = 0x4000;  this->opcode_list[6].mask = 0xF000;  this->opcode_list[6].fun_ptr = &Cpu::_op_sne;         this->opcode_list[6].code = OPCODE::_4XNN_;   /* 4XNN */
    this->opcode_list[7].id = 0x5000;  this->opcode_list[7].mask = 0xF00F;  this->opcode_list[7].fun_ptr = &Cpu::_op_se_vy;       this->opcode_list[7].code = OPCODE::_5XY0_;   /* 5XY0 */
    this->opcode_list[8].id = 0x6000;  this->opcode_list[8].mask = 0xF000;  this->opcode_list[8].fun_ptr = &Cpu::_op_ld_vx_byte;  this->opcode_list[8].code = OPCODE::_6XNN_;   /* 6XNN */
    this->opcode_list[9].id = 0x7000;  this->opcode_list[9].mask = 0xF000;  this->opcode_list[9].fun_ptr = &Cpu::_op_add_vx_byte; this->opcode_list[9].code = OPCODE::_7XNN_;   /* 7XNN */
    this->opcode_list[10].id = 0x8000; this->opcode_list[10].mask = 0xF00F; this->opcode_list[10].fun_ptr = &Cpu::_op_ld_vx_vy;   this->opcode_list[10].code = OPCODE::_8XY0_;  /* 8XY0 */
    this->opcode_list[11].id = 0x8001; this->opcode_list[11].mask = 0xF00F; this->opcode_list[11].fun_ptr = &Cpu::_op_or;         this->opcode_list[11].code = OPCODE::_8XY1_;  /* 8XY1 */
    this->opcode_list[12].id = 0x8002; this->opcode_list[12].mask = 0xF00F; this->opcode_list[12].fun_ptr = &Cpu::_op_and;        this->opcode_list[12].code = OPCODE::_8XY2_;  /* 8XY2 */
    this->opcode_list[13].id = 0x8003; this->opcode_list[13].mask = 0xF00F; this->opcode_list[13].fun_ptr = &Cpu::_op_xor;        this->opcode_list[13].code = OPCODE::_8XY3_;  /* 8XY3 */
    this->opcode_list[14].id = 0x8004; this->opcode_list[14].mask = 0xF00F; this->opcode_list[14].fun_ptr = &Cpu::_op_add_vx_vy;  this->opcode_list[14].code = OPCODE::_8XY4_;  /* 8XY4 */
    this->opcode_list[15].id = 0x8005; this->opcode_list[15].mask = 0xF00F; this->opcode_list[15].fun_ptr = &Cpu::_op_sub;        this->opcode_list[15].code = OPCODE::_8XY5_;  /* 8XY5 */
    this->opcode_list[16].id = 0x8006; this->opcode_list[16].mask = 0xF00F; this->opcode_list[16].fun_ptr = &Cpu::_op_shr;        this->opcode_list[16].code = OPCODE::_8XY6_;  /* 8XY6 */
    this->opcode_list[17].id = 0x8007; this->opcode_list[17].mask = 0xF00F; this->opcode_list[17].fun_ptr = &Cpu::_op_subn;       this->opcode_list[17].code = OPCODE::_8XY7_;  /* 8XY7 */
    this->opcode_list[18].id = 0x800E; this->opcode_list[18].mask = 0xF00F; this->opcode_list[18].fun_ptr = &Cpu::_op_shl;        this->opcode_list[18].code = OPCODE::_8XYE_;  /* 8XYE */
    this->opcode_list[19].id = 0x9000; this->opcode_list[19].mask = 0xF00F; this->opcode_list[19].fun_ptr = &Cpu::_op_sne_vx_vy;  this->opcode_list[19].code = OPCODE::_9XY0_;  /* 9XY0 */
    this->opcode_list[20].id = 0xA000; this->opcode_list[20].mask = 0xF000; this->opcode_list[20].fun_ptr = &Cpu::_op_ld_i_addr;  this->opcode_list[20].code = OPCODE::_ANNN_;  /* ANNN */
    this->opcode_list[21].id = 0xB000; this->opcode_list[21].mask = 0xF000; this->opcode_list[21].fun_ptr = &Cpu::_op_jp_v0_addr; this->opcode_list[21].code = OPCODE::_BNNN_;  /* BNNN */
    this->opcode_list[22].id = 0xC000; this->opcode_list[22].mask = 0xF000; this->opcode_list[22].fun_ptr = &Cpu::_op_rnd;        this->opcode_list[22].code = OPCODE::_CXNN_;  /* CXNN */
    this->opcode_list[23].id = 0xD000; this->opcode_list[23].mask = 0xF000; this->opcode_list[23].fun_ptr = &Cpu::_op_drw;        this->opcode_list[23].code = OPCODE::_DXYN_;  /* DXYN */
    this->opcode_list[24].id = 0xE09E; this->opcode_list[24].mask = 0xF0FF; this->opcode_list[24].fun_ptr = &Cpu::_op_skp;        this->opcode_list[24].code = OPCODE::_EX9E_;  /* EX9E */
    this->opcode_list[25].id = 0xE0A1; this->opcode_list[25].mask = 0xF0FF; this->opcode_list[24].fun_ptr = &Cpu::_op_sknp;       this->opcode_list[25].code = OPCODE::_EXA1_;  /* EXA1 */
    this->opcode_list[26].id = 0xF007; this->opcode_list[26].mask = 0xF0FF; this->opcode_list[26].fun_ptr = &Cpu::_op_ld_vx_dt;   this->opcode_list[26].code = OPCODE::_FX07_;  /* FX07 */
    this->opcode_list[27].id = 0xF00A; this->opcode_list[27].mask = 0xF0FF; this->opcode_list[27].fun_ptr = &Cpu::_op_ld_vx_k;    this->opcode_list[27].code = OPCODE::_FX0A_;  /* FX0A */
    this->opcode_list[28].id = 0xF015; this->opcode_list[28].mask = 0xF0FF; this->opcode_list[28].fun_ptr = &Cpu::_op_ld_dt_vx;   this->opcode_list[28].code = OPCODE::_FX15_;  /* FX15 */
    this->opcode_list[29].id = 0xF018; this->opcode_list[29].mask = 0xF0FF; this->opcode_list[29].fun_ptr = &Cpu::_op_ld_st_vx;   this->opcode_list[29].code = OPCODE::_FX18_;  /* FX18 */
    this->opcode_list[30].id = 0xF01E; this->opcode_list[30].mask = 0xF0FF; this->opcode_list[30].fun_ptr = &Cpu::_op_add_i_vx;   this->opcode_list[30].code = OPCODE::_FX1E_;  /* FX1E */
    this->opcode_list[31].id = 0xF029; this->opcode_list[31].mask = 0xF0FF; this->opcode_list[31].fun_ptr = &Cpu::_op_ld_f_vx;    this->opcode_list[31].code = OPCODE::_FX29_;  /* FX29 */
    this->opcode_list[32].id = 0xF033; this->opcode_list[32].mask = 0xF0FF; this->opcode_list[32].fun_ptr = &Cpu::_op_ld_b_vx;    this->opcode_list[32].code = OPCODE::_FX33_;  /* FX33 */
    this->opcode_list[33].id = 0xF055; this->opcode_list[33].mask = 0xF0FF; this->opcode_list[33].fun_ptr = &Cpu::_op_ld_i_vx;    this->opcode_list[33].code = OPCODE::_FX55_;  /* FX55 */
    this->opcode_list[34].id = 0xF065; this->opcode_list[34].mask = 0xF0FF; this->opcode_list[34].fun_ptr = &Cpu::_op_ld_vx_i;    this->opcode_list[34].code = OPCODE::_FX65_;  /* FX65 */
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

void Cpu::debug_inst (opcode_t opcode) {
    cout << "[" << this->pc << "] ";
    switch (opcode.code) {
    case _00E0_: cout << "cls" << endl; break;
    case _00EE_: cout << "ret" << endl; break;
    case _0NNN_: cout << "sys addr" << endl; break;
    case _1NNN_: cout << "jp addr" << endl; break;
    case _2NNN_: cout << "call addr" << endl; break;
    case _3XNN_: cout << "se vx, byte" << endl; break;
    case _4XNN_: cout << "sne vx, byte" << endl; break; 
    case _5XY0_: cout << "se vx, vy" << endl; break;
    case _6XNN_: cout << "ld vx, byte" << endl; break;
    case _7XNN_: cout << "add vx, byte" << endl; break;
    case _8XY0_: cout << "ld vx, vy" << endl; break;
    case _8XY1_: cout << "or vx, vy" << endl; break;
    case _8XY2_: cout << "and vx, vy" << endl; break;
    case _8XY3_: cout << "xor vx, by" << endl; break;
    case _8XY4_: cout << "add vx, vy" << endl; break;
    case _8XY5_: cout << "sub vx, vy" << endl; break;
    case _8XY6_: cout << "shr vx, vy" << endl; break;
    case _8XY7_: cout << "subn vx, vy" << endl; break;
    case _8XYE_: cout << "shl vx, vy" << endl; break;
    case _9XY0_: cout << "sne vx, vy" << endl; break;
    case _ANNN_: cout << "ld I, addr" << endl; break;
    case _BNNN_: cout << "jp v0, addr" << endl; break;
    case _CXNN_: cout << "rnd vx, byte" << endl; break;
    case _DXYN_: cout << "drw vx, vy, nibble" << endl; break;
    case _EX9E_: cout << "skp vx" << endl; break;
    case _EXA1_: cout << "sknp vx" << endl; break;
    case _FX07_: cout << "ld vx, dt" << endl; break;
    case _FX0A_: cout << "ld vx, k" << endl; break;
    case _FX15_: cout << "ld dt, vx" << endl; break;
    case _FX18_: cout << "ld st, vx" << endl; break;
    case _FX1E_: cout << "add I, vx" << endl; break;
    case _FX29_: cout << "ld f, vx" << endl; break;
    case _FX33_: cout << "ld b, vx" << endl; break;
    case _FX55_: cout << "ld [I], vx" << endl; break;
    case _FX65_: cout << "ld vx, [I]" << endl; break;
    default:
	cout << "[!] Unknown opcode !" << endl;
	exit (-1);
    }

    char rep = -1;
    while (rep != 'c') {
	cout << " >> ";
	cin >> rep;
	if (rep == 'p') {
	    this->dump ();
	} else if (rep == 'q') {
	    this->shutdown ();
	}
    }
}
