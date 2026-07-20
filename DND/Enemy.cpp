



#include "Enemy.hpp"
#include "Room.hpp"
#include "Field.hpp"
#include "Random.hpp"
#include "algorithm"



int Room::executor_x_ = 0;
int Room::executor_y_ = 0;
std::ostream& operator<<(std::ostream& srteam, const Enemy* enemy)
{
    std::cout<<enemy->type_id_<<enemy->id_<<": "
    <<enemy->current_hp_<<"/"<<enemy->max_hp_;
  return srteam;
}

Enemy::Enemy(int max_hp, int armor, int str, int vit, int id,std::string type_id)
    : Character(max_hp, armor, str, vit)
{
  id_ = id;
  setId(type_id);
  if(type_id_=="Z")
  {
    Item* dagger = getAllItems()->find("DAGG")->second;
    inventory_.insert({dagger,1});
    loot_.insert({dagger,1});
    resist_ = DamageType::NONE;
    equipped_armor_ = nullptr;
  }
  if(type_id_=="G")
  {
    Item* handaxe = getAllItems()->find("HAXE")->second;
    inventory_.insert({handaxe,1});
    loot_.insert({handaxe,1});
    Item* shortbow = getAllItems()->find("SBOW")->second;
    inventory_.insert({shortbow,1});
    loot_.insert({shortbow,1});
    Item* arrow = getAllItems()->find("ARRW")->second;
    inventory_.insert({arrow,-1});
    loot_.insert({arrow,3});
    equipped_armor_ = nullptr;
    resist_ = DamageType::NONE;
  }
  if(type_id_=="L")
  {
    Item* qfir = getAllItems()->find("QFIR")->second;
    inventory_.insert({qfir,1});
    Item* qcld = getAllItems()->find("QCLD")->second;
    inventory_.insert({qcld,1});
    Item* qacd = getAllItems()->find("QACD")->second;
    inventory_.insert({qacd,1});
    Item* qfrc = getAllItems()->find("QFRC")->second;
    inventory_.insert({qfrc,1});
    equipped_armor_ = nullptr;
    resist_ = DamageType::COLD;
    id_ = 1;
  }
  full_name_ = nametoFullName(type_id_);
}

void Enemy::doMove(Room* room)
{
  Field* i_am_on = room->getNeededEnemy(this);
  int true_y = i_am_on->getY();
  int true_x = i_am_on->getX();;
  std::vector<std::pair<int, int>> free_fields;
  std::vector<std::pair<int,int>> clockwise = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};

  for(auto direction : clockwise)
  {
    int field_y = true_y + direction.first;
    int field_x = true_x + direction.second;

    if (room->inBound(field_y, field_x, room->getDimX(), room->getDimY()))
    {
      std::pair<int, int> pair = {field_y, field_x};
      free_fields.push_back(pair);
    }
  }
  int reandom = free_fields.size();
  using namespace Oop;
  size_t move_to = Random::getInstance().getRandomNumber(reandom);
  int y_to = free_fields[move_to-1].first;
  int x_to = free_fields[move_to-1].second;
  Field* field_to = room->getFields()[y_to][x_to];

  if(field_to->getEntity() == nullptr)
  {
    field_to->setEntity(this);
  }
  else
  {
    field_to = room->findFirstFreeClockwise(y_to, x_to);
    if (field_to != nullptr)
    {
      field_to->setEntity(this);
    } else
    {
      return;
    }
  }
  if (field_to != nullptr)
  {
    room->getFields()[true_y][true_x]->setEntity(nullptr);
    Field* new_on = room->getNeededEnemy(this);
    int new_y = new_on->getY();
    int new_x = new_on->getX();;
    std::cout<<full_name_<<" "<<id_<<" ["<<type_id_<<id_<<"] moved to ("
    <<new_y+1<<","<<new_x+1<<")."<<std::endl;;
  }

}

std::string Enemy::nametoFullName(std::string& name)
{
  std::map<std::string, std::string> wtransfer={ {"Z", "Zombie"}, {"G", "Goblin"},
  {"L", "Lich"}};
  auto pair = wtransfer.find(name);
  return pair->second;
}

void Enemy::doAction(Room* room, std::vector<Player*> players)
{
  if_affecting_ = false;
  Player* target_player = chooseTarget(room, players);
  Item* weapon  = chooseWeapon(room, target_player);

  if (weapon == nullptr)
  {
    this->doMove(room);
    return;
  }
  int my_x = room->getNeededEnemy(this)->getX();
  int my_y = room->getNeededEnemy(this)->getY();
  int target_x = room->getNeededRoom(target_player->getId())->getX();
  int target_y = room->getNeededRoom(target_player->getId())->getY();
  int dif = target_x - my_x;
  doAttack(target_x, target_y, my_y, dif, weapon, room);
  std::cout<<full_name_<<" "<<id_<<" ["<<type_id_<<id_<<"] used \""
  <<weapon->getFullName()<<"\" on ("<<target_y+1<<","<<target_x+1
  <<") affecting ";
  std::sort(affected_fields_.begin(), affected_fields_.end(), Functions::compareFields);
  for (auto& affecte_ent : affected_fields_)
  {
    std::cout<<"("<<affecte_ent->getY()+1<<","
    <<affecte_ent->getX()+1<<")";
    if (affecte_ent != affected_fields_[affected_fields_.size()-1])
    {
      std::cout<<", ";
    }
  }
  std::cout<<".";
  std::cout<<std::endl;
  if_affecting_ = true;
  int damageam = weapon->damagecalc(this);
  for (auto & fields : affected_fields_)
  {
    damagedeal(fields->getX(), fields->getY(), weapon, room, damageam);
  }
}

