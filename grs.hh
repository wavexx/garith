/*
 * grs: base graphics - shared definitions
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef grs_hh
#define grs_hh

/*
 * Generic geometric types
 */

struct Point
{
  Point(float x, float y)
  : x(x), y(y)
  {}

  Point()
  {}

  float x;
  float y;
};


struct Bounds
{
  Point ll;
  Point ur;
};



/*
 * Utility functions
 */

void
extendBounds(Bounds& buf, const Point& point);

#endif
