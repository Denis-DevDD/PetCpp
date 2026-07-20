#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Character.hpp"
#include <map>
#include "Item.hpp"
#include "Weapon.hpp"
#include "Player.hpp"


class Enemy : public Character
{
private:
  std::string full_name_;
  int id_;
  std::vector<Field*> affected_fields_;
  bool if_affecting_;
public:
  friend std::ostream& operator<<(std::ostream& srteam, const Enemy* enemy);
  Enemy(int max_hp, int armor, int str, int vit, int id,std::string type_id);

  std::string getId() const override
  {
    return type_id_;
  }

  int getNumber() const override
  {
    return id_;
  }

  void doMove(Room* room);

  Player* chooseTarget(Room* room, std::vector<Player*> players);

  Item* chooseWeapon(Room* room, Player* player);

  std::string nametoFullName(std::string& name);

  std::string getName() const override {return full_name_;}

  void doAction(Room* room, std::vector<Player*> player);

  void doAttack(int x, int y, int yline, int dif, Item* att, Room* curr_room);

  void attackLine(int x, int y, int yline, int dif, Item* att, Room* curr_room, int damageam);
  void attackThrust(int x, int y, int yline, int dif, Item* att, Room* curr_room, int damageam);
  void attackSlash(int x, int y, int yline, int dif, Item* att, Room* curr_room, int damageam);

  void damagedeal(int x, int y, Item* att, Room* room,  int damageam);

  Enemy(const Enemy&) = delete;

  ~Enemy() override = default;

};


#endif
