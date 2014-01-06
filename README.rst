===================================================
garith: a game to improve your arithmetic abilities
===================================================

`garith` is a game similar in spirit to BSD's "arithmetic" or TuxMath_. The
game asks you to perform a simple arithmetic operation within a certain
time-frame. `garith` though is geared to improve your skills by stressing the
operations you get more frequently wrong or that take longer to answer. The
statistics are saved/restored across games, so that each game is tuned for your
current arithmetic abilities.

.. _TuxMath: http://tux4kids.alioth.debian.org/tuxmath.php


Gameplay
========

In the game there's a stack of blocks (operations) on the left side, and a time
limit on the right side. At the beginning of the game the stack is empty and a
new block with an operation at random falls from the top, starting the timer.

If you answer correctly, the block is removed from the stack, and an old block
can be answered again. If your answer is wrong or you reach the time limit, a new
block falls from the top, thus increasing the stack size.

As the game progresses, the operations which you got more frequently wrong or
that took longer to answer are chosen more likely, thus slowly increasing the
difficulty. Also, the time allowed for each block converges towards your
successful response average, giving you less time to think at each new block.

When the stack hits the top, the game is over.

In `practice` mode, there is no time limit to answer a block. The only way to
lose is to repeatedly give the wrong answer.

In `medium` difficulty mode, the time available to answer an old block is
borrowed the from time left answering the last block. Thus, to clear an old
block you need to be fast enough to leave some extra time, and think ahead.

In `hard` difficulty mode, you cannot recover an old block. There's no room for
errors or delays.


Dependencies and compilation instructions
=========================================

The following software is required to build/run `garith` from source:

- OpenGL headers/libraries
- GLUT (http://www.opengl.org/resources/libraries/glut.html) or
  (preferably) FreeGLUT (http://freeglut.sourceforge.net/)
- A recent C++ compiler
- POSIX system (currently tested on OS X and Linux).

On Debian/Ubuntu, you can install all the required dependencies with::

  sudo apt-get install build-essential freeglut3-dev

You can then compile `garith` by executing ``make`` in the source directory.


Running garith
==============

You can run `garith` by executing::

  ./garith 1

in the source directory, which runs the game in `normal` playing mode.

The first argument is the game mode, which can be one of:

:-1: Shows the about/credits screen.
:0: Starts `practice` mode, which disables the time limit.
:1: Starts a normal game.
:2: Starts a game in `medium` difficulty, where the time allowed to answer an
    old block is borrowed from the time left from the previous block.
:3: Starts a game in `hard` mode, where blocks cannot be removed.

The second (optional) argument can be a list of operations that you want to
practice, and can include any of the following characters:

:"+": Addition
:"-": Subtraction
:"*": Multiplication
:"/": Division
:"\:": Division with rest

The default is "+-\*/", which includes the normal four operations. ":" enables
division with rest which shows operations such as "13 / 5", where the quotient
(answer) is 2, but with a rest of 3. By comparison, the regular division "/" is
always exact.

To practice only addition and subtraction you can start the game with::

  ./garith 1 "+-"

Please note the quotes around the second argument to escape special shell
characters.


Authors and Copyright
=====================

`garith` can be found at http://www.thregr.org/~wavexx/hacks/garith/

| `garith` is distributed under LGPL (see COPYING) WITHOUT ANY WARRANTY.
| Copyright(c) 2005-2014 by wave++ "Yuri D'Elia" <wavexx@thregr.org>.

garith's GIT repository is publicly accessible at
``git://src.thregr.org/garith``.
