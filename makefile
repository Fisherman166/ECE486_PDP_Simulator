CC=gcc
CFLAGS= -g -c -Wall
LDFLAGS= -pthread
SOURCES=main.c memory.c cpu.c kb_input.c branch_trace.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=PDP8_sim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *~ *.o $(EXECUTABLE)

# to generate this list use: gcc -MM -g -c -Wall *.c

cpu.o: cpu.c cpu.h
main.o: main.c main.h memory.h cpu.h
memory.o: memory.c memory.h
kb_input.o: kb_input.c kb_input.h
branch_trace.o: branch_trace.c branch_trace.h
