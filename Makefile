# standard config
CPPFLAGS = -MDupdate Makedepend -I/usr/freeware/include -I/usr/local/include
LDFLAGS = -FE:template_in_elf_section -quiet_prelink
GLUT_LDADD = -L/usr/freeware/lib32 -L/usr/local/lib32 -lm -lglut -lGL -lGLU -lX11

# targets
TARGETS = f2f garith

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
	rm -rf $(TARGETS) ii_files core *.o ~* \#*

garith: $(GARITH_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(GARITH_OBJECTS) $(GARITH_LDADD)

f2f: $(F2F_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(F2F_OBJECTS) $(F2F_LDADD)


# stubs
.PHONY: all clean

sinclude Makedepend
