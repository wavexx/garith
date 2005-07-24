/*
 * game: the game of arithmetic - interactive gui - implementation
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

/*
 * Headers
 */

#include "game.hh"
#include "interp.hh"
using std::string;

#include <algorithm>
using std::max;

#include <cstdlib>
using std::sprintf;

#include <GLUT/glut.h>


/*
 * Implementation
 */

void
Game::drawFWString(const char* str, const Point& pos)
{
  // draws a fixed width strings. useful for numbers.
  glPushMatrix();
  glTranslatef(pos.x, pos.y, 0.);

  for(int c = 0; *str; ++str, ++c)
  {
    // center the character
    glPushMatrix();

    const Bounds& cur = resources.cm->getChar(*str).metrics.bounds;
    glTranslatef(
	(nmHSpace * c) + (nmHSpace - (cur.ur.x - cur.ll.x)) / 2 - cur.ll.x,
	(nmVSpace - (cur.ur.y - cur.ll.y)) / 2 - cur.ll.y, 0.);
    
    resources.cm->draw(*str);
    glPopMatrix();
  }

  glPopMatrix();
}


void
Game::drawRString(const char* str, const Point& pos)
{
  // right aligned string
  Bounds bounds;
  resources.cm->bounds(bounds, str);

  glPushMatrix();

  glTranslatef(pos.x - (bounds.ur.x - bounds.ll.x) - bounds.ll.x,
      pos.y - bounds.ll.y, 0.);
  resources.cm->draw(str);

  glPopMatrix();
}


void
Game::drawStackElem(const Question& q, const Point& pos)
{
  // get the correct color
  Color color = (q.errs? resources.error:
      interpolate(q.cum / max(1., avgTime),
	  resources.normal, resources.timeout));

  // background
  color.rgba[3] = 0.5;
  glColor4fv(color);
  glBegin(GL_QUADS);
  glVertex2f(pos.x, pos.y);
  glVertex2f(pos.x, pos.y + nmVSpace);
  glVertex2f(pos.x + stackW, pos.y + nmVSpace);
  glVertex2f(pos.x + stackW, pos.y);
  glEnd();

  // contour
  color.rgba[3] = 1.;
  glColor4fv(color);
  glBegin(GL_LINE_STRIP);
  glVertex2f(pos.x, pos.y + nmVSpace);
  glVertex2f(pos.x + stackW, pos.y + nmVSpace);
  glVertex2f(pos.x + stackW, pos.y);
  glEnd();

  // string
  char fmt[32];
  sprintf(fmt, "%%%dd%%c%%-%dd", maxLenA, maxLenB);

  char* buf = new char[maxLen + 1];
  sprintf(buf, fmt, q.na, q.kernel->first.o->sym(), q.kernel->first.b);

  glColor4fv(resources.foreground);
  drawFWString(buf, pos);
  delete []buf;
}


void
Game::drawBar()
{
  Color color;

  // background
  glBegin(GL_QUADS);

  color = resources.bar[0];
  color.rgba[3] = 0.5;
  glColor4fv(color);
  glVertex2f(urGeom.x - nmVSpace, urGeom.y);
  glVertex2f(urGeom.x, urGeom.y);

  color = resources.bar[1];
  color.rgba[3] = 0.5;
  glColor4fv(color);
  glVertex2f(urGeom.x, 0);
  glVertex2f(urGeom.x - nmVSpace, 0);

  glEnd();

  if(state == playing)
  {
    float r = interpLinear(frame, now);
    float ry = urGeom.y - r * urGeom.y;

    // hilight
    color = interpolate(r, resources.bar[0], resources.bar[1]);
    glColor4fv(color);
    glBegin(GL_QUADS);
    glVertex2f(urGeom.x - nmVSpace, 0);
    glVertex2f(urGeom.x - nmVSpace, ry);
    glVertex2f(urGeom.x, ry);
    glVertex2f(urGeom.x, 0);
    glEnd();

    // timer
    glPushMatrix();
    const Font& font = resources.cm->getFont();
    glTranslatef(urGeom.x - nmVSpace - font.bounds.ll.y,
	urGeom.y + font.bounds.ll.x, 0);
    glRotatef(-90., 0., 0., 1.);
    char buf[sizeof(int) * 4];
    sprintf(buf, "%d", static_cast<int>(frame.end - now));
    glColor4fv(resources.background);
    resources.cm->draw(buf);
    glPopMatrix();
  }

  // separator
  glColor4fv(resources.foreground);
  glBegin(GL_LINES);
  glVertex2f(urGeom.x - nmVSpace, 0);
  glVertex2f(urGeom.x - nmVSpace, urGeom.y);
  glEnd();
}


