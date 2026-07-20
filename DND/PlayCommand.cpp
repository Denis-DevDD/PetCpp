#include "PlayCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
#include "Functions.hpp"

bool PlayCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 1)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  manageActionCount(game);
  getEntityInfo(game);




  command_rating_.clear();
  command_rating_.push_back({"move",0});
  command_rating_.push_back({"attack",0});
  command_rating_.push_back({"loot",0});
  command_rating_.push_back({"use",0});
  chosen_item_ = "NONE";
  chosen_command_ = "NONE";

  calculateMove(game);
  calculateLoot(game);
  calculateAttack(game);
  calculateUse();
  std::sort(command_rating_.begin(), command_rating_.end(),
    [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
    {
    return a.second > b.second;
    });

  chosen_command_ = command_rating_.at(0).first;
  choseTargetsLoot(game);
  choseTargetsUse();
  chooseTargetsMove(game);
  chooseAttack(game);
  chosen_coordinats_.first+=1;
  chosen_coordinats_.second+=1;
  Functions::allToLower(chosen_command_);
  Functions::allToLower(chosen_player_);
  Functions::allToLower(chosen_item_);
  if (chosen_command_ == "use")
  {

    std::cout<<"AI: "<<chosen_command_<<" "<<chosen_player_<<" "<<chosen_item_<<std::endl;
    CommandParser* parser;
    Functions::allToUpper(chosen_command_);
    Functions::allToUpper(chosen_player_);
    Functions::allToUpper(chosen_item_);
    parser = game->getParser();
    parser->setParsersValues(chosen_command_, chosen_player_, chosen_item_, chosen_coordinats_);
    parser->executeCommand(chosen_command_, game);
    return true;
  }
  std::cout<<"AI: "<<chosen_command_<<" "<<chosen_player_<<" "<<chosen_coordinats_.first<<","
  <<chosen_coordinats_.second<<std::endl;
  CommandParser* parser;
  Functions::allToUpper(chosen_command_);
  Functions::allToUpper(chosen_player_);
  Functions::allToUpper(chosen_item_);
  parser = game->getParser();
  parser->setParsersValues(chosen_command_, chosen_player_, "1,1", chosen_coordinats_);
  parser->executeCommand(chosen_command_, game);
  action_count_++;
  return true;
}


void PlayCommand::getEntityInfo(Game* game)
{
  players_.clear();
  enemies_.clear();
  player_coordinats_.clear();
  enemy_coordinats_.clear();
  Room* current_room = game->getCurrentRoom();
  players_ = game->getPlayers();
  players_.erase(std::remove_if(players_.begin(), players_.end(), [](Player* plr)
  {
    return plr->getId() == "X";
  }), players_.end());

  enemies_ = current_room->getEnemies();

  enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](Enemy* enem)
  {
    return enem->getId() == "X";
  }), enemies_.end());

  std::sort(enemies_.begin(), enemies_.end(), Functions::compareEnemies);

  for (auto& plr : players_)
  {
    if (plr->getIfDead())
    {
      int y = current_room->getNeededRoom(plr->getId())->getY();
      int x = current_room->getNeededRoom(plr->getId())->getX();
      player_coordinats_.push_back({y,x});
    }
  }
  for (auto& enemy: enemies_)
  {
    if (enemy->getIfDead())
    {
      int y = current_room->getNeededEnemy(enemy)->getY();
      int x = current_room->getNeededEnemy(enemy)->getX();
      enemy_coordinats_.push_back({y,x});
    }
  }
}

void PlayCommand::manageActionCount(Game* game)
{
  if(action_count_ > 3)
  {
    action_count_ = 1;
  }
  if (game->getCurrentRoom()->getIfCompleted())
  {
    action_count_ = 1;
  }
}

void PlayCommand::calculateLoot(Game* game)
{
  Room* cur_room = game->getCurrentRoom();
  if (!cur_room->getIfCompleted())
  {
    command_rating_.at(2).second-=100;
  }
  std::vector<std::pair<int, int>> coordints;
  while (cur_room->findEntById("X", coordints) != nullptr)
  {
  }
  while (cur_room->findEntById("T", coordints) != nullptr)
  {
  }
  for (auto& plr : players_)
  {
    if (plr->getIfDead())
    {
    int x = cur_room->getNeededRoom(plr->getId())->getX();
    int y = cur_room->getNeededRoom(plr->getId())->getY();
    for (auto& possible_crdnt : coordints)
    {
      if(Functions::getPositionsAdjacent(possible_crdnt.second, possible_crdnt.first,
      x, y))
      {
        command_rating_.at(2).second += 100;
        buffer_coordinats_loot_ = possible_crdnt;
        return;
      }
    }
    }
  }
}

