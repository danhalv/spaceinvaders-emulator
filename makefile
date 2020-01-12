CC=gcc

CFLAGS=-g -Wall

TARGET=spaceinvaders

all: $(TARGET)

$(TARGET): main.c arcade_machine.o i8080.o
	$(CC) $(CFLAGS) -o $(TARGET) main.c arcade_machine.o i8080.o `sdl2-config --cflags --libs` -Iinclude

arcade_machine.o: arcade_machine.c
	$(CC) $(CFLAGS) -c arcade_machine.c -Iinclude

i8080.o: i8080.c
	$(CC) $(CFLAGS) -c i8080.c -Iinclude

clean:
	$(RM) $(TARGET) *.o
