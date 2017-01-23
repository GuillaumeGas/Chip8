#include <iostream>
#include <SDL/SDL.h>

#include "src/Chip8.hpp"

using namespace std;

int main(int argc, char** argv)
{
    if (argc == 3) {
	if (string (argv[1]) == "-d") {
	    Chip8 c (argv[2], true);
	    c.start ();
	} else {
	    cout << "Parameters must be before the binary file name." << endl;
	    cout << "Usage : chip8 [-d] file" << endl;
	    return -1;
	}
    } else if (argc == 2) {
	Chip8 c (argv[1], false);
	c.start ();
    } else {
	if (argc > 3) {
	    cout << "Too many arguments !" << endl;
	    cout << "Usage : chip8 [-d] file" << endl;
	} else {
	    cout << "Missing program." << endl;
	    cout << "Usage : chip8 [-d] file" << endl;
	}
	return -1;
    }    
    return 0;
}
