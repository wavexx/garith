/*
 * game: the game of arithmetic - interactive gui
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef game_hh
#define game_hh

#include "garith.hh"
#include <deque>


struct Question
{
  KernMap::iterator kernel;
  Time cum;
  int errs;
  int na;
  int r;
};

typedef std::deque<Question> QDeque;


class Game: public State
{
  // misc geometry
  float nmVSpace;
  float nmHSpace;
  Point urGeom;
  float stackW;

  // operation sizes
  int maxLenA;
  int maxLenB;
  int maxLenR;
  int maxLen;

  // game state
  enum state_t {animating, playing, gameOver} state;
  TimeFrame frame;
  Time now;

  // statistics
  int questions;
  int maxSize;
  Time avgTime;
  Time playingTime;

  // question state
  QDeque stack;
  Question question;
  Question lastQuestion;
  bool lastResult;
  Time lastRTime;
  std::string answer;
  int oldSize;

  // drawing utilities
  void drawFWString(const char* str, const Point& pos);
  void drawStackElem(const Question& q, const Point& pos);
  void drawAnswer(const float y);
  void drawBar();

  // game utilities
  void updateAvg();
  void updateTimes(const Time shift);
  void resetQuestion();
  void drawState();
  void nextState();
  void timeOut();
  void submitAnswer();

  // game states
  void initAnim();
  void initGame();
  void contGame();
  void stopGame();


  // predicate for randFind
  class NrPred: public std::unary_function<Kernel, bool>
  {
    const Game& ref;

  public:
    NrPred(Game& ref)
    : ref(ref)
    {}

    bool operator()(const Kernel& kernel) const;
  };


public:
  Game(const Resources& resources, const Time now, GameData& data);

  void reshape(const int w, const int h);
  void display(const Time now);
  void keyboard(const unsigned char key);
};

#endif