void PlayCommand::choseTargetsLoot(Game* game)
{
  if (chosen_command_ != "loot")
  {
    return;
  }
  chosen_coordinats_ = buffer_coordinats_loot_;
  Room* cur_room = game->getCurrentRoom();
  std::vector<Player*> players_to_loot;
  for (auto& plr : players_)
  {
    if (plr->getIfDead())
    {
      int x = cur_room->getNeededRoom(plr->getId())->getX();
      int y = cur_room->getNeededRoom(plr->getId())->getY();
      if (Functions::getPositionsAdjacent(chosen_coordinats_.second, chosen_coordinats_.first, x, y))
      {
        players_to_loot.push_back(plr);
      }
    }
  }

  if (players_to_loot.at(0)->getId() == "R" && players_to_loot.size() > 1)
  {
    chosen_player_ = players_to_loot.at(1)->getId();
    return;
  }

  chosen_player_ = players_to_loot.at(0)->getId();
}

void PlayCommand::calculateUse()
{
  for (auto& plr : players_)
  {
    auto normal_potion = Entity::getAllItems()->find("NHEP")->second;
    auto big_potion = Entity::getAllItems()->find("GHEP")->second;
    std::map<Item*, int> plr_inv = plr->getLoot();
    auto bigest_potion = Entity::getAllItems()->find("SHEP")->second;


    if (static_cast<float>(plr->getHP()) / plr->getMaxHP() < 0.75
    && (plr_inv.find(normal_potion) != plr_inv.end() ||
    plr_inv.find(big_potion) != plr_inv.end()))
    {
      command_rating_.at(3).second += 90;
    }
    if (static_cast<float>(plr->getHP()) / plr->getMaxHP() < 0.45
    && (plr_inv.find(normal_potion) != plr_inv.end() || plr_inv.find(bigest_potion) != plr_inv.end() ||
    plr_inv.find(big_potion) != plr_inv.end()))
    {
      command_rating_.at(3).second += 999;
    }

    manageItemEffectivness(plr);
    std::sort(weapon_list_.begin(), weapon_list_.end(),
    [](const std::pair<WeaponName, int>& a, const std::pair<WeaponName, int>& b)
    {
      return a.second > b.second;
    });
    std::sort(armor_list_.begin(), armor_list_.end(),
    [](const std::pair<Armortype, int>& a, const std::pair<Armortype, int>& b)
    {
      return a.second > b.second;
    });

    if (plr->getEquippedw() != nullptr && !weapon_list_.empty() &&
    plr->getEquippedw()->getWName() != weapon_list_[0].first)
    {
      command_rating_.at(3).second += 35;
      if (plr->getId() == "W")
      {
        command_rating_.at(3).second += 40;
      }
    }
    else if (plr->getEquippeda() != nullptr && !armor_list_.empty() &&
    plr->getEquippeda()->getArmName() != armor_list_[0].first)
    {
      command_rating_.at(3).second += 45;
    }
    managePotions(plr);
  }
  if (enemies_.empty())
  {
    command_rating_.at(3).second += 20;
  }
  else
  {
    command_rating_.at(3).second -= 10;
  }
}



void PlayCommand::manageItemEffectivness(Player* plr)
{
  weapon_list_.clear();
  armor_list_.clear();
  for (auto& weapon : plr->getLoot())
  {
  manageWeaponEffectivness(weapon.first, plr);
  manageArmorEffectivness(weapon.first, plr);
  }

}

void PlayCommand::manageArmorEffectivness(Item* armor, Player* plr)
{
  if (armor->getItemType() != ItemType::ARMOR)
  {
    return;
  }
  Armortype armtype = armor->getArmName();
  int result = 0;
  switch (armtype)
  {
   case Armortype::LARM:
    result =1 + plr->getVital();
     break;
    case Armortype::BPLT:
    result =4 + std::min(plr->getVital(),2);
     break;
    case Armortype::CHML:
     result =6;
      break;
    case Armortype::PARM:
     result =8;
      break;
    default :
      result = 0;
       break;
  }
  armor_list_.push_back({armtype, result});
}

