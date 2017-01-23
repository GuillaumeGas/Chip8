#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <SDL/SDL.h>

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define STACK_SIZE 16
#define CPU_SPEED 4
#define FPS 16
#define NB_OPCODES 35

enum OPCODE {
    _00E0_, _00EE_, _0NNN_, _1NNN_, _2NNN_, _3XNN_, _4XNN_, _5XY0_, _6XNN_,
    _7XNN_, _8XY0_, _8XY1_, _8XY2_, _8XY3_, _8XY4_, _8XY5_, _8XY6_, _8XY7_,
    _8XYE_, _9XY0_, _ANNN_, _BNNN_, _CXNN_, _DXYN_, _EX9E_, _EXA1_, _FX07_,
    _FX0A_, _FX15_, _FX18_, _FX1E_, _FX29_, _FX33_, _FX55_, _FX65_
};

class Screen;
class Cpu;
struct opcode_t {
    Uint16 id;
    Uint16 mask;
    OPCODE code;
    typedef void (Cpu::* FunPtr) (const Uint16 opcode);
    FunPtr fun_ptr;
};

struct param_t {
    Uint16 vx, vy;
};

class Cpu {
public:
    Cpu (Screen * sc, bool debug);

    void start ();
    void dump () const;
    void loadProgram (const char* file_name);
    void shutdown ();

private:
    void count ();
    Uint8 getRegV (Uint16 i);
    void setRegV (Uint16 i, Uint8 val);
    Uint16 getNextOpCode ();
    void exec_opcode (const Uint16 opcode);
    param_t getParams (const Uint16 opcode);
    void debug_inst (opcode_t);

    void init_opcodes ();
    void loadFont ();

    bool started;
    Uint8 memory[MEM_SIZE];
    /* program counter */
    Uint16 pc;
    /* 16 registers V0 -> VF */
    Uint8 v_registers[V_REGISTERS_SIZE];
    /* Usually used to store memory adresses */
    Uint16 I;
    /* Stack pointer */
    Uint8 sp;
    Uint16 stack[STACK_SIZE];
    /* delay timer */
    Uint8 delay_timer;
    /* sound timer */
    Uint8 sound_timer;
    /* Pointer on the screen */
    Screen * screen;
    /* List of existing opcodes */
    opcode_t opcode_list[35];

    bool debug;

    /* Operation's functions */
    void _op_sys (const Uint16 opcode);
    void _op_cls (const Uint16 opcode);
    void _op_ret (const Uint16 opcode);
    void _op_jp_addr (const Uint16 opcode);
    void _op_call (const Uint16 opcode);
    void _op_se_byte (const Uint16 opcode);
    void _op_sne (const Uint16 opcode);
    void _op_se_vy (const Uint16 opcode);
    void _op_ld_vx_byte (const Uint16 opcode);
    void _op_add_vx_byte (const Uint16 opcode);
    void _op_ld_vx_vy (const Uint16 opcode);
    void _op_or (const Uint16 opcode);
    void _op_and (const Uint16 opcode);
    void _op_xor (const Uint16 opcode);
    void _op_add_vx_vy (const Uint16 opcode);
    void _op_sub (const Uint16 opcode);
    void _op_shr (const Uint16 opcode);
    void _op_subn (const Uint16 opcode);
    void _op_shl (const Uint16 opcode);
    void _op_sne_vx_vy (const Uint16 opcode);
    void _op_ld_i_addr (const Uint16 opcode);
    void _op_jp_v0_addr (const Uint16 opcode);
    void _op_rnd (const Uint16 opcode);
    void _op_drw (const Uint16 opcode);
    void _op_skp (const Uint16 opcode);
    void _op_sknp (const Uint16 opcode);
    void _op_ld_vx_dt (const Uint16 opcode);
    void _op_ld_vx_k (const Uint16 opcode);
    void _op_ld_dt_vx (const Uint16 opcode);
    void _op_ld_st_vx (const Uint16 opcode);
    void _op_add_i_vx (const Uint16 opcode);
    void _op_ld_f_vx (const Uint16 opcode);
    void _op_ld_b_vx (const Uint16 opcode);
    void _op_ld_i_vx (const Uint16 opcode);
    void _op_ld_vx_i (const Uint16 opcode);
};
