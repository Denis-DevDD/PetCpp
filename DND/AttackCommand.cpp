#include "Exceptions.hpp"
#include "AttackCommand.hpp"
#include <iostream>
#include "Functions.hpp"
#include <algorithm>
void AttackCommand::damagedeal(int x, int y, Item* att, Room* room, int damageam)
{
  if (!room->inBorders(y,x))
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
      res_dmg = res_dmg-armor_val;
      Functions::Damagecheck(res_dmg, health);
      health-=res_dmg;
      target->setHp(health);
      std::cout<<target->getName();
      if (target->getId() == "Z" || target->getId() == "G" || target->getId() == "L")
      {
          std::cout<<" "<<target->getNumber();
      }
      std::cout<<" loses "<<res_dmg<<" health ("
      <<damageam<<" * "<<percent<<" % - "<<armor_val
      <<").";

      if (target->getHP() <= 0)
      {
        target->setHp(0);
        target->kill();
        std::cout<<std::endl;
        std::cout<<target->getName();
      if (target->getId() != "W" || target->getId() != "B" || target->getId() != "R")
      {
          std::cout<<" "<<target->getNumber();
      }
      std::cout<<" was defeated."<<std::endl;
      }
      else
      {
        std::cout<<std::endl;
      }
    }
  }
}
void AttackCommand::attackThrust(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
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
void AttackCommand::attackLine(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
{
  damagedeal(x, y, att, curr_room, damageam);

  int dx = 0;
  int dy = 0;

  if (dif == -1)
  {
    dx = (y - y0 == 0) ? -1 : 0;
    dy = (y - y0 > 0) ? 1 : -1;
  }
  else if (dif == 0)
  {
    dx = 0;
    dy = (y - y0 > 0) ? 1 : (y - y0 < 0) ? -1 : 0;
  }
  else if (dif == 1)
  {
    dx = (y - y0 == 0) ? 1 : 0;
    dy = (y - y0 > 0) ? 1 : -1;
  }

  int curr_x = x + dx;
  int curr_y = y + dy;

  while (curr_x >= 0 && curr_x < curr_room->getDimX() && curr_y >= 0 && curr_y < curr_room->getDimY())
  {
    damagedeal(curr_x, curr_y, att, curr_room, damageam);
    curr_x += dx;
    curr_y += dy;
  }
}

void AttackCommand::attackSlash(int x, int y, int y0, int dif, Item* att, Room* curr_room, int damageam)
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

bool AttackCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 3)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }

  if (input[2].find_first_of(',') == std::string::npos)
  {
    throw WrongParamExceptions("E_INVALID_PARAM");
  }

  if (game->getParser()->getIfThrow() == true)
  {
    throw WrongParamExceptions("E_INVALID_PARAM");
  }
  Room* current_room = game->getCurrentRoom();
  std::string abbreviation = game->getParser()->getUserInput()[1];
  Player* attacker=game->getPlayer(abbreviation);
  if (current_room->getNeededRoom(abbreviation) == nullptr)
  {
    throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
  }
  Item* attcw=attacker->getEquippedw();
  SubWeaponType weapon_type_ = SubWeaponType::MELEE;
  if (attcw != nullptr)
  {
    weapon_type_ = attcw->getSubType();
  }
  int x1=current_room->getNeededRoom(attacker->getId())->getX();
  int y1=current_room->getNeededRoom(attacker->getId())->getY();


  int x2 = game->getParser()->getCrd()[1]-1;
  int y2 = game->getParser()->getCrd()[0]-1;
  if (!current_room->inBorders(y1, x1) || !current_room->inBorders(y2, x2))
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  current_room->checkRange(y2+1, x2);
  current_room->checkRange(y2, x2);
  if_affecting_ = false;
  affected_fields_.clear();
  int damageam = 0;

    if (attcw == nullptr)
    {
      throw CommandExceptions("E_ATTACK_NO_WEAPON_EQUIPPED");
    }
    else
    {
      DamagePattern ptrn = attcw->getPattern();
      if (attcw->getaType() == AttackType::MELEE)
      {
        if (!Functions::canHit(y1, x1, y2, x2))
        {
          throw CommandExceptions("E_INVALID_POSITION");
        }
          else
          {
             int direction= x2-x1;
            if (ptrn == DamagePattern::HIT)
            {
              damagedeal(x2, y2, attcw, current_room, damageam);
            }
            else if (ptrn == DamagePattern::THRUST)
            {
              attackThrust(x2, y2, y1, direction, attcw, current_room, damageam);
            }
            else if (ptrn == DamagePattern::SLASH)
            {
              attackSlash(x2, y2, y1, direction, attcw, current_room, damageam);
            }
            else if (ptrn == DamagePattern::LINE)
            {
              attackLine(x2, y2, y1, direction, attcw, current_room, damageam);
            }
          }
      }
        else
        {
          try
          {
            current_room->checkRange(y2, x2);
          } catch (const CommandExceptions& e)
           {
            throw CommandExceptions(e.what());
           }
          if (ptrn == DamagePattern::SHOT)
          {
            if ((!attacker->checkAmmunition("ARRW") && weapon_type_ == SubWeaponType::BOW)
            && (!attacker->checkAmmunition("BOLT") && weapon_type_ == SubWeaponType::CROSSBOW))
            {
              throw CommandExceptions("E_ATTACK_NO_AMMUNITION");
            }
            damagedeal(x2, y2, attcw, current_room, damageam);
          }
            else if (ptrn == DamagePattern::BURST)
            {
              damagedeal(x2, y2, attcw, current_room, damageam);
              damagedeal(x2+1, y2, attcw, current_room, damageam);
              damagedeal(x2-1, y2, attcw, current_room, damageam);
              damagedeal(x2, y2+1, attcw, current_room, damageam);
              damagedeal(x2, y2-1, attcw, current_room, damageam);
            }
        }
        printAffected(attacker, y2, x2, attcw, current_room);
    }
    return true;
}

void AttackCommand::printAffected(Player* executor, int target_y, int target_x, Item* attcw, Room* room)
{
  if (if_affecting_)
  {
    return;
  }
  std::sort(affected_fields_.begin(), affected_fields_.end(), Functions::compareFields);
  std::cout<<executor->getClassName()<<" ["<<executor->getId()<<"] \""
  <<executor->getName()<<"\" used \""<<attcw->getFullName()
  <<"\" on ("<<target_y+1<<","<<target_x+1<<") affecting ";
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
  int damageam = attcw->damagecalc(executor);
  for (auto& fields : affected_fields_)
  {
    if (fields != nullptr)
    {
      damagedeal(fields->getX(), fields->getY(), attcw, room, damageam);
    }
  }
  affected_fields_.clear();
}

AttackCommand::~AttackCommand() {}
