#include <iostream>
#include <SDL/SDL.h>

#include "src/Chip8.hpp"

using namespace std;

int main(int argc, char** argv)
{
    Chip8 c;
    c.start ();

    return 0;
}
