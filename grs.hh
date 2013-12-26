/*
 * grs: base graphics - shared definitions
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
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
 * Color definitions
 */

struct Color
{
  Color(float r = 0., float g = 0., float b = 0., float a = 1.)
  {
    set(r, g, b, a);
  }

  float rgba[4];

  void
  set(float r = 0., float g = 0., float b = 0., float a = 1.)
  {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
  }

  operator float*()
  {
    return rgba;
  }

  operator const float*() const
  {
    return rgba;
  }
};



/*
 * Utility functions
 */

void
extendBounds(Bounds& buf, const Point& point);

#endif
