CC = gcc
CFLAGS = -Wall
TARGET = libserv.a

all: $(TARGET)

$(TARGET): serveur.o 
	ar rcs $@ $<
	
serveur.o: serveur.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(TARGET) *.o
	
