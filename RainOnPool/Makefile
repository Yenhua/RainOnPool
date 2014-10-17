CC = g++
LPATH = -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib 
CFLAGS=-g -Wall
LIBS=-lGL -lglut -lGLU -ljpeg -lpng16 -lm
IPATH= -I/usr/pkg/include

all: TextureDemo
TextureDemo : TextureDemo.o G308_ImageLoader.o
	$(CC) -o TextureDemo TextureDemo.o G308_ImageLoader.o $(LIBS) $(LPATH) $(LDPATH)
TextureDemo.o :
	$(CC) -c $(CFLAGS) TextureDemo.cpp $(IPATH)  
G308_ImageLoader.o :
	$(CC) -c $(CFLAGS) G308_ImageLoader.cpp $(IPATH)
clean :
	rm -rf *.o
	rm TextureDemo