void PlayCommand::manageWeaponEffectivness(Item* weapon, Player* plr)
{
  if  (weapon->getItemType() != ItemType::WEAPON)
  {
    return;
  }
  int result = 0;
  switch (weapon->getWName())
  {
    case WeaponName::DAGG:
      result = result=4 + plr->getVital();
    break;
    case WeaponName::MACE:
      result = result= 6 + plr->getStr();
    break;
    case WeaponName::WARH:
      result = result= 10 + plr->getStr();
    break;
    case WeaponName::HAXE:
      result = result= 6 + plr->getStr();
    break;
    case WeaponName::GAXE:
      result = result= 12 + plr->getStr();
    break;
    case WeaponName::RAPI:
      result = result= 8 + plr->getVital();
    break;
    case WeaponName::SSWD:
      result = result= 6 + plr->getVital();
    break;
    case WeaponName::LSWD:
      result = result= 10 + plr->getVital();
    break;
    case WeaponName::GSWD:
      result = result= 6 + 6 + 1 + plr->getStr();
    break;
    case WeaponName::SBOW:
      result = result= 6 + plr->getVital();
    break;
    case WeaponName::LBOW:
      result = result= 8 + plr->getVital();
    break;
    case WeaponName::LCRS:
      result = result= 8 + plr->getVital();
    break;
    case WeaponName::HCRS:
      result = result= 8 + plr->getVital();
    break;
    default:
    break;
    }
  if (plr->getId()=="W")
  {
    switch (weapon->getWName())
    {
      case WeaponName::QFIR :
        result = 6 + 6 + 6;
      break;
      case WeaponName::QCLD :
        result = 10 + 4;
      break;
      case WeaponName::QACD :
        result = 10 + 5;
      break;
      case WeaponName::QFRC :
        result = 10;
      break;
      default:
      break;
    }
  }
  bool wiz_alive = false;
  for (auto& plur : players_)
  {
    if (plur->getId() == "W")
    {
      wiz_alive = true;
    }
  }
  if (!wiz_alive && plr->getId() == "R" && chosen_command_ != "attack")
  {
    switch (weapon->getWName())
    {
     case WeaponName::SBOW:
        result = result= 6 + plr->getVital() + 10;
      break;
      case WeaponName::LBOW:
        result = result= 8 + plr->getVital() + 10;
      break;
      case WeaponName::LCRS:
        result = result= 8 + plr->getVital() + 10;
      break;
      case WeaponName::HCRS:
        result = result= 8 + plr->getVital() + 10;
      break;
      default:
      break;
    }
  }
  weapon_list_.push_back({weapon->getWName(), result});
}

void PlayCommand::choseTargetsUse()
{
  if (chosen_command_ != "use")
  {
    return;
  }
  for (auto& plr : players_)
  {

    manageItemEffectivness(plr);
    std::sort(weapon_list_.begin(), weapon_list_.end(),
    [](const std::pair<WeaponName, int>& a, const std::pair<WeaponName, int>& b)
    {
      return a.second > b.second;
    });
        std::sort(armor_list_.begin(), armor_list_.end(),
    [](const std::pair<Armortype, int>& a, const std::pair<Armortype, int>& b)
    {
      return a.second > b.second;
    });

    auto normal_potion = Entity::getAllItems()->find("NHEP")->second;
    auto big_potion = Entity::getAllItems()->find("GHEP")->second;
    std::map<Item*, int> plr_inv = plr->getLoot();
    auto bigest_potion = Entity::getAllItems()->find("SHEP")->second;


    if (static_cast<float>(plr->getHP()) / plr->getMaxHP() < 0.45
    && (plr_inv.find(normal_potion) != plr_inv.end() || plr_inv.find(bigest_potion) != plr_inv.end() ||
    plr_inv.find(big_potion) != plr_inv.end()))
    {
      chosen_player_ = plr->getId();
      if (plr_inv.find(bigest_potion) != plr_inv.end())
      {
        chosen_item_ = plr_inv.find(bigest_potion)->first->getName();
        return;
      }
      else if (plr_inv.find(big_potion) != plr_inv.end())
      {
        chosen_item_ = plr_inv.find(big_potion)->first->getName();
        return;
      }
        else if (plr_inv.find(normal_potion) != plr_inv.end())
      {
        chosen_item_ = plr_inv.find(normal_potion)->first->getName();
        return;
      }
    }
    else if (static_cast<float>(plr->getHP()) / plr->getMaxHP() < 0.75
    && (plr_inv.find(normal_potion) != plr_inv.end() ||
    plr_inv.find(big_potion) != plr_inv.end()))
    {
      chosen_player_ = plr->getId();
      if (plr_inv.find(big_potion) != plr_inv.end())
      {
        chosen_item_ = plr_inv.find(big_potion)->first->getName();
        return;
      }
        else if (plr_inv.find(normal_potion) != plr_inv.end())
      {
        chosen_item_ = plr_inv.find(normal_potion)->first->getName();
        return;
      }
    }
    if (checkEquippedstuff(plr) == 1)
    {
      return;
    }
  }

}

bool PlayCommand::managePotions(Player* plr)
{
  for (auto& enem : enemies_)
  {
    for (auto& item : enem->getInventory())
    {
      if (item.first->getItemType() == ItemType::WEAPON)
      {
        for (auto& plr_inv_item : plr->getLoot())
        {
          if (plr_inv_item.first->getItemType() == ItemType::CONSUMABLE
          && plr_inv_item.first->getDmgREsistanmces() == item.first->getDType())
          {
            command_rating_.at(3).second += 25;
          }
        }
      }
    }
  }
  return true;
}


int PlayCommand::manageFieldValue(Game* game, int y, int x)
{
  int y_dim = game->getCurrentRoom()->getDimY();
  int x_dim = game->getCurrentRoom()->getDimX();
  if (y >= y_dim || y < 0 || x >= x_dim || x < 0)
  {
    return 0;
  }
  return getFieldval(game->getCurrentRoom()->getFields()[y][x]);
}

int PlayCommand::getFieldval(Field* af)
{
  int result=0;
  if (!af->getEntity())
  {
    return 0;
  }
  if (af->getEntity()->getId() == "Z" || af->getEntity()->getId() == "G" || af->getEntity()->getId() == "L")
  {
    result+=15;
    if (af->getEntity()->getId() == "G" || af->getEntity()->getId() == "L")
    {
      result+=10;
    }
  }
  if (af->getEntity()->getId()=="W" || af->getEntity()->getId()=="B" || af->getEntity()->getId()=="R")
  {
    result-=40;
  }
  return result;
}

