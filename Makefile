CC = gcc
CFLAGS = -Wall -g
OBJS = main.o parser.o executor.o
TARGET = mini_shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c shell.h
	$(CC) $(CFLAGS) -c parser.c

executor.o: executor.c shell.h
	$(CC) $(CFLAGS) -c executor.c

clean:
	rm -f $(TARGET) *.o
