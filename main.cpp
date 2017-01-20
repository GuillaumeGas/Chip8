#include <iostream>
#include <SDL/SDL.h>

#include "src/Chip8.hpp"

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2) {
	cout << "Missing program." << endl;
	return -1;
    }

    Chip8 c (argv[1]);
    c.start ();

    return 0;
}
