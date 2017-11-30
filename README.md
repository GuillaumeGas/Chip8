# Chip8
Chip8 emulator for fun

# Compile
mkdir .build && cd .build
cmake ..
make

# Usage
./chip8 [-d | -disass] rom_file
* -d : debug mode
* -disass : disassemble the rom

# Debug mode
- p : step (executes one instruction and break)
- r : reg (displays all the registers)
- c : continues the execution
- b* breakpoint commands :
 - bp addr : adds a breakpoint on this address
 - bl : breakpoints list
 - bd addr : disables the breakpoint on this address
 - be addr : enables the breakpoint on this address
 - bc addr : clears the breakpoint on this address
- q : quits the emulator
- h : displays this help
