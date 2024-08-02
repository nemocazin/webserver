CC = gcc
CFLAGS = -Wall -I ../libs/Reseau -I ../libs/IPC -lserv -lipc
TARGET = sioux
LIBS = -L ../libs/Reseau -L ../libs/IPC -lserv -lipc -pthread

all: $(TARGET)

$(TARGET): sioux.o analyste_http.o thread.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	
sioux.o: sioux.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
analyste_http.o: analyste_http.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
thread.o : thread.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	rm -f $(TARGET) *.o
