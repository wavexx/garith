/*
 * garith: the game of arithmetic - game control implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

/*
 * Headers
 */

#include "game.hh"
using std::string;
using std::map;
using std::make_pair;


#include <fstream>
using std::ifstream;

#include <iostream>
using std::cerr;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include <GL/freeglut.h>


/*
 * Settings
 */

int
loadPairs(map<string, string>& buf, const char* file)
{
  ifstream in(file);
  if(!in)
    throw runtime_error(string("cannot open ") + file);

  int n = 0;
  string line;
  for(; std::getline(in, line); ++n)
  {
    // empty lines
    if(!line.size())
      continue;
    if(line[0] == '#')
      continue;

    string::size_type eq = line.find('=');
    if(eq == string::npos || eq == 0)
      throw runtime_error(string("syntax error in ") + file);

    // insert the new element
    buf.insert(std::make_pair(line.substr(0, eq), line.substr(eq + 1)));
  }

  return n;
}


string
fileNameExpand(const char* file, const char* prefix = NULL)
{
  // expansions
  if(file[0] == '~')
    return string(getenv("HOME")) + (file + 1);
  else if(file[0] != '/')
    return string(prefix) + file;

  return file;
}


/*
 * GLUT handlers
 */

namespace
{
  Resources resources;
  GameData data;

  Time off = 0.;
  bool paused = false;
  State* state;
}


void
reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  state->reshape(w, h);
  glMatrixMode(GL_MODELVIEW);
}


void
display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  state->display((paused? off: now() - off));
  glutSwapBuffers();
}


void
keyboard(unsigned char c, int, int)
{
  state->keyboard(c);
}



/*
 * State implementation
 */

void
State::pause()
{
  off = now() - off;

  if(::paused)
  {
    ::paused = false;
    glutIdleFunc(glutPostRedisplay);
  }
  else
  {
    ::paused = true;
    glutIdleFunc(NULL);
    glutPostRedisplay();
  }
}


bool
State::paused()
{
  return ::paused;
}


void
State::quit()
{
  glutLeaveMainLoop();
}



/*
 * Helpers functions implementation
 */

void
GameData::update(KernMap::iterator kit, int errs, Time time)
{
  TimeMap::iterator tit = times.find(kit->first.o);
  int n = static_cast<int>(time / tit->second.avg());
  tit->second.add(time);

  if(n || errs)
  {
    kit->second += (n * timePenality);
    kit->second += (errs * errorPenality);
  }
  else
    kit->second += balance;
}



/*
 * Initialization
 */

int
main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutCreateWindow(argv[0]);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(glutPostRedisplay);
  glutKeyboardFunc(keyboard);

  // initialize resources
  loadFontAF(resources.font, argv[1]);
  resources.cm = new GCharMap(resources.font);
  resources.minWidth = 0.2;
  resources.videoRatio = 0.777;
  resources.animTime = 1.;
  resources.foreground.set(1., 1., 1.);
  resources.background.set(0., 0., 0.);
  resources.normal.set    (0., 1., .5);
  resources.timeout.set   (0., .5, 1.);
  resources.error.set     (1., .5, 1.);
  resources.bar[0].set    (1., .5, 0.);
  resources.bar[1].set    (1., 0., 0.);

  // initialize game data
  data.mode = GameData::normal;
  data.stackSize = 20;
  data.errorPenality = 5;
  data.timePenality = 1;
  data.balance = -1;

  // setup operators
  for(const char* sym = "+-*/"; *sym; ++sym)
  {
    Operation* op = opFromSym(*sym);
    data.times.insert(make_pair(op,
	    AvgTime(20. * data.stackSize, data.stackSize)));
    data.ops.insert(op);

    for(int a = 2; a != 10; ++a)
      for(int b = 2; b != 10; ++b)
      {
	Kernel buf = {a, op, b};
	data.kernels.insert(make_pair(buf, 0));
      }
  }

  // set initial GL parameters
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LINE_SMOOTH);
  glLineWidth(resources.minWidth);
  glClearColor(resources.background[0], resources.background[1],
      resources.background[2], resources.background[3]);

  // main loop
  off = now();
  srand48(time(NULL));
  state = new Game(resources, 0., data);
  glutMainLoop();
  return EXIT_SUCCESS;
}