Player* Enemy::chooseTarget(Room* room, std::vector<Player*> players)
{
  room->setExecutorY(0);
  room->setExecutorX(0);
  int my_y = room->getNeededEnemy(this)->getY();
  int my_x = room->getNeededEnemy(this)->getX();
  room->setExecutorY(my_y);
  room->setExecutorX(my_x);
  std::vector<std::pair<Player*, int>> possible_targets;
  for (Player* player : players)
  {
    if (player->getIfDead() && !player->getIfFleed())
    {
      Field* field_target = room->getNeededRoom(player->getId());
      int player_x = field_target->getX();
      int player_y = field_target->getY();
      int distance = abs(player_y - my_y) + abs(player_x - my_x);
      possible_targets.push_back({player, distance});
    }
  }
  std::sort(possible_targets.begin(), possible_targets.end(), Functions::comparePlayers);
  for (auto& target : possible_targets)
  {
    if (possible_targets[0].first != target.first && possible_targets[0].second == target.second
    && possible_targets[0].first->getHP() == target.first->getHP())
    {
        if (target.first == (room->findFirstFreeClockwisePlr(my_y, my_x)->getEntity()))
        {
          Player* temp = possible_targets[0].first;
          possible_targets[0].first = target.first;
          target.first = temp;
        }
    }
  }
  return possible_targets[0].first;
}

Item* Enemy::chooseWeapon(Room* room, Player* player)
{
  std::vector<Item*> available_weapon;
  int player_y = room->getNeededRoom(player->getId())->getY();
  int player_x = room->getNeededRoom(player->getId())->getX();
  int my_y = room->getNeededEnemy(this)->getY();
  int my_x = room->getNeededEnemy(this)->getX();

  for (auto& pair : inventory_)
  {
    if(Functions::canHit(my_y, my_x, player_y, player_x) &&
    pair.first->getItemType() == ItemType::WEAPON)
    {
      available_weapon.push_back(pair.first);
    }
    else if (pair.first->getaType() == AttackType::RANGED &&
             pair.first->getItemType() == ItemType::WEAPON)
    {
      available_weapon.push_back(pair.first);
    }
  }
  using namespace Oop;
  if (!available_weapon.empty())
  {
    int random_number = Random::getInstance().getRandomNumber(available_weapon.size()) - 1;
    return available_weapon[random_number];
  }
  return nullptr;
}

void Enemy::doAttack(int x, int y, int ythrust, int dif, Item* att, Room* room)
{
  affected_fields_.clear();
  DamagePattern pattern = att->getPattern();
  int damageam = 0;

  switch (pattern)
  {
    case DamagePattern::HIT :
    damagedeal(x, y, att, room, damageam);
    break;
    case DamagePattern::THRUST :
    attackThrust(x, y, ythrust, dif, att, room, damageam);
    break;
    case DamagePattern::LINE :
    attackLine(x, y, ythrust, dif, att, room, damageam);
    break;
    case DamagePattern::SLASH :
    attackSlash(x, y, ythrust, dif, att, room, damageam);
    break;
    case DamagePattern::SHOT :
    damagedeal(x, y, att, room, damageam);
    break;
    case DamagePattern::BURST :
    {
      damagedeal(x, y, att, room, damageam);
      damagedeal(x+1, y, att, room, damageam);
      damagedeal(x-1, y, att, room, damageam);
      damagedeal(x, y+1, att, room, damageam);
      damagedeal(x, y-1, att, room, damageam);
    }
    break;
  }
}
void Enemy::damagedeal(int x, int y, Item* att, Room* room, int damageam)
{
    if (!room->inBound(y, x, room->getDimX(), room->getDimY()))
    {
      return;
    }
    Entity* target = room->getFields()[y][x]->getEntity();
    if(!if_affecting_)
    {
      affected_fields_.push_back(room->getFields()[y][x]);
      return;
    }
  if (target != nullptr)
  {
    int res_dmg = damageam;

    DamageType elemental = att->getDType();
    DamageType resistance = target->getRes();
    if (!target->getIfPositionLocked())
    {
      if (target->getHP() <= 0)
      {
        return;
      }
      int percent = 100;
      int health = target->getHP();
      int armor_val = target->getArmorVal();
      if (target->getEquippeda() != nullptr)
      {
        target->getEquippeda()->setArmValue(target);
        if (target->getEquippeda()->getArmValue() > armor_val)
        {
          armor_val = target->getEquippeda()->getArmValue();
        }
      }

      if (resistance==elemental)
      {
        res_dmg=damageam/2;
        percent = percent/2;
      }
      res_dmg = res_dmg - armor_val;
      Functions::Damagecheck(res_dmg, health);
      health-=res_dmg;
      target->setHp(health);
      std::cout<<target->getName()<<" loses "<<res_dmg<<" health ("
      <<damageam<<" * "<<percent<<" % - "<<armor_val
      <<").";
      if (target->getHP() <= 0)
      {
        target->setHp(0);
        target->kill();
        std::cout<<std::endl;
        std::cout<<target->getName()<<" was defeated."<<std::endl;;
      }
      else
      {
        std::cout<<std::endl;
      }
    }
  }
}

