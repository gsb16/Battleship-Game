CFLAGS = -Wall -g
OBJECTS = sockets.o jogo.o

bnaval: $(OBJECTS)

clean:
	rm -rf *.o

purge: clean
	rm -f bnaval

remake: purge
	make
	make clean
