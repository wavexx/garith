/*
 * time: time and timing utilities
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef time_hh
#define time_hh

// c system headers
#include <time.h>


/*
 * Shared definitions
 */

typedef double Time;

struct TimeFrame
{
  TimeFrame()
  {}

  TimeFrame(const Time begin, const Time end)
  : begin(begin), end(end)
  {}
  

  Time begin;
  Time end;


  bool
  between(const Time t)
  {
    return (t >= begin && t <= end);
  }
};


struct AvgTime
{

public:
  AvgTime()
  : cum(0), num(0)
  {}

  AvgTime(const Time cum, const size_t num)
  : cum(cum), num(num)
  {}


  Time cum;
  size_t num;


  void
  add(const Time t)
  {
    cum += t;
    ++num;
  }

  Time
  avg()
  {
    return (cum / num);
  }
};



/*
 * Utility functions
 */

inline Time
now()
{
  timespec n;
  clock_gettime(CLOCK_REALTIME, &n);
  return (static_cast<Time>(n.tv_sec) +
      .000000001 * static_cast<Time>(n.tv_nsec));
}

#endif
