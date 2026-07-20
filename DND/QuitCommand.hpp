#include "Command.hpp"
#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP
#include "Game.hpp"
class Game;

class QuitCommand : public Command
{
public:
  QuitCommand() : Command(false) {};

  bool execute(Game* game) override;

  QuitCommand(const QuitCommand&) = delete;

  ~QuitCommand() override {};
};

#endif
