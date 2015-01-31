CC=gcc
CFLAGS=-g -c -Wall
LDFLAGS=
SOURCES=main.c memory.c
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

main.o: main.c main.h memory.h cpu.h
memory.o: memory.c memory.h
