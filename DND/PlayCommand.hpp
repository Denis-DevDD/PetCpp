#ifndef PLAYCOMMAND_HPP
#define PLAYCOMMAND_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
#include <string>
#include "Armor.hpp"
#include <cmath>

class Game;

class PlayCommand : public Command
{
private:
  std::vector<Player*> players_;
  std::vector<Enemy*> enemies_;

  std::vector<std::pair<int, int>> player_coordinats_;
  std::vector<std::pair<int, int>> enemy_coordinats_;

  int action_count_ = 1;

  std::vector<std::pair<std::string,int>> command_rating_;
  std::string chosen_command_;
  std::string chosen_player_;
  std::string chosen_item_;
  std::pair<int, int> chosen_coordinats_;
  std::pair<int, int> buffer_coordinats_loot_;
  std::vector<std::pair <WeaponName, int>> weapon_list_;
  std::vector<std::pair <Armortype, int>> armor_list_;
public:
  PlayCommand() : Command(false), action_count_(1){};

  bool execute(Game* game) override;

  void getEntityInfo(Game* game);

  void manageActionCount(Game* game);

  void calculateMove(Game *game);

  void moveDirectionEntity(Game *game,std::pair<int,int> player,std::pair<int,int> entity);

  void chooseTargetMoveEnemiesLeft(Game* game);

  std::vector<std::pair<Entity*,std::pair<int,int>>> posLockEntInRoom(Game* game);

  void chooseTargetMoveNoEnemies(Game* game);

  void chooseTargetsMove(Game* game);

  void calculateLoot(Game* game);

  void choseTarget(Game* game);

  void choseCoordinatsOrItem(Game* game);


  void calculateUse();

  void manageItemEffectivness(Player* plr);

  void manageArmorEffectivness(Item* armor, Player* plr);

  int getFieldval(Field* af);

  std::vector<int> checkforslash(Game* game, int xpos, int ypos);

  std::vector<int> checkforhit(Game* game, int xpos, int ypos);

  std::vector<int> checkforline(Game* game, int xpos, int ypos);

  std::vector<int> checkforthrust(Game* game, int xpos, int ypos);

  std::vector<int> checkforburst(Game* game, int xpos, int ypos);

  std::vector<int> checkforshot(Game* game, int xpos, int ypos);

  int manageFieldValue(Game* game, int y, int x);

  void calculateAttack(Game* game);

  void chooseAttack(Game* game);

  void manageWeaponEffectivness(Item* weapon, Player* plr);

  bool managePotions(Player* plr);

  void choseTargetsLoot(Game* game);

  void choseTargetsUse();


  int checkEquippedstuff(Player* plr);
  void checkPotion(Player* plr);
  PlayCommand(const PlayCommand&) = delete;

  ~PlayCommand() override = default;
};
#endif

