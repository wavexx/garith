/*
 * garith: the game of arithmetic - game control implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

/*
 * Headers
 */

#include "game.hh"
#include "about.hh"
using std::string;
using std::map;
using std::make_pair;

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iostream>
using std::cerr;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include <GLUT/glut.h>
#include <unistd.h>


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
    buf.insert(make_pair(line.substr(0, eq), line.substr(eq + 1)));
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
    return fileNameExpand(prefix, NULL) + file;

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
  ofstream stream;
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
  exit(EXIT_SUCCESS);
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

  // write into the log
  if(stream.is_open())
  {
    stream << ::time(NULL) << "\t" << kit->first.o->cSym() << "\t" <<
      kit->first.a << " " << kit->first.b << "\t" <<
      time << "\t" << errs << std::endl;
  }
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

  // initialize default resources
  loadFontAF(resources.font, argv[1]);
  resources.cm = new GCharMap(resources.font);
  resources.minWidth = 0.5;
  resources.videoRatio = 1.;
  resources.animTime = 1.;
  resources.minTime = 0.5;
  resources.foreground.set(1., 1., 1.);
  resources.background.set(0., 0., 0.);
  resources.normal.set    (0., 1., .5);
  resources.timeout.set   (0., .5, 1.);
  resources.error.set     (1., .5, 1.);
  resources.bar[0].set    (1., .5, 0.);
  resources.bar[1].set    (1., 0., 0.);

  // initialize game data
  data.mode = static_cast<GameData::mode_t>(atoi(argv[2]));
  data.stackSize = 20;
  data.errorPenality = 5;
  data.timePenality = 1;
  data.balance = -1;

  // setup operators
  for(const char* sym = (argc > 3? argv[3]: "+-*/:"); *sym; ++sym)
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

  // open the log
  char date[16];
  time_t time = ::time(NULL);
  strftime(date, sizeof(date), "%Y%m%d", localtime(&time));
  string fileName = fileNameExpand(date, "~/.arith/");
  stream.open(fileName.c_str(), std::ios_base::out | std::ios_base::app);

  // main loop
  off = now();
  srand48(time);
  state = (data.mode < 0?
      static_cast<State*>(new About(resources, 0., data)):
      static_cast<State*>(new Game(resources, 0., data)));
  glutMainLoop();
  return EXIT_SUCCESS;
}
