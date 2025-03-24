

### Specs
- 8 Registers each holds a 32-bit word.
- Memory segments dynamic collection; accessed via 32-bit addresses.
- I/O with 8-bit ASCII characters.
- Program Counter 32-bit, points to instruction in segment 0 (the program).

### Initial State
- `$m[0]` = loaded with program instructions.
- Other segments unmapped
- All registers set to zero
- Program counter points to first word seg 0

### Execution Cycle:
- Fetch instruction from `$m[0][PC]`
- Increment PC.
- Execute instruction.


## Instructions

Format (Big endien)
MSB Opcode(4 bits), extra space (20 bits), RegA(3 bits), RegB(3 bits), RegC(3 bits), LSB

Load val format
MSB Opcode(4 bits), RegA (3 bits), value (25 bits), LSB

|Opcode|Name|Action|
|---|---|---|
|0|Conditional Move|if `$r[C] ≠ 0`, then `$r[A] = $r[B]`|
|1|Segmented Load|`$r[A] = $m[$r[B]][$r[C]]`|
|2|Segmented Store|`$m[$r[A]][$r[B]] = $r[C]`|
|3|Addition|`$r[A] = ($r[B] + $r[C]) mod 2^32`|
|4|Multiplication|`$r[A] = ($r[B] × $r[C]) mod 2^32`|
|5|Division|`$r[A] = floor($r[B] ÷ $r[C])`|
|6|Bitwise NAND|`$r[A] = ~($r[B] & $r[C])`|
|7|Halt|Stop execution.|
|8|Map Segment|New segment with `$r[C]` words; ID stored in `$r[B]`.|
|9|Unmap Segment|Unmap segment at `$r[C]`.|
|10|Output|Output `$r[C]` (must be 0–255).|
|11|Input|Read input into `$r[C]`; EOF → all 1s in 32 bits.|
|12|Load Program|Duplicate `$m[$r[B]]`, replace `$m[0]`, set PC to `$r[C]`.|

## Failure Modes 

- PC points outside `$m[0]`.
- Invalid instruction.
- Load/store to unmapped segment or out of memory
- Unmapping `$m[0]`.
- Divide by zero.
- Output > 255.
- Load from unmapped segment

----------

## Architecture:
The program mimics the design of a CPU. The program consists of a memory class, an arithmetic 
class, and a program class. The driver cfile (um.c) loads the initial program 
into memory, initializes general memory, and begins the process loop. 

The memory class consists of several functions that pertain to general memory.
It allows the user to allocate memory, deallocate memory, get memory and free
memory. Under the hood this system allows the user to allocate memory into 
a growing Hanson sequence. The Hanson sequence stores arrays of varying 
lengths (segments) with their size stored alongside. When allocating memory 
the user is given an address which pertains to the index of that segment. 
This abstraction hides the address system and general memory system.

The arithmetic class is restrained to a single function which then executes 
all other functions. Aptly named “execute”, this function takes in pointers 
to the registers used, the opcode of the command, and a value in the case of 
the load value command. This abstraction hides all of the code behind each 
instruction and what the instructions are.

The CPU class holds the registers, the program counter, and the process loop.
Before a loop is run the program counter is checked to be a valid number. 
On each iteration of this loop the general memory is queried for the next 
command to run. The command is decoded within the CPU, and then sent to the 
execute function within the arithmetic class. Depending on the function, 
arithmetic class may augment general memory or the program counter. At the 
end of the loop the program counter is incremented. At the end of the process,
all general memory is freed.

