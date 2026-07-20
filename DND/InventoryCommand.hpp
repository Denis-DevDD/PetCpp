#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
#ifndef INVENTORYCOMMAND_HPP
#define INVENTORYCOMMAND_HPP
class Game;
class InventoryCommand : public Command
{
private:
  Room* current_room_;
  std::string id_;
  Player* player_;
  std::map<Item*, int> inv_;
  std::vector<std::pair<Item*, int>> armors_;
  std::vector<std::pair<Item*, int>> weapons_;
  std::vector<std::pair<Item*, int>> consumes_;
  Item* equipped_armor_;
  Item* equipped_weapon_;

public:
  InventoryCommand() : Command(false) {};

  bool execute(Game* game) override;

  InventoryCommand(const InventoryCommand&) = delete;

  ~InventoryCommand() override {};

  void SafeAllNecessary(Game* game);

  void printInv();
};
#endif
