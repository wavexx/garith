/*
 * about: the game of arithmetic - about
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef about_hh
#define about_hh

#include "garith.hh"


class About: public State
{
  // misc geometry
  float VSpace;
  Point urGeom;
  float middle;
  TimeFrame frame;

public:
  About(const Resources& resources, const Time now, GameData& data);

  void reshape(const int w, const int h);
  void display(const Time now);
  void keyboard(const unsigned char key);
};

#endif
