#include "cmd_whoami.h"
#include "game.h"
#include "error.h"
#include <iostream>

CommandRegister<CommandWhoami> CommandWhoami::reg("whoami");

CommandWhoami::CommandWhoami() : Command("whoami")
{
}

CommandWhoami::~CommandWhoami()
{
}


int CommandWhoami::execute(Game& board, std::vector<std::string>& params)
{
  // to get unused parameter warning away ;)
  if (params.size() == 0)
  {
    board.getSteps();
  }
  std::cout << "JustKidding" << std::endl;
  return 0;
}
