/*
 * gfont: graphical font rendition - implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

// Interface and headers
#include "gfont.hh"
using std::vector;

#include <GL/glu.h>


/*
 * Render functions
 */

void
renderCharFilled(const Char& c)
{
  GLUtesselator* tess(gluNewTess());
  gluTessCallback(tess, GLU_TESS_BEGIN,
      reinterpret_cast<void (*)()>(glBegin));
  gluTessCallback(tess, GLU_TESS_VERTEX,
      reinterpret_cast<void (*)()>(glVertex2fv));
  gluTessCallback(tess, GLU_TESS_END,
      reinterpret_cast<void (*)()>(glEnd));
  gluTessBeginPolygon(tess, NULL);

  for(vector<Outline>::const_iterator it = c.outlines.begin();
      it != c.outlines.end(); ++it)
  {
    gluTessBeginContour(tess);
    for(Outline::const_iterator xt = it->begin(); xt != it->end(); ++xt)
    {
      double data[3];
      data[0] = xt->x;
      data[1] = xt->y;
      data[2] = 0.;

      gluTessVertex(tess, data, const_cast<Point*>(&(*xt)));
    }
    gluTessEndContour(tess);
  }
  gluTessEndPolygon(tess);
  gluDeleteTess(tess);
}


void
renderCharStroked(const Char& c)
{
  for(vector<Outline>::const_iterator it = c.outlines.begin();
      it != c.outlines.end(); ++it)
  {
    glBegin(GL_LINE_STRIP);
    for(Outline::const_iterator xt = it->begin(); xt != it->end(); ++xt)
      glVertex2f(xt->x, xt->y);
    glEnd();
  }
}



/*
 * Graphical character map implementation
 */

GCharMap::GCharMap(const Font& font)
: CharMap(font)
{
  base = glGenLists(256);

  for(vector<Char>::const_iterator it = font.chars.begin();
      it != font.chars.end(); ++it)
  {
    int idx = c2i(it->c);
    
    // filled char
    glNewList(base + idx * 2, GL_COMPILE);
    renderCharFilled(*it);
    glEndList();
    
    // outline char
    glNewList(base + idx * 2 + 1, GL_COMPILE);
    renderCharStroked(*it);
    glEndList();
  }
}


GCharMap::~GCharMap() throw()
{
  glDeleteLists(base, 256);
}


void
GCharMap::draw(const char c, const fill_t mode)
{
  switch(mode)
  {
  case filled:
    glCallList(base + c2i(c) * 2);
    break;
    
  case stroked:
    glCallList(base + c2i(c) * 2 + 1);
    break;
    
  default:
    glCallList(base + c2i(c) * 2);
    glCallList(base + c2i(c) * 2 + 1);
  }
}


void
GCharMap::draw(const char* str, const fill_t mode)
{
  for(; *str; ++str)
  {
    draw(*str, mode);
    glTranslatef(getChar(*str).metrics.advance, 0., 0.);
  }
}
