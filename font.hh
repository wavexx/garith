/*
 * font: base font - shared definitions
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

#ifndef font_hh
#define font_hh

#include "grs.hh"
#include <vector>


/*
 * Font data
 */

struct Metrics
{
  Bounds bounds;
  float advance;
};

typedef std::vector<Point> Outline;

struct Char
{
  char c;
  Metrics metrics;
  std::vector<Outline> outlines;
};

struct Font
{
  Bounds bounds;
  std::vector<Char> chars;
};


/*
 * Character Maps
 */

class CharMap
{
  const Font& font;
  const Char** refs;

protected:
  int
  c2i(const char c)
  {
    return static_cast<int>(c);
  }


public:
  CharMap(const Font& font);
  ~CharMap() throw();

  const Char&
  getChar(const char c)
  {
    return *refs[c2i(c)];
  }

  const Font&
  getFont()
  {
    return font;
  }

  bool
  valid(const char c)
  {
    return (refs[c2i(c)] != NULL);
  }

  void
  bounds(Bounds& bounds, const char* str);

  void
  maxOf(Bounds& bounds, const char* str);
};


/*
 * Utility functions
 */

// load an Ascii Font
int
loadFontAF(Font& buf, const char* file);

#endif