void Enemy::attackThrust(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
{
  damagedeal(x, y, att, curr_room, damageam);
  switch (dif)
  {
    case (-1):
      if ((y - y0) > 0)
      {
        damagedeal(x - 1, y + 1, att, curr_room, damageam);
      }
      else if (y == y0)
      {
        damagedeal(x - 1, y, att, curr_room, damageam);
      }
      else
      {
        damagedeal(x - 1, y - 1, att, curr_room, damageam);
      }
      break;
    case (0):
      if ((y - y0) > 0)
      {
        damagedeal(x, y + 1, att, curr_room, damageam);
      }
      else if ((y - y0) < 0)
      {
        damagedeal(x, y - 1, att, curr_room, damageam);
      }
      break;
    case (1):
      if ((y - y0) > 0)
      {
        damagedeal(x + 1, y + 1, att, curr_room, damageam);
      }
      else if (y == y0)
      {
        damagedeal(x + 1, y, att, curr_room, damageam);
      }
      else
      {
        damagedeal(x + 1, y - 1, att, curr_room, damageam);
      }
      break;
  }
}

void Enemy::attackLine(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
{
  damagedeal(x, y, att, curr_room, damageam);
  switch (dif)
  {
    case (-1):
      if ((y - y0) > 0)
      {
        for (int i = y + 1; i < curr_room->getDimY(); ++i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      else if (y == y0)
      {
        for (int i = x - 1; i >= 0; --i)
        {
          damagedeal(i, y, att, curr_room, damageam);
        }
      }
      else
      {
        for (int i = y - 1; i >= 0; --i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      break;
    case (0):
      if ((y - y0) > 0)
      {
        for (int i = y + 1; i < curr_room->getDimY(); ++i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      else if ((y - y0) < 0)
      {
        for (int i = y - 1; i >= 0; --i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      break;
    case (1):
      if ((y - y0) > 0)
      {
        for (int i = y + 1; i < curr_room->getDimY(); ++i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      else if (y == y0)
      {
        for (int i = x + 1; i < curr_room->getDimX(); ++i)
        {
          damagedeal(i, y, att, curr_room, damageam);
        }
      }
      else
      {
        for (int i = y - 1; i >= 0; --i)
        {
          damagedeal(x, i, att, curr_room, damageam);
        }
      }
      break;
  }
}
void Enemy::attackSlash(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
{
  damagedeal(x, y, att, curr_room, damageam);
    switch (dif)
    {
      case -1:
        if ((y - y0) > 0)
        {
          damagedeal(x + 1, y, att, curr_room, damageam);
          damagedeal(x, y - 1, att, curr_room, damageam);
        }
        else if (y == y0)
        {
          damagedeal(x, y + 1, att, curr_room, damageam);
          damagedeal(x, y - 1, att, curr_room, damageam);
        }
        else
        {
          damagedeal(x, y + 1, att, curr_room, damageam);
          damagedeal(x + 1, y, att, curr_room, damageam);
        }
      break;
      case 0:
        if ((y - y0) > 0)
        {
          damagedeal(x + 1, y, att, curr_room, damageam);
          damagedeal(x - 1, y, att, curr_room, damageam);
        }
        else if ((y - y0) < 0)
        {
          damagedeal(x + 1, y, att, curr_room, damageam);
          damagedeal(x - 1, y, att, curr_room, damageam);
        }
      break;
      case 1:
        if ((y - y0) > 0)
        {
            damagedeal(x - 1, y, att, curr_room, damageam);
            damagedeal(x, y - 1, att, curr_room, damageam);
        }
        else if (y == y0)
        {
            damagedeal(x, y - 1, att, curr_room, damageam);
            damagedeal(x, y + 1, att, curr_room, damageam);
        }
        else
        {
            damagedeal(x, y + 1, att, curr_room, damageam);
            damagedeal(x - 1, y, att, curr_room, damageam);
        }
      break;
    }
}
