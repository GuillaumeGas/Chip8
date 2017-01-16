#include "Cpu.hpp"

using namespace std;

Cpu::Cpu () {
    memset (_memory, 0, MEM_SIZE);
    memset (_v_registers, 0, V_REGISTERS_SIZE);
    memset (_stack, 0, STACK_SIZE);

    _pc = START_ADDRESS;
    _I = 0;
    _sp = 0;
    _delay_counter = 0;
    _sound_counter = 0;
}

void Cpu::count () {
    if (_delay_counter > 0)
	_delay_counter--;
    if (_sound_counter > 0)
	_sound_counter--;
}

Uint8 * Cpu::getRegV (unsigned int i) {
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
