
OUTFILE=shader
CFLAGS=-Wall -g
LFLAGS=-g
LIBS=-lglfw -lGL -lGLU -lGLEW

OBJECTS= \
	main.o \
	mesh.o \
	app.o


all: $(OBJECTS)
	g++ -g $(OBJECTS) $(LFLAGS) -o $(OUTFILE) $(LIBS)

.c.o:
	g++ $< -c $(CFLAGS) -o $@

tags: FORCE
	ctags *.cc *.hh

clean:
	rm -rf $(OBJECTS) $(OUTFILE)

FORCE:

