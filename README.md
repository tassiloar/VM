

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
- **Registers and Program Counter:**  
  There are 8 general-purpose 32-bit registers that hold data and computation results. A 32-bit program counter (PC) points to the current instruction in segment 0 (the program segment) and is updated after each fetch.

- **Memory Model:**  
  Memory is dynamically managed as a collection of segments (arrays of 32-bit words). Segment 0 is initially loaded with the program instructions, while other segments can be mapped (allocated) and unmapped (deallocated) on demand. A memory module handles allocation, initialization, and reuse of segments using internal data structures like sequences and stacks.

- **Instruction Format and Execution:**  
  Instructions come in two main formats: a three-register format for most operations (such as conditional move, segmented load/store, arithmetic operations, bitwise NAND, I/O, and program load) and a load value format for directly setting a register. Bit-level operations (using bitpack utilities) are used to encode and decode these instructions from 32-bit words. During execution, the CPU fetches an instruction from segment 0, decodes it to determine the opcode and register operands, and then dispatches it to the corresponding function (found in modules like the arithmetic and instructions files).

- **Fetch-Decode-Execute Cycle:**  
  The main loop (in the CPU module) continually fetches instructions from segment 0 using the program counter, decodes them (extracting opcodes and operand fields), and executes them by invoking the appropriate operation function. The cycle terminates when a halt instruction is executed or an error condition occurs (e.g., an invalid memory access or instruction).

- **I/O Handling:**  
  Input and output instructions work with 8-bit ASCII characters, ensuring that I/O values are within the allowed range, with the output function asserting values do not exceed 255.

- **Test and Build Infrastructure:**  
  The architecture includes a set of helper functions to construct instruction sequences (in the instructions and write_instructions_file modules) that serve both as test cases and demonstrations of the emulator’s capabilities.
