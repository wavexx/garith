/*
 * font: base font - implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

// Interface and headers
#include "font.hh"
using std::vector;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <stdexcept.h>
using std::runtime_error;


/*
 * Character map implementation
 */

CharMap::CharMap(const Font& font)
: font(font)
{
  if(font.chars.size() > 127)
    throw runtime_error("too many characters for CharMap");

  refs = new const Char*[128];
  memset(refs, 0, sizeof(Char*) * 128);
  
  for(vector<Char>::const_iterator it = font.chars.begin();
      it != font.chars.end(); ++it)
    refs[c2i(it->c)] = &(*it);
}


CharMap::~CharMap() throw()
{
  delete []refs;
}


void
CharMap::bounds(Bounds& bounds, const char* str)
{
  // side cases
  if(!*str)
  {
    bounds.ll.x = bounds.ll.y = 0;
    bounds.ur = bounds.ll;
    return;
  }
  
  // initial bounds
  int idx = c2i(*str);
  bounds = refs[idx]->metrics.bounds;
  float shift = refs[idx]->metrics.advance;
  
  for(++str; *str; ++str)
  {
    idx = c2i(*str);
    
    const Bounds& cur = refs[idx]->metrics.bounds;
    float llx = cur.ll.x + shift;
    float urx = cur.ur.x + shift;
    
    // increase bounds
    if(llx < bounds.ll.x)
      bounds.ll.x = llx;
    if(urx > bounds.ur.x)
      bounds.ur.x = urx;
    if(cur.ll.y < bounds.ll.y)
      bounds.ll.y = cur.ll.y;
    if(cur.ur.y > bounds.ur.y)
      bounds.ur.y = cur.ur.y;
    
    shift += refs[idx]->metrics.advance;
  }
}



/*
 * Utility functions implementation
 */

int
loadFontAF(Font& buf, const char* file)
{
  ifstream in(file);
  if(!in)
    throw runtime_error(string("cannot open ") + file);

  // initial parameters
  string str;
  in >> str;
  if(str != "af")
    throw runtime_error(string(file) + " not an af file");
  
  float mul;
  in >> mul;
  if(mul != 1)
    throw runtime_error(string(file) + " unsupported byte lenght");
  in >> mul;
  if(mul > 1)
    mul = 1. / mul;
  in.get();

  Char c;
  for(c.c = in.get(); !in.eof(); c.c = in.get())
  {
    // static info
    in >> c.metrics.advance;
    c.metrics.advance *= mul;

    // outlines
    int o;
    in >> o;
    c.outlines.resize(o);

    for(vector<Outline>::iterator it = c.outlines.begin();
	it != c.outlines.end(); ++it)
    {
      // vertexes
      int n;
      in >> n;
      it->clear();
      it->reserve(n + 1);

      while(n--)
      {
	Point pnt;
	in >> pnt.x;
	in >> pnt.y;
	pnt.x *= mul;
	pnt.y *= mul;

	// update metrics
	if(it == c.outlines.begin() && !it->size())
	{
	  c.metrics.bounds.ll = c.metrics.bounds.ur = pnt;
	  if(!buf.chars.size())
	    buf.bounds.ll = buf.bounds.ur = pnt;
	}
	else
	{
	  extendBounds(c.metrics.bounds, pnt);
	  extendBounds(buf.bounds, pnt);
	}

	it->push_back(pnt);
      }

      // we always need closed contours
      if(it->size())
	it->push_back(it->front());
    }

    // fix bounds for empty glypths
    if(!c.outlines.size())
    {
      c.metrics.bounds.ll.x = c.metrics.bounds.ur.x = 0;
      c.metrics.bounds.ur = c.metrics.bounds.ll;
    }

    buf.chars.push_back(c);
    in.get();
  }

  if(buf.chars.size() > 127)
    throw runtime_error(string("too many characters in font ") + file);
  
  return buf.chars.size();
}
