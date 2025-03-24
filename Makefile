CC      = gcc
CFLAGS  = -Wall -Wextra -O1 -std=c99
LDFLAGS = -lm

# source files for the UM 
UM_SRCS = vm.c instructions.c arithmetic.c memory.c bitpack.c cpu.c seq.c stack.c
UM_OBJS = $(UM_SRCS:.c=.o)

# source files for the test generator
TEST_SRCS = write_instructions_file.c instructions.c arithmetic.c memory.c bitpack.c cpu.c seq.c stack.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

.PHONY: all clean

all: um test

um: $(UM_OBJS)
	$(CC) $(CFLAGS) -o $@ $(UM_OBJS) $(LDFLAGS)

test: $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o um test
