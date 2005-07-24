# standard config
CPPFLAGS = -MD
GLUT_LDADD = -framework GLUT -framework OpenGL

# targets
TARGETS = garith # f2f

F2F_OBJECTS = f2f.o
F2F_LDADD = -lufm

GARITH_OBJECTS = grs.o font.o gfont.o game.o garith.o about.o
GARITH_LDADD = $(GLUT_LDADD)


# rules
.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<


# targets
all: $(TARGETS)

clean:
	rm -rf $(TARGETS) ii_files core *.o *.d ~* \#*

garith: $(GARITH_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(GARITH_OBJECTS) $(GARITH_LDADD)

f2f: $(F2F_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(F2F_OBJECTS) $(F2F_LDADD)


# stubs
.PHONY: all clean

sinclude *.d