void
Game::drawStats()
{
  // some useful data
  int num = 1;
  float anchor = urGeom.x - nmVSpace - nmHSpace;
  char buf[32];
  int len;

  glColor4fv(resources.foreground);

  // playing time
  int seconds = static_cast<int>(playingTime);
  len = sprintf(buf, "%d:%02d", seconds / 60, seconds % 60) - 1;
  drawRString("playing time:", Point(anchor, urGeom.y - nmVSpace * num++));
  drawFWString(buf, Point(anchor - nmHSpace * len,
	  urGeom.y - nmVSpace * num++));

  // questions
  len = sprintf(buf, "%d", questions) - 1;
  drawRString("questions:", Point(anchor, urGeom.y - nmVSpace * num++));
  drawFWString(buf, Point(anchor - nmHSpace * len,
	  urGeom.y - nmVSpace * num++));
  
  // max size
  len = sprintf(buf, "%d", maxSize) - 1;
  drawRString("max size:", Point(anchor, urGeom.y - nmVSpace * num++));
  drawFWString(buf, Point(anchor - nmHSpace * len,
	  urGeom.y - nmVSpace * num++));

  // gameOver/paused
  if(paused())
  {
    glColor4fv(resources.bar[0]);
    drawRString("paused", Point(anchor, urGeom.y - nmVSpace * num++));
  }
  else if(state == gameOver)
  {
    glColor4fv(resources.bar[1]);
    drawRString("game over", Point(anchor, urGeom.y - nmVSpace * num++));
  }
}


void
Game::drawAnswer(const float y)
{
  string buf("=");
  buf += answer;

  glColor4fv(resources.foreground);
  drawFWString(buf.c_str(), Point(stackW, y));
}


void
Game::updateAvg()
{
  // time averages
  AvgTime res;
  for(TimeMap::iterator it = data.times.begin(); it != data.times.end(); ++it)
  {
    // only for the active operators
    if(data.ops.find(it->first) != data.ops.end())
      res.add(it->second.avg());
  }
  avgTime = res.avg();
}


void
Game::updateTimes(const Time shift)
{
  playingTime += shift;
  question.cum += shift;
}


void
Game::resetQuestion()
{
  question.kernel = randFind(data.kernels, NrPred(*this));
  const Kernel& kernel(question.kernel->first);
  question.r = kernel.o->result(kernel.a, kernel.b, question.na);
  question.cum = 0.;
  question.errs = 0;
}


void
Game::initAnim()
{
  // basic animation data
  state = animating;
  frame.begin = now;
  frame.end = now + resources.animTime;
}


void
Game::initGame()
{
  // basic animation data
  state = playing;

  if(lastResult && data.mode == GameData::medium &&
      static_cast<int>(stack.size()) < oldSize)
  {
    // only continue when an element was successfully removed
    frame.end = now + lastRTime;
    frame.begin = frame.end - avgTime;
  }
  else
  {
    frame.begin = now;
    frame.end = now + avgTime;
  }
}


void
Game::drawState()
{
  // always draw the stack
  QDeque::iterator it = stack.begin();
  for(int i = 0; it != stack.end(); ++i, ++it)
    drawStackElem(*it, Point(0, nmVSpace * i));

  // common variables
  float stackHead = nmVSpace * stack.size();

  if(state == animating)
  {
    // animating parts
    float oldHead = nmVSpace * oldSize;
    float ci = interpCubic(frame, now);

    // dropping element
    if(static_cast<int>(stack.size()) >= oldSize)
      drawStackElem(question, interpolate(ci,
	      Point(0, urGeom.y), Point(0, stackHead)));
    else
      drawStackElem(question, Point(0, stackHead));

    // removing element
    if(lastResult)
      drawStackElem(lastQuestion, Point(-ci * stackW * 2, oldHead));

    // answer symbol
    drawAnswer(interpolate(ci, oldHead, stackHead));
  }
  else
  {
    // current element and answer
    drawStackElem(question, Point(0, stackHead));
    drawAnswer(stackHead);
  }

  // remaining time and stats
  drawBar();
  drawStats();
}


void
Game::nextState()
{
  if(state == animating)
    initGame();
  else if(state == playing && data.mode != GameData::practice)
    timeOut();
}


void
Game::contGame()
{
  // check game status
  if(static_cast<int>(stack.size()) == data.stackSize)
    stopGame();
  else
  {
    // update statistics
    ++questions;

    // only update on success. this could be used for calculating correctly
    // a better score in the future.
    if(static_cast<int>(stack.size()) <= oldSize && oldSize > maxSize)
      maxSize = oldSize;

    // continue
    answer.clear();
    initAnim();
  }
}