std::vector<int> PlayCommand::checkforslash(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom = game->getCurrentRoom()->getFields();
  int xattack1 = 0, yattack1 = 0;
  int xattack2 = 0, yattack2 = 0;
  int bestoption1 = 0, bestoption2 = 0;

  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1; j <= 1; ++j)
    {
      if (i == 0 && j == 0)
      {
        continue;
      }
      xattack1 = xpos + i;
      yattack1 = ypos + j;
      bestoption1 = manageFieldValue(game, ypos + j, xpos + i);

      if (i == -1)
      {
        if (j == -1)
        {
          bestoption1 += manageFieldValue(game, ypos, xpos + i);
          bestoption1 += manageFieldValue(game, ypos + j, xpos);
        }
        else if (j == 0)
        {
          bestoption1 += manageFieldValue(game, ypos + 1, xpos + i);
          bestoption1 += manageFieldValue(game, ypos - 1, xpos + i);
        }
        else
        {
          bestoption1 += manageFieldValue(game, ypos + j, xpos);
          bestoption1 += manageFieldValue(game, ypos, xpos - j);
        }
      }
      else if (i == 0)
      {
        if (j == -1)
        {
          bestoption1 += manageFieldValue(game, ypos - 1, xpos - 1);
          bestoption1 += manageFieldValue(game, ypos - 1, xpos + 1);
        }
        else
        {
          bestoption1 += manageFieldValue(game, ypos + 1, xpos + 1);
          bestoption1 += manageFieldValue(game, ypos - 1, xpos - 1);
        }
      }
      else
      {
        if (j == -1)
        {
          bestoption1 += manageFieldValue(game, ypos - 1, xpos);
          bestoption1 += manageFieldValue(game, ypos, xpos + 1);
        }
        else if (j == 0)
        {
          bestoption1 += manageFieldValue(game, ypos + 1, xpos + i);
          bestoption1 += manageFieldValue(game, ypos - 1, xpos + i);
        }
        else
        {
          bestoption1 += manageFieldValue(game, ypos + 1, xpos);
          bestoption1 += manageFieldValue(game, ypos, xpos + 1);
        }
      }

      if (bestoption1 > bestoption2)
      {
        bestoption2 = bestoption1;
        xattack2 = xattack1;
        yattack2 = yattack1;
      }
    }
  }
  if (Functions::getPositionsAdjacent(xpos, ypos, xattack2, yattack2))
  {
    bestoption2+=5;
  }
  std::vector<int> result{bestoption2, xattack2, yattack2};
  return result;
}

std::vector<int> PlayCommand::checkforhit(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom = game->getCurrentRoom()->getFields();
  int xattack1 = 0, yattack1 = 0;
  int xattack2 = 0, yattack2 = 0;
  int bestoption1 = 0, bestoption2 = 0;

  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1; j <= 1; ++j)
    {
      if (xpos + i >= game->getCurrentRoom()->getDimX() || xpos + i < 0 || ypos + j >= game->getCurrentRoom()->getDimY() || ypos + j < 0)
      {
        continue;
      }

      bestoption1 = getFieldval(checkroom[ypos + j][xpos + i]);
      xattack1 = xpos + i;
      yattack1 = ypos + j;

      if (bestoption1 > bestoption2)
      {
        bestoption2 = bestoption1;
        xattack2 = xattack1;
        yattack2 = yattack1;
        xattack1 = 0;
        yattack1 = 0;
        bestoption1 = 0;
      }
    }
  }

  if (Functions::getPositionsAdjacent(xpos, ypos, xattack2, yattack2))
  {
    bestoption2+=5;
  }
  std::vector<int> result{bestoption2, xattack2, yattack2};
  return result;
}

