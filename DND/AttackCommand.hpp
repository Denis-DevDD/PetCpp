#ifndef ATTACKCOMMAND_HPP
#define ATTACKCOMMAND_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
#include "Field.hpp"
class Game;
class AttackCommand : public Command
{
  private:
  bool if_affecting_;
  std::vector<Field*> affected_fields_;
  public:
  AttackCommand() : Command(true) {};
  void damagedeal(int x, int y, Item* att, Room* room, int damageam);
  void attackThrust(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam);
  void attackLine(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam);
  void attackSlash(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam);
  bool execute(Game* game) override;
  void printAffected(Player* executor, int target_y, int target_x, Item* attcw, Room* room);

  AttackCommand(const AttackCommand&) = delete;

  ~AttackCommand() override;
};
#endif
