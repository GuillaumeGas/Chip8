#include <iostream>
#include <SDL/SDL.h>

#include "src/Exceptions.hpp"
#include "src/Chip8.hpp"
#include "src/Disassembly.hpp"

using namespace std;

int main(int argc, char** argv)
{
    if (argc == 3) {
	if (string (argv[1]) == "-d") {
	    try {
		Chip8 c (argv[2], true);
		c.start ();
	    } catch (const Chip8Exception & e) {
		cout << e.toString () << endl;
	    }
	} else if (string (argv[1]) == "-disass") {
	    try {
		Disassembly d (argv[2]);
		d.disass ();
	    } catch (const Chip8Exception & e) {
		cout << e.toString () << endl;
	    }
	} else {
	    cout << "Parameters must be before the binary file name." << endl;
	    cout << "Usage : chip8 [-d | -disass] file" << endl;
	    return -1;
	}
    } else if (argc == 2) {
	try {
	    Chip8 c (argv[1], false);
	    c.start ();
	} catch (const Chip8Exception & e) {
	    cout << e.toString () << endl;
	}
    } else {
	if (argc > 3) {
	    cout << "Too many arguments !" << endl;
	    cout << "Usage : chip8 [-d] file" << endl;
	} else {
	    cout << "Missing program." << endl;
	    cout << "Usage : chip8 [-d | -disass] file" << endl;
	}
	return -1;
    }    
    return 0;
}