std::vector<int> PlayCommand::checkforshot(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom=game->getCurrentRoom()->getFields();
  int xattack1 = 0, yattack1 = 0;
  int xattack2 = 0, yattack2 = 0;
  int bestoption1 = 0, bestoption2 = 0;
  int xdim=game->getCurrentRoom()->getDimX();
  int ydim=game->getCurrentRoom()->getDimY();
  for (int i=0; i<xdim; ++i)
  {
    for (int j=0; j<ydim; ++j)
    {
      bestoption1=getFieldval(checkroom[j][i]);
      if (bestoption1>bestoption2)
      {
        bestoption2=bestoption1;
        xattack2=i;
        yattack2=j;
        xattack1=0;
        yattack1=0;
        bestoption1=0;
      }
    }
  }

  if (Functions::getPositionsAdjacent(xpos, ypos, xattack2, yattack2))
  {
    bestoption2+=5;
  }
  std::vector<int> result{bestoption2, xattack2, yattack2};
  return result;
}
std::vector<int> PlayCommand::checkforthrust(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom = game->getCurrentRoom()->getFields();
  int xattack1 = 0, yattack1 = 0;
  int xattack2 = 0, yattack2 = 0;
  int bestoption1 = 0, bestoption2 = 0;

  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1; j <= 1; ++j)
    {
      if (i == 0 && j == 0)
      {
        continue;
      }

      xattack1 = xpos + i;
      yattack1 = ypos + j;

      if (xattack1 < 0 || xattack1 >= game->getCurrentRoom()->getDimX() || yattack1 < 0 || yattack1 >= game->getCurrentRoom()->getDimY())
      {
        continue;
      }

      bestoption1 = manageFieldValue(game, yattack1, xattack1);

      int newX = xpos + 2 * i;
      int newY = ypos + 2 * j;

      if (newX >= 0 && newX < game->getCurrentRoom()->getDimX() && newY >= 0 && newY < game->getCurrentRoom()->getDimY())
      {
        bestoption1 += manageFieldValue(game, newY, newX);
      }

      if (bestoption1 > bestoption2)
      {
        bestoption2 = bestoption1;
        xattack2 = xattack1;
        yattack2 = yattack1;
      }
    }
  }

  if (Functions::getPositionsAdjacent(xpos, ypos, xattack2, yattack2))
  {
    bestoption2 += 5;
  }

  std::vector<int> result{bestoption2, xattack2, yattack2};
  return result;
}

std::vector<int> PlayCommand::checkforline(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom = game->getCurrentRoom()->getFields();
  int xdim = game->getCurrentRoom()->getDimX();
  int ydim = game->getCurrentRoom()->getDimY();
  int bestoption = 0;
  int xattack = xpos;
  int yattack = ypos;
  auto checkLine = [&](int dx, int dy)
  {
    int value = 0;
    int x = xpos + dx;
    int y = ypos + dy;

    while (x >= 0 && x < xdim && y >= 0 && y < ydim)
    {
      value += getFieldval(checkroom[y][x]);
      x += dx;
      y += dy;
    }
    return value;
  };
  std::vector<std::pair<int, int>> directions = {
  {-1, -1}, {-1, 0}, {-1, 1},
  {0, -1},          {0, 1},
  {1, -1}, {1, 0}, {1, 1}
  };

  for (const auto& dir : directions)
  {
    int value = checkLine(dir.first, dir.second);
    if (value > bestoption)
    {
      bestoption = value;
      xattack = xpos + dir.first;
      yattack = ypos + dir.second;
    }
  }

  std::vector<int> result{bestoption, xattack, yattack};
  return result;
}

std::vector<int> PlayCommand::checkforburst(Game* game, int xpos, int ypos)
{
  std::vector<std::vector<Field*>> checkroom=game->getCurrentRoom()->getFields();
  int xattack1 = 0, yattack1 = 0;
  int xattack2 = 0, yattack2 = 0;
  int bestoption1 = 0, bestoption2 = 0;
  int xdim = game->getCurrentRoom()->getDimX();
  int ydim = game->getCurrentRoom()->getDimY();
  for (int i=1; i<xdim; ++i)
  {
    for (int j=1; j<ydim; ++j)
    {
      bestoption1 = 0;
      if (xpos+i>=xdim || xpos+i<0 || ypos+j>=ydim || ypos+i<0)
      {
        break;
      }
        else
        {
          xattack1=i;
          yattack1=j;
          for (int k=-1; k<2; ++k)
          {
            for (int g=-1; g<2; ++g)
            {
             if(i+k>=xdim || i+k<0 || j+g>=ydim || j+g<0)
             {
               break;
             }
               else
               {
                 bestoption1+=manageFieldValue(game, i+k, j+g);
               }
            }
          }
          if (bestoption1>bestoption2)
          {
            bestoption2=bestoption1;
            xattack2=xattack1;
            yattack2=yattack1;
            xattack1=0;
            yattack1=0;
            bestoption1=0;
          }
        }
    }
  }
  if (Functions::getPositionsAdjacent(xpos, ypos, xattack2, yattack2))
  {
    bestoption2+=5;
  }
  std::vector<int> result{bestoption2, xattack2, yattack2};
  return result;
}

