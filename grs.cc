/*
 * grs: base graphics - implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

// Interface and headers
#include "grs.hh"


// Implementation
void
extendBounds(Bounds& buf, const Point& point)
{
  if(buf.ll.x > point.x)
    buf.ll.x = point.x;
  if(buf.ur.x < point.x)
    buf.ur.x = point.x;
  if(buf.ll.y > point.y)
    buf.ll.y = point.y;
  if(buf.ur.y < point.y)
    buf.ur.y = point.y;
}
