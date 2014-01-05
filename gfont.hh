/*
 * gfont: graphical font rendition
 * Copyright(c) 2005-2014 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

#ifndef gfont_hh
#define gfont_hh

#include "font.hh"
#include "gl.hh"


/*
 * Basic rendering functions
 */

void
renderCharStroked(const Char& c);

void
renderCharFilled(const Char& c);


/*
 * Graphical Character Map
 */

class GCharMap: public CharMap
{
  GLuint base;

public:
  // fill modes
  enum fill_t {filled, stroked, filledstroked};

  GCharMap(const Font& font);
  ~GCharMap() throw();

  void
  draw(const char c, const fill_t mode = filledstroked);

  void
  draw(const char* str, const fill_t mode = filledstroked);
};

#endif
