/*
 * wrand: random numbers with weighted distribution - shared definitions
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef wrand_hh
#define wrand_hh

#include <map>
#include <functional>
#include <cstdlib>


/*
 * A parametric derivate of map for storing number distribution informations
 */

template<typename T>
class WRandMap: public std::map<T, int>
{};


/*
 * Selectors
 */

template<typename T, typename P> typename WRandMap<T>::iterator
randFind(WRandMap<T>& map, P pred)
{
  // find the base/max
  int base = map.begin()->second;

  for(WRandMap<T>::iterator it = map.begin(); it != map.end(); ++it)
  {
    if(pred(it->first))
      if(it->second < base)
	base = it->second;
  }

  // max extent
  int max = 0;
  
  for(WRandMap<T>::iterator it = map.begin(); it != map.end(); ++it)
  {
    if(pred(it->first))
      max += it->second - base + 1;
  }

  // select the element
  int cur = 0;
  int n = static_cast<int>(drand48() * max);
  WRandMap<T>::iterator ret;

  for(ret = map.begin(); ret != map.end(); ++ret)
  {
    if(pred(ret->first))
    {
      cur += ret->second - base + 1;
      if(cur >= n)
	break;
    }
  }

  return ret;
}

#endif
