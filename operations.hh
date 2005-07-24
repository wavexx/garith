/*
 * operators: generic binary operators
 * Copyright(c) 2005 by wave++ "Yuri D'Elia" <wavexx@users.sf.net>
 */

#ifndef operations_hh
#define operations_hh

#include <stdexcept>
#include <cstdlib>


/*
 * Base classes
 */

typedef char OpSym;

class Operation
{
public:
  virtual OpSym sym() const = 0;
  virtual int result(const int a, const int b, int& na) const = 0;

  virtual OpSym
  cSym() const
  {
    return sym();
  }

  bool
  operator<(const Operation& r) const
  {
    return (cSym() < r.cSym());
  }
};


struct Kernel
{
  // basic data
  int a;
  Operation* o;
  int b;

  // predicates
  bool
  operator==(const Kernel& r) const
  {
    return (a == r.a && o == r.o && b == r.b);
  }
  
  bool
  operator<(const Kernel& r) const
  {
    return (a < r.a || *o < *r.o || b < r.b);
  }
};



/*
 * Operations
 */

class Addition: public Operation
{
public:
  OpSym sym() const
  {
    return '+';
  }

  int
  result(const int a, const int b, int& na) const
  {
    na = a;
    return (a + b);
  }
};


class Subtraction: public Operation
{
public:
  OpSym sym() const
  {
    return '-';
  }

  int
  result(const int a, const int b, int& na) const
  {
    na = a + b;
    return (na - b);
  }
};


class Multiplication: public Operation
{
public:
  OpSym sym() const
  {
    return '*';
  }

  int
  result(const int a, const int b, int& na) const
  {
    na = a;
    return (a * b);
  }
};


class Division: public Operation
{
public:
  OpSym sym() const
  {
    return '/';
  }

  int
  result(const int a, const int b, int& na) const
  {
    na = a * b;
    return (na / b);
  }
};


class ImpDivision: public Operation
{
public:
  OpSym sym() const
  {
    return '/';
  }

  OpSym cSym() const
  {
    return ':';
  }

  int
  result(const int a, const int b, int& na) const
  {
    na = a * b + static_cast<int>(drand48() * (b - 2) + 1);
    return (na / b);
  }
};


/*
 * Utility functions
 */

inline Operation*
opFromSym(const OpSym sym)
{
  switch(sym)
  {
  case '+': return new Addition();
  case '-': return new Subtraction();
  case '*': return new Multiplication();
  case '/': return new Division();
  case ':': return new ImpDivision();
  }

  throw std::runtime_error("unknown operation symbol");
}

#endif
