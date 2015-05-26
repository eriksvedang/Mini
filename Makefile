CFLAGS=-I/usr/local/include
LDFLAGS=-L/usr/local/lib/
LDLIBS=-lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit

all: main.o;
	clang main.c -o Mini $(CFLAGS) $(LDFLAGS) $(LDLIBS)

run:
	./Mini

clean: rm -f *.o main