void PlayCommand::calculateAttack(Game* game)
{
  if (enemies_.empty())
  {
    command_rating_.at(1).second=-99999;
    return;
  }
  int sumrating = 0;
  int final = 0;
  for (auto& player : players_)
  {
    if (player->getId() == "X" || !player->getIfDead())
    {
      continue;
    }
      Item* equippedw = player->getEquippedw();
      if (equippedw)
      {
        Field* plr_field = game->getCurrentRoom()->getNeededRoom(player->getId());
        switch (equippedw->getPattern())
        {
          case DamagePattern::HIT:
            sumrating = checkforhit(game, plr_field->getX(), plr_field->getY())[0];
            break;
          case DamagePattern::SLASH:
            sumrating = checkforslash(game, plr_field->getX(), plr_field->getY())[0];
            break;
          case DamagePattern::SHOT:
            sumrating = checkforshot(game, plr_field->getX(), plr_field->getY())[0];
            break;
          case DamagePattern::THRUST:
             sumrating = checkforthrust(game, plr_field->getX(), plr_field->getY())[0];
             break;
          case DamagePattern::LINE:
            sumrating = checkforline(game, plr_field->getX(), plr_field->getY())[0];
            break;
          case DamagePattern::BURST:
            sumrating = checkforburst(game, plr_field->getX(), plr_field->getY())[0];
          break;
        }
      }
    if (sumrating > final)
    {
      final = sumrating;
    }
  }
  if (final == 0)
  {
    command_rating_.at(1).second-=10;
    command_rating_.at(0).second+=50;
  }
  command_rating_.at(1).second+=final;

  for(size_t i=0; i<enemies_.size();i++)
  {
    if (enemies_[i]->getId() == "L")
    {
      for(size_t b=0; b < players_.size();b++)
      {
        if (players_[b]->getEquippedw()->getaType() == AttackType::MELEE)
        {
          int l_y = enemy_coordinats_.at(i).first;
          int l_x = enemy_coordinats_.at(i).second;
          int p_y = player_coordinats_.at(b).first;
          int p_x = player_coordinats_.at(b).second;
          if (Functions::getPositionsAdjacent(l_x, l_y, p_x, p_y))
          {
              command_rating_.at(1).second+=9999;
          }
        }
      }
    }

  }
}

void PlayCommand::chooseAttack(Game* game)
{
  if (chosen_command_ != "attack")
  {
    return;
  }
  int final = 0;
  Player* best = nullptr;
  std::vector<int> betweencalc;
  std::pair<int, int> finalcoordinates;

  for (const auto& player : players_)
  {
    if (player->getId() == "X" || !player->getIfDead())
    {
      continue;
    }
    Field* plr_field = game->getCurrentRoom()->getNeededRoom(player->getId());
    Item* equippedw = player->getEquippedw();
    weapon_list_.clear();
    manageWeaponEffectivness(equippedw, player);
    switch (equippedw->getPattern())
    {
      case DamagePattern::HIT:
      {
        betweencalc = checkforhit(game, plr_field->getX(), plr_field->getY());
        if (betweencalc[0]> 0)
        betweencalc[0] += weapon_list_[0].second;
        break;
      }
      case DamagePattern::SLASH:
      {
        betweencalc = checkforslash(game, plr_field->getX(), plr_field->getY());
        if (betweencalc[0]> 0)
        betweencalc[0] += weapon_list_[0].second;
        break;
      }
      case DamagePattern::SHOT:
      {
        betweencalc = checkforshot(game, plr_field->getX(), plr_field->getY());
        if (betweencalc[0]> 0)
        betweencalc[0] += weapon_list_[0].second;
        break;
      }
      case DamagePattern::THRUST:
      {
        betweencalc = checkforthrust(game, plr_field->getX(), plr_field->getY());
        if (betweencalc[0]> 0)
        betweencalc[0] += weapon_list_[0].second;
        break;
      }
      case DamagePattern::LINE:
      {
        betweencalc = checkforline(game, plr_field->getX(), plr_field->getY());
        if (betweencalc[0]> 0)
        betweencalc[0] += weapon_list_[0].second;
        break;
      }
      case DamagePattern::BURST:
      {
        betweencalc[0] += weapon_list_[0].second;
        if (betweencalc[0]> 0)
        betweencalc = checkforburst(game, plr_field->getX(), plr_field->getY());
        break;
      }
    }

    if (betweencalc[0] > final)
    {
      final = betweencalc[0];
      best = player;
      if (betweencalc[2] < 0)
      {
        betweencalc[2]+=1;
      }
      if (betweencalc[1] < 0)
      {
        betweencalc[1]+=1;
      }
      finalcoordinates = {betweencalc[2], betweencalc[1]};
    }
  }
  chosen_coordinats_ = finalcoordinates;
  chosen_player_ = best->getId();
}

int PlayCommand::checkEquippedstuff(Player* plr)
{
  if (plr->getEquippedw() != nullptr && !weapon_list_.empty()
  && plr->getEquippedw()->getWName() != weapon_list_[0].first && plr->getId() == "W")
  {
    chosen_player_ = "W";
    chosen_item_ = Weapon::wNameToName(weapon_list_[0].first);
    return 1;
  }
  if (plr->getEquippedw() == nullptr && !weapon_list_.empty())
  {
    chosen_player_ = plr->getId();
    chosen_item_ = Weapon::wNameToName(weapon_list_[0].first);
    return 0;
  }
  if (plr->getEquippeda() == nullptr && !armor_list_.empty())
  {
    chosen_player_ = plr->getId();
    chosen_item_ = Armor::aNameToName(armor_list_[0].first);
    return 0;
  }
  if (plr->getEquippedw() != nullptr && !weapon_list_.empty()
   && plr->getEquippedw()->getWName() != weapon_list_[0].first)
  {
    chosen_player_ = plr->getId();
    chosen_item_ = Weapon::wNameToName(weapon_list_[0].first);
    return 0;
  }
  else if (plr->getEquippeda() != nullptr && !armor_list_.empty()
  && plr->getEquippeda()->getArmName() != armor_list_[0].first)
  {
    chosen_player_ = plr->getId();
    chosen_item_ = Armor::aNameToName(armor_list_[0].first);
    return 0;
  }
  checkPotion(plr);
  return 0;
}

