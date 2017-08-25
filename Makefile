TARGET		= pidemo
OBJS_TARGET	= main.o Gtruetype.o Gtexture.o

CFLAGS = -O3 -fpermissive -ffast-math `sdl-config --cflags` `pkg-config ftgl --cflags`
LDFLAGS = 
LIBS = `sdl-config --libs` `pkg-config ftgl --libs` -lc -lm -lGL -lGLU -lglut -lstdc++

include Makefile.in
