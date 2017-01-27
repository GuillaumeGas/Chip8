#include "Disassembly.hpp"

using namespace std;

Disassembly::Disassembly (const char * file_name) {
    this->loadFile (file_name);
}

void Disassembly::disass () {
    int offset = 512;
    uint16_t opcode;
    int i = 0;

    do {
	opcode = (this->buffer[i] << 8) | this->buffer[i + 1];
	bool opcode_found = false;
	
	for (auto it : *(Opcodes::instance ()->getList ())) {
	    if ((opcode & it.second->mask) == it.first) {
		cout << "[" << offset << "] " 
		     << std::showbase << std::internal << std::setfill ('0') 
		     << std::hex << std::setw (6) << opcode 
		     << std::dec << " " << it.second->disassemble (opcode) << endl;
		opcode_found = true;
		break;		
	    }
	}
	if (!opcode_found && opcode != 0)
	    throw OpcodeNotFound (opcode, offset);

	offset += 2;
	i += 2;
    } while (i < (BUF_SIZE - 1) && opcode != 0);
}

void Disassembly::loadFile (const char * file_name) {
    cout << "> Loading binary file..." << endl;
    FILE * game = NULL;
    game = fopen (file_name, "rb");

    if (game != NULL) {
	fread (&this->buffer, sizeof (uint8_t) * BUF_SIZE, 1, game);
	fclose (game);
    } else {
	throw LoadFileError (file_name);
    }
}
