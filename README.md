# Chip8
Chip8 emulator for fun

# Compile
mkdir .build && cd .build
cmake ..
make

# Usage
./chip8 [-d | -disass] rom_file
* -d : debug mode (p : print memory dump, n : next instruction, c : continue, q : quit)
* -disass : disassemble the rom
