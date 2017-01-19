#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <SDL/SDL.h>

#define MEM_SIZE 4096
#define START_ADDRESS 512
#define V_REGISTERS_SIZE 16
#define STACK_SIZE 16
#define CPU_SPEED 4
#define FPS 16
#define NB_OPCODES 35

class Screen;
class Cpu;
struct opcode_t {
    Uint16 id;
    Uint16 mask;
    typedef void (Cpu::* FunPtr) (const Uint16 opcode);
    FunPtr fun_ptr;
};

struct param_t {
    Uint16 vx, vy;
};

class Cpu {
public:
    Cpu (Screen * sc);

    void start ();
    void print () const;

private:
    void _count ();
    Uint8 _getRegV (Uint16 i);
    void _setRegV (Uint16 i, Uint8 val);
    Uint16 _getNextOpCode ();
    void _exec_opcode (const Uint16 opcode);
    param_t _getParams (const Uint16 opcode);

    void _init_opcodes ();

    Uint8 _memory[MEM_SIZE];
    /* program counter */
    Uint16 _pc;
    /* 16 registers V0 -> VF */
    Uint8 _v_registers[V_REGISTERS_SIZE];
    /* Usually used to store memory adresses */
    Uint16 _I;
    /* Stack pointer */
    Uint8 _sp;
    Uint16 _stack[STACK_SIZE];
    /* delay timer */
    Uint8 _delay_timer;
    /* sound timer */
    Uint8 _sound_timer;
    /* Pointer on the screen */
    Screen * _screen;
    /* List of existing opcodes */
    opcode_t _opcode_list[35];

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
