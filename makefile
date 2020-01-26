CC=gcc
CFLAGS=-std=c99 -g -Wall -pedantic -Iinclude -Ii8080-emulator/include

TARGET=spaceinvaders

all: $(TARGET)

$(TARGET): main.c arcade_machine.o i8080.o
	$(CC) $(CFLAGS) -o $(TARGET) main.c arcade_machine.o i8080.o `sdl2-config --cflags --libs`

arcade_machine.o: arcade_machine.c
	$(CC) $(CFLAGS) -c arcade_machine.c

i8080.o: i8080-emulator/i8080.c
	$(CC) $(CFLAGS) -c i8080-emulator/i8080.c

clean:
	$(RM) $(TARGET) *.o
