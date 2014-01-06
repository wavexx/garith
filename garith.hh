/*
 * garith: the game of arithmetic - game control
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

#ifndef garith_hh
#define garith_hh

#include "wrand.hh"
#include "time.hh"
#include "gfont.hh"
#include "operations.hh"
#include <set>


/*
 * Shared structures
 */

typedef std::map<Operation*, AvgTime> TimeMap;
typedef std::set<Operation*> OpSet;
typedef WRandMap<Kernel> KernMap;


struct Resources
{
  // font subsystem
  Font font;
  GCharMap* cm;

  // drawing parameters
  float minWidth;
  float videoRatio;
  Time animTime;
  Time minTime;

  // colors
  Color foreground;
  Color background;
  Color normal;
  Color timeout;
  Color error;
  Color bar[2];
};


struct GameData
{
  // operators
  TimeMap times;
  KernMap kernels;
  OpSet ops;

  // game mode
  enum mode_t {practice, normal, medium, hard} mode;

  // game settings
  int stackSize;
  int errorPenality;
  int timePenality;
  int balance;
  int defaultTime;

  // helper functions
  void
  update(KernMap::iterator it, int errs, Time time);
};


class State
{
protected:
  const Resources& resources;
  const Time begin;
  GameData& data;

public:
  State(const Resources& resources, const Time now, GameData& data)
  : resources(resources), begin(now), data(data)
  {}

  virtual ~State()
  {}

  // required methods
  virtual void reshape(const int w, const int h) = 0;
  virtual void display(const Time now) = 0;
  virtual void keyboard(const unsigned char key) = 0;

  // shared controls
  static void pause();
  static bool paused();
  static void quit();
};


#endif
