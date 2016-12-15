all : base_opengl



base_opengl.o : base_opengl.c base_opengl.h
	gcc -c base_opengl.c

base_opengl: base_opengl.o
	gcc base_opengl.o -o base_opengl -L/usr/X11R6/include/GL -L/usr/X11R6/lib -lGL -lGLU -lglut -lm -lX11


clean :
	rm -f base_opengl *.o

