//------------------------------------------------------------------------------
// cmd_Solve.h
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#ifndef COMMANDSOLVE_H
#define COMMANDSOLVE_H

#include "command.h"
#include "command_register.h"

//------------------------------------------------------------------------------
// CommandSolve derived by class Command
// Command to solve game.
//
class CommandSolve : public Command
{
private:

  //----------------------------------------------------------------------------
  // Object that registers Command in its constructor.
  //
  static CommandRegister<CommandSolve> reg;

public:

  //----------------------------------------------------------------------------
  // Constructor
  //
  CommandSolve();

  //----------------------------------------------------------------------------
  // Destructor
  //
  virtual ~CommandSolve();

  //----------------------------------------------------------------------------
  // Execute Method
  // Performs the Command Solve.
  // @param board The current board.
  // @param params Contains the parameters entered in the console.
  //            1. Parameter: Name of the command.
  // @return Returns 0 if finished
  //
  virtual int execute(Game& board, std::vector<std::string>& params);
};

#endif // COMMANDSolve_H