void PlayCommand::checkPotion(Player* plr)
{
  for (auto& enem : enemies_)
  {
    for (auto& item : enem->getInventory())
    {
      if (item.first->getItemType() == ItemType::WEAPON)
      {
        for (auto& plr_inv_item : plr->getLoot())
        {
          if (plr_inv_item.first->getItemType() == ItemType::CONSUMABLE
          && plr_inv_item.first->getDmgREsistanmces() == item.first->getDType())
          {
            chosen_player_ = plr->getId();
            chosen_item_ = plr_inv_item.first->getName();
            return;
          }
        }
      }
    }
  }
}

bool freeFieldsAround(Game *game, int row, int col)
{
  std::vector<std::pair<int,int>> directions ={
      {0,0},
      {-1,0},
      {-1,1},
      {0,-1},
      {0,1},
      {1,-1},
      {1,0},
      {1,1},
      {-1,-1}
  };

  for(auto& direction : directions)
  {
    std::pair<int,int> new_position = {row+direction.first, col+direction.second};
    if(new_position.first >= 0 && new_position.first < game->getCurrentRoom()->getDimY())
    {
      if(new_position.second >= 0 && new_position.second < game->getCurrentRoom()->getDimX())
      {
        if(game->getCurrentRoom()->getFields()[new_position.first][new_position.second]->getEntity()==nullptr)
        {
          return true;
        }
      }
    }
  }
  return false;
}

void PlayCommand::calculateMove(Game *game)
{
  int& rating = command_rating_.at(0).second;
  if(game->getCurrentRoom()->getIfCompleted())
  {
    rating+=30;
  }
  else
  {
    if(action_count_==1)
    {
      for(auto& enem : enemies_)
      {
        if (enem->getId() == "G")
        {
          rating+=15;
        }
        else if (enem->getId() == "L")
        {
          rating+=30;
        }
      }
    }
    else
    {
      rating-=45;
    }
  }
  if (game->getCurrentRoom()->getIfCompleted())
  {
    return;
  }
  for(Enemy* enemy : enemies_)
  {
    if(enemy->getId()=="L")
    {
      rating+=70;
    }
    else
    {
      rating-=40;
    }
  }

  for(size_t i=0; i<players_.size();i++)
  {
    Player* player = players_.at(i);
    if(!freeFieldsAround(game, player_coordinats_.at(i).first,player_coordinats_.at(i).second) && player->getId() != "W")
    {
      rating -= 150/players_.size();
    }
    if(player->getEquippedw()->getaType()==AttackType::MELEE)
    {
      for(size_t b=0;b<enemies_.size();b++)
      {
        if( (player_coordinats_.at(i).first < enemy_coordinats_.at(b).first-1)
            || (player_coordinats_.at(i).second < enemy_coordinats_.at(b).second-1) )
        {
          rating+=20;
          break;
        }
      }
    }
  }
  for(size_t i=0; i<players_.size();i++)
  {
    for(size_t b=0;b<enemies_.size();b++)
    {
      int enem_y = enemy_coordinats_.at(b).first;
      int enem_x = enemy_coordinats_.at(b).second;
      int plr_y = player_coordinats_.at(i).first;
      int plr_x = player_coordinats_.at(i).second;
      if (Functions::getPositionsAdjacent(enem_x, enem_y, plr_x, plr_y))
      {
        rating-=40;
        break;
      }
    }

  }
}

void PlayCommand::moveDirectionEntity(Game *game, std::pair<int, int> player, std::pair<int, int> entity)
{
  std::pair<int,int> chosen_coordinats;
  std::vector<std::pair<int,int>> directions ={
      {0,0},
      {-1,0},
      {-1,1},
      {0,-1},
      {0,1},
      {1,-1},
      {1,0},
      {1,1},
      {-1,-1}
  };
  int min_distance = abs(player.first-entity.first)+abs(player.second-entity.second);
  for(auto& direction : directions)
  {
    std::pair<int,int> new_position = {player.first+direction.first, player.second+direction.second};
    if(new_position.first >= 0 && new_position.first < game->getCurrentRoom()->getDimY())
    {
      if(new_position.second >= 0 && new_position.second < game->getCurrentRoom()->getDimX())
      {
        if(game->getCurrentRoom()->getFields()[new_position.first][new_position.second]->getEntity()!=nullptr)
        {
          if(game->getCurrentRoom()->getFields()[new_position.first][new_position.second]->getEntity()->getId()!="D"||
          (game->getCurrentRoom()->getFields()[new_position.first][new_position.second]->getEntity()->getId()=="D" &&
          game->getCurrentRoom()->getFields()[new_position.first][new_position.second]->getEntity()->getIfLocked()))
          {
            continue;
          }

        }
      }
    }
    int distance = abs(new_position.first - entity.first)+abs(new_position.second - entity.second);
    if(distance <= min_distance)
    {
      min_distance = distance;
      chosen_coordinats = new_position;
    }
  }
  chosen_coordinats_ = chosen_coordinats;
}

