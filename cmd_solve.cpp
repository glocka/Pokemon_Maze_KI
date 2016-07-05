//------------------------------------------------------------------------------
// cmd_solve.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "cmd_solve.h"
#include "game.h"
#include "error.h"

//------------------------------------------------------------------------------
CommandRegister<CommandSolve> CommandSolve::reg("solve");

//------------------------------------------------------------------------------
CommandSolve::CommandSolve() : Command("solve")
{
}

//------------------------------------------------------------------------------
CommandSolve::~CommandSolve()
{
}

//------------------------------------------------------------------------------
int CommandSolve::execute(Game &board, std::vector<std::string> &params)
{
  if (params.size() > 2)
  {
    Error::show(Error::WRONG_PARAMETER_COUNT);
    return 0;
  }

  if (!board.isLoaded())
  {
    Error::show(Error::NO_MAZE_LOADED);
    return 0;
  }

  if (params.size() == 2 && params[1] == "silent")
  {
    board.solve(true);
  }
  else
  {
    board.solve(false);
  }
  return 0;
}
