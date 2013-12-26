/*
 * interp: interpolation utilities
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

#ifndef interp_hh
#define interp_hh

#include "grs.hh"
#include <cmath>


/*
 * Utility functions
 */

inline double
clamp(double v, double min = 0., double max = 0.)
{
  return (v < min? min: (v > max? max: v));
}


/*
 * Interpolation kernels
 */

inline double
interpLinear(const TimeFrame& frame, const Time now)
{
  return
    (now <= frame.begin? 0.:
	(now >= frame.end? 1.:
	    ((now - frame.begin) / (frame.end - frame.begin))));
}


inline double
interpCubic(const TimeFrame& frame, const Time now, const double degree = 2.)
{
  double i = (interpLinear(frame, now) * 2);
  return ((i < 1? std::pow(i, degree): (2. - std::pow(2. - i, degree))) / 2);
}


inline double
interpolate(double v, double a, double b)
{
  return a + v * (b - a);
}


/*
 * Point interpolation
 */

inline Point
interpolate(double v, const Point& a, const Point& b)
{
  return Point(
      interpolate(v, a.x, b.x),
      interpolate(v, a.y, b.y));
}


/*
 * Color interpolation
 */

inline Color
interpolate(double v, const Color& a, const Color& b)
{
  return Color(
      interpolate(v, a.rgba[0], b.rgba[0]),
      interpolate(v, a.rgba[1], b.rgba[1]),
      interpolate(v, a.rgba[2], b.rgba[2]),
      interpolate(v, a.rgba[3], b.rgba[3]));
}

#endif