void
Game::stopGame()
{
  // change the state
  state = gameOver;

  // commit errors on the stack
  for(QDeque::const_iterator it = stack.begin(); it != stack.end(); ++it)
  {
    if(it->errs)
      data.update(it->kernel, it->errs, it->cum);
  }
}


void
Game::timeOut()
{
  // update stack
  lastResult = false;
  lastRTime = 0.;
  oldSize = stack.size();
  stack.push_back(question);
  resetQuestion();
  contGame();
}


void
Game::submitAnswer()
{
  // check result
  int r = strtol(answer.c_str(), NULL, 10);
  lastResult = (r == question.r);
  lastRTime = frame.end - now;
  oldSize = stack.size();

  if(lastResult)
  {
    // correct, update the stack
    lastQuestion = question;
    data.update(question.kernel, question.errs, question.cum);
    updateAvg();
    
    if(stack.size() &&
	(data.mode != GameData::medium || lastRTime > resources.minTime) &&
	(data.mode != GameData::hard))
    {
      // recover and old question
      question = stack.back();
      stack.pop_back();
    }
    else
      resetQuestion();
  }
  else
  {
    // wrong answers
    ++question.errs;
    stack.push_back(question);
    resetQuestion();
  }

  contGame();
}


bool
Game::NrPred::operator()(const Kernel& kernel) const
{
  // exclude disabled operations
  if(ref.data.ops.find(kernel.o) == ref.data.ops.end())
    return false;

  // when in hard and/or limit conditions the stack is ignored
  if(ref.data.mode == GameData::hard ||
      ref.stack.size() >= ref.data.kernels.size())
    return true;

  // exclude all kernels already present in the stack
  for(QDeque::const_iterator it = ref.stack.begin();
      it != ref.stack.end(); ++it)
    if(kernel == it->kernel->first)
      return false;

  return true;
}


Game::Game(const Resources& resources, const Time now, GameData& data)
: State(resources, now, data)
{
  // font spacing
  Bounds nmBounds;
  resources.cm->maxOf(nmBounds, "0123456789+-*/=~");
  nmHSpace = nmBounds.ur.x - nmBounds.ll.x;
  nmHSpace += nmHSpace * 0.1;
  nmVSpace = 1.1;

  // sizes
  maxLenA = maxLenB = maxLenR = 0;
  for(KernMap::iterator it = data.kernels.begin();
      it != data.kernels.end(); ++it)
  {
    // only consider active operations
    if(data.ops.find(it->first.o) == data.ops.end())
      continue;

    int na;
    char buf[sizeof(int) * 4];
    int r = it->first.o->result(it->first.a, it->first.b, na);
    maxLenA = max(maxLenA, sprintf(buf, "%d", na));
    maxLenB = max(maxLenB, sprintf(buf, "%d", it->first.b));
    maxLenR = max(maxLenR, sprintf(buf, "%d", r));
  }

  // misc geometry
  maxLen = maxLenA + 1 + maxLenB;
  stackW = nmHSpace * maxLen;

  // initial stats
  questions = 0;
  maxSize = 0;
  playingTime = 0.;

  // initial state
  this->now = now;
  lastResult = false;
  lastRTime = 0.;
  oldSize = -1;
  updateAvg();
  resetQuestion();
  initAnim();
}


void
Game::reshape(const int w, const int h)
{
  // calculate enough vertical space to fit the stack exactly
  urGeom.y = nmVSpace * data.stackSize;
  urGeom.x = urGeom.y * static_cast<float>(w) / h * resources.videoRatio;
  gluOrtho2D(0, urGeom.x, 0, urGeom.y);
}


void
Game::display(const Time now)
{
  // update times
  if(state == playing)
    updateTimes(now - this->now);
  this->now = now;

  // update status
  if(now > frame.end)
    nextState();

  // actual drawing
  drawState();
}


void
Game::keyboard(const unsigned char key)
{
  // quit
  if(key == 27 || key == 'q' || key == 'Q')
  {
    stopGame();
    quit();
    return;
  }

  // pause
  if(state != gameOver && (key == 'p' || key == 'P'))
  {
    pause();
    return;
  }

  // the following only works when actively playing
  if(state != playing || paused())
    return;

  if(key == 8 || key == 127)
  {
    // backspace
    if(answer.size())
      answer.resize(answer.size() - 1);
  }
  else if(key >= '0' && key <= '9')
  {
    // number
    if(static_cast<int>(answer.size()) < maxLenR)
      answer += key;
  }
  else if(key == 13)
  {
    // empty answers as timeouts
    if(answer.size())
      submitAnswer();
    else
      timeOut();
  }
}