void PlayCommand::chooseTargetMoveEnemiesLeft(Game *game)
{
  int tank = 0;
  int max = 0;
  for(size_t i=0; i<players_.size();i++)
  {
    bool melee = false;
    Player* player = players_.at(i);
    if (player->getEquippedw() != nullptr)
    {
      if(player->getEquippedw()->getaType()==AttackType::MELEE)
      {
        melee = true;
      }
    }
    int value = player->getHP();
    if (player->getEquippeda() != nullptr)
    {
      value = ( player->getEquippeda()->getArmValue() + player->getHP() )*2;
    }
    if(value>max && melee)
    {
      max=value;
      tank=i;
    }
  }
  chosen_player_ = players_.at(tank)->getId();
  std::pair<int,int> tank_crd = player_coordinats_.at(tank);
  std::string preferred_enemy = "Z";
  int min_distance = 2147483647;
  int closest_enemy = 0;
  for(size_t i=0; i < enemies_.size();i++)
  {
    std::pair<int,int> enemy_crd = enemy_coordinats_.at(i);
    int distance = pow((tank_crd.first - enemy_crd.first),2) + pow((tank_crd.second - enemy_crd.second),2);
    if(enemies_.at(i)->getId()=="L")
    {
      if(preferred_enemy == "Z")
      {
        preferred_enemy = "L";
      }
      if(distance < min_distance)
      {
        min_distance = distance;
        closest_enemy = i;
      }
    }
    else if(enemies_.at(i)->getId()=="G")
    {
      if(preferred_enemy == "Z")
      {
        preferred_enemy = "G";
      }
      if(distance < min_distance)
      {
        min_distance = distance;
        closest_enemy = i;
      }
    }
    else
    {
      if(distance < min_distance)
      {
        min_distance = distance;
        closest_enemy = i;
      }
    }
  }

  if (!enemies_.empty())
  {
    moveDirectionEntity(game,player_coordinats_.at(tank),enemy_coordinats_.at(closest_enemy));
  }
}

std::vector<std::pair<Entity*,std::pair<int,int>>> PlayCommand::posLockEntInRoom(Game* game)
{
  std::vector<std::pair<Entity*,std::pair<int,int>>> list;
  Room *room = game->getCurrentRoom();
  std::vector<std::vector<Field*>> fields = room->getFields();
  for(auto& row : fields)
  {
    for(auto& field : row)
    {
      if(field->getEntity()!=nullptr)
      {
        if((field->getEntity()->getId()=="X" && field->getEntity()->getIfplayer()) || (field->getEntity()->getId()=="D" && field->getEntity()->getNumber() > game->getCurrentRoom()->getId()) || field->getEntity()->getId()=="T")
        {
          list.push_back({field->getEntity(),{field->getY(),field->getX()}});
        }
      }
    }
  }
  return list;
}

void PlayCommand::chooseTargetMoveNoEnemies(Game* game)
{
  std::vector<std::pair<Entity*,std::pair<int,int>>> list = posLockEntInRoom(game);

  std::string preferred = "D";
  int preferred_index = -1;
  for(size_t i=0; i<list.size();i++)
  {
    if(list.at(i).first->getId()=="X")
    {
      if(preferred == "D")
      {
        preferred = "X";
      }
      preferred_index = i;
    }
    else if(list.at(i).first->getId()=="T")
    {
      if(preferred == "D")
      {
        preferred = "T";
      }
      preferred_index = i;
    }
    else if(preferred == "D")
    {
      preferred_index = i;
    }
  }

  int min_distance = 2147483647;

  int chosen_player_index = 0;
  for(size_t i = 0; i<players_.size();i++)
  {
    if (players_.at(i)->getId()=="R")
    {
      chosen_player_index = i;
    }
  }
  std::pair<int,int> entity_crd = list.at(preferred_index).second;
  for(size_t i = 0; i<players_.size();i++)
  {
    std::pair<int,int> player_crd = player_coordinats_.at(i);
    int distance = pow((player_crd.first - entity_crd.first),2) + pow((player_crd.second - entity_crd.second),2);
    if(distance < min_distance && players_.at(i)->getId()!="R")
    {
      min_distance = distance;
      chosen_player_index = i;
    }
  }
  chosen_player_ = players_.at(chosen_player_index)->getId();
  moveDirectionEntity(game,player_coordinats_.at(chosen_player_index),entity_crd);
}

void PlayCommand::chooseTargetsMove(Game* game)
{
  if(chosen_command_ != "move")
  {
    return;
  }
  if(!enemies_.empty())
  {
    chooseTargetMoveEnemiesLeft(game);
  }
  else
  {
    chooseTargetMoveNoEnemies(game);
  }
}