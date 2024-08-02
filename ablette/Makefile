CC = gcc
CFLAGS = -Wall -I ../libs/IPC -lipc
TARGET = ablette
LIBS = -L ../libs/IPC -lipc -lpcap

all: $(TARGET)

$(TARGET): ablette.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS) -lipc
	
ablette.o: ablette.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(TARGET) *.o
