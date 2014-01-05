/*
 * about: the game of arithmetic - about - implementation
 * Copyright(c) 2005-2014 by wave++ "Yuri D'Elia" <wavexx@thregr.org>
 */

/*
 * Headers
 */

#include "gl.hh"
#include "about.hh"
#include "interp.hh"
using std::string;

#include <algorithm>
using std::max;


/*
 * Text data
 */

namespace
{
  const int aboutHeight = 20;
  const float linesSec = 1.5;

  const char* aboutText[] =
  {
    "garith",
    "a game of arithmetic",
    "",
    "",
    "<cast:",
    "-",
    ">author: Yuri D'Elia",
    ">concept: BSD's arithmetic",
    "",
    "",
    "<thanks:",
    "-",
    ">insert some",
    ">names here",
    "",
    "",
    "www.thregr.org/~wavexx/",
    "",
    ""
  };

  int aboutLines = (sizeof(aboutText) / sizeof(*aboutText));
}



/*
 * Implementation
 */

About::About(const Resources& resources, const Time now, GameData& data)
: State(resources, now, data)
{
  // font spacing
  VSpace = 1.1;

  // add some initial delay
  frame.begin = now + 1;
  frame.end = frame.begin + VSpace / linesSec * aboutLines;
}


void
About::reshape(const int w, const int h)
{
  // calculate enough vertical space to fit vertically
  urGeom.y = VSpace * aboutHeight;
  urGeom.x = urGeom.y * static_cast<float>(w) / h * resources.videoRatio;
  gluOrtho2D(0, urGeom.x, 0, urGeom.y);
  middle = urGeom.x / 2;
}


void
About::display(const Time now)
{
  // vertical shift and first visible line
  float shift = -VSpace + (interpLinear(frame, now) * VSpace * aboutLines);
  float base = resources.cm->getFont().bounds.ll.y;
  int begin = max(0, static_cast<int>((shift - urGeom.y)
	  / VSpace + (VSpace + base)));

  // process the needed data
  float y = shift - VSpace * begin;
  for(size_t l = begin; y > 0; ++l, y -= VSpace)
  {
    const char* line = aboutText[l];
    if(!*line)
      continue;

    // shade bottom lines
    float v = (y < 2? y / 2: 1);
    Color c = interpolate(v, resources.background, resources.foreground);
    glColor3fv(c);

    if(*line != '-')
    {
      // alignment
      float x;
      if(*line == '<')
      {
	x = VSpace;
	++line;
      }
      else if(*line == '>')
      {
	Bounds bounds;
	resources.cm->bounds(bounds, line + 1);
	x = urGeom.x - VSpace - bounds.ur.x - bounds.ll.x;
	++line;
      }
      else
      {
	Bounds bounds;
	resources.cm->bounds(bounds, line);
	x = middle - (bounds.ur.x - bounds.ll.x) / 2.;
      }

      // draw the string
      glPushMatrix();
      glTranslatef(x, y, 0.);
      glLineWidth(resources.minWidth);
      resources.cm->draw(line);
      glPopMatrix();
    }
    else
    {
      // a line
      glLineWidth(2.);
      glBegin(GL_LINES);
      glVertex2f(VSpace,  y - base);
      glVertex2f(urGeom.x - VSpace, y - base);
      glEnd();
    }
  }
}


void
About::keyboard(const unsigned char key)
{
  quit();
}
