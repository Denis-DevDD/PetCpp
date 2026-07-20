#ifndef LOOTCOMMAND_HPP
#define LOOTCOMMAND_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"

class Game;

class LootCommand : public Command
{
private:
  std::vector<Entity*> entities_to_deallocate;
public:
  LootCommand() : Command(true){};

  bool execute(Game* game) override;

  LootCommand(const LootCommand&) = delete;

  ~LootCommand() override;
};


#endif

