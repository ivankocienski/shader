
OUTFILE=shader
CFLAGS=-Wall -g
LFLAGS=-g
LIBS=-lglfw -lGL -lGLU -lGLEW -lboost_regex

OBJECTS= \
	main.o \
	mesh.o \
	app.o \
	shader.o \
	globals.o \
	mesh_loader.o


all: $(OBJECTS)
	g++ -g $(OBJECTS) $(LFLAGS) -o $(OUTFILE) $(LIBS)

.cc.o:
	g++ $< -c $(CFLAGS) -o $@

tags: FORCE
	ctags *.cc *.hh

clean:
	rm -rf $(OBJECTS) $(OUTFILE)

FORCE:

