
OUTFILE=shader
CFLAGS=-Wall -g `freetype-config --cflags`
LFLAGS=-g -rdynamic
LIBS=-lglfw -lGL -lGLU -lGLEW -lboost_regex `freetype-config --libs`

OBJECTS= \
	main.o \
	mesh.o \
	app.o \
	shader.o \
	globals.o \
	mesh_loader.o \
	font.o \
	exception.o


all: $(OBJECTS)
	g++ -g $(OBJECTS) $(LFLAGS) -o $(OUTFILE) $(LIBS)

.cc.o:
	g++ $< -c $(CFLAGS) -o $@

tags: FORCE
	ctags *.cc *.hh

clean:
	rm -rf $(OBJECTS) $(OUTFILE)

FORCE:

