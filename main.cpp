#include <iostream>
#include <SDL2/SDL.h>
#include <exception>

#include "src/Exceptions.hpp"
#include "src/Chip8.hpp"
#include "src/Disassembly.hpp"
#include "src/utils/Directory.hpp"
//#include "src/Assembler/Assembler.hpp"

#define DEFAULT_ROM "welcome.ch8"

using namespace std;

void printUsage();

int main(int argc, char** argv)
{
	try {
		if (argc == 1)
		{
			Chip8 c(DEFAULT_ROM);
			c.start();
		}
		else if (argc == 2)
		{
			Chip8 c(argv[1]);
			c.start();
		}
		else if (argc >= 3)
		{
			string cmd = argv[1];

			if (cmd == "-d")
			{
				Chip8 c(argv[2], true);
				c.start();
			}
			else if (cmd == "-disass")
			{
				Disassembly d(argv[2]);
				d.disass();
			}
			/*else if (cmd == "-asm")
			{
				if (argc > 3)
				{
					Assembler a(argv[2], argv[3]);
					a.assemble();
				}
				else
				{
					Assembler a(argv[2], argv[3]);
					a.assemble();
				}
			}*/
			else
			{
				throw UnknownCommandArgsException(argv[1]);
			}
		}
		else
		{
			throw TooManyArgsException();
		}
	}
	catch (const ArgsException & e)
	{
		cout << e.toString() << endl;
		printUsage();
	}
	catch (const Chip8Exception & e)
	{
		cout << e.toString() << endl;
	}
	catch (const exception & e)
	{
		cout << "An error occured ! (" << e.what() << ")" << endl;
		return -1;
	}
	return 0;
}

void printUsage()
{
	cout << "Usage :" << endl;
	cout << "  chip8.exe [[-d | -disass] rom_path.ch8]" << endl << endl;
	cout << "  -d : enable debug mode" << endl;
	cout << "  -disass : disassemble the rom" << endl;
}