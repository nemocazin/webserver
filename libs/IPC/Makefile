CC = gcc
CFLAGS = -Wall
TARGET = libipc.a

all: $(TARGET)

$(TARGET): ipc.o 
	ar rcs $@ $<
	
ipc.o: ipc.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(TARGET) *.o
	
