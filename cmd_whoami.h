#ifndef CMD_WHOAMI_H
#define CMD_WHOAMI_H

#include "command.h"
#include "command_register.h"

class CommandWhoami : public Command
{
private:

  static CommandRegister<CommandWhoami> reg;

public:

  CommandWhoami();
  virtual ~CommandWhoami();
  virtual int execute(Game& board, std::vector<std::string>& params);
};

#endif // CMD_WHOAMI_H
