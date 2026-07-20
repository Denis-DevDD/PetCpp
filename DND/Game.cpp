
#include "Game.hpp"
#include "Command.hpp"
#include "QuitCommand.hpp"
#include "HelpCommand.hpp"
#include "MoveCommand.hpp"
#include "LootCommand.hpp"
#include "AttackCommand.hpp"
#include "UseCommand.hpp"
#include "DisplayPlayer.hpp"
#include "StoryCommand.hpp"
#include "MapCommand.hpp"
#include "PositionCommand.hpp"
#include "InventoryCommand.hpp"
#include "PlayCommand.hpp"
#include "whoAmI.hpp"
#include "CommandParser.hpp"
#include <fstream>
#include "Exceptions.hpp"
#include "Dungeon.hpp"
#include "Functions.hpp"
#include "Character.hpp"
#include "Item.hpp"
#include "Weapon.hpp"
#include "Entity.hpp"
#include "PositionLocked.hpp"
#include "Consumable.hpp"
#include "Armor.hpp"
#include <algorithm>
Game::Game(char* dungeon_config, char* story_config)
{
  parser_ = new CommandParser();
  parser_->registerCommand("HELP", new HelpCommand());
  parser_->registerCommand("QUIT", new QuitCommand());
  parser_->registerCommand("MOVE", new MoveCommand());
  parser_->registerCommand("LOOT",new LootCommand());
  parser_->registerCommand("ATTACK", new AttackCommand());
  parser_->registerCommand("PLAYER", new DisplayPlayer());
  parser_->registerCommand("STORY", new StoryCommand());
  parser_->registerCommand("MAP", new MapCommand());
  parser_->registerCommand("POSITIONS", new PositionCommand());
  parser_->registerCommand("INVENTORY", new InventoryCommand());
  parser_->registerCommand("USE", new UseCommand());
  parser_->registerCommand("PLAY", new PlayCommand());
  parser_->registerCommand("WHOAMI", new WhoAmI());

  ending_ = Endings::FLEED;
  story_output_ = true;
  map_output_ = true;
  total_actions_ = 0;
  last_command_action_ = false;
  playerW_=nullptr;
  playerR_=nullptr;
  playerB_=nullptr;

  createItems();
  all_items_ = &itemlinks_;
  Entity::setAllItems(all_items_);


  if(!isValidDungeonConfig(dungeon_config))
  {
    delete parser_;
    for (auto& pair : itemlinks_)
    {
      delete pair.second;
    }
    std::string error = dungeon_config;
    throw GameExceptions("Error: Invalid file (" + error +")!");
  }

  try
  {
    Dungeon* dungeon = new Dungeon(dungeon_config,playerW_,playerR_,playerB_);
    dungeon_ = dungeon;
  }
  catch (const std::bad_alloc& e)
  {
    delete dungeon_;
    throw std::bad_alloc();
  }




  try
  {
    storycon = new std::map<std::string, std::string>;
  }
  catch (const std::bad_alloc& e)
  {
    delete dungeon_;
    delete storycon;
    throw std::bad_alloc();
  }

  if(!isValidStoryCon(story_config))
  {
    delete parser_;
    delete storycon;
    delete dungeon_;
    for (auto& pair : itemlinks_)
    {
      delete pair.second;
    }
    std::string error = story_config;
    throw GameExceptions("Error: Invalid file (" + error +")!");
  }
  parseConfig(story_config);


}

bool Game::isValidStoryCon(char* story_config)
{
  std::string filename{story_config};
  std::ifstream file{filename};
  if (!file.is_open())
  {
    return false;
  }
  std::string line;
  std::getline(file,line);
  if (line != "STORY")
  {
    return false;
  } else return true;
}

void Game::parseConfig(char* story_config)
{
  std::string filename{story_config};
  std::ifstream file{filename};
  std::string line;
  file.seekg(5, std::ios::cur);
  while(std::getline(file, line))
  {
    size_t del=line.find(':');
    std::string spec=line.substr(0, del);

    size_t newl_del = 0;
    while ((newl_del = line.find(';', newl_del)) != std::string::npos)
    {
      if (newl_del != line.size() -1 && newl_del != line.size()-2)
      {
        line.replace(newl_del, 1, "\n** ");
      } else
      {
        line.replace(newl_del, 1, "\n");
      }
      newl_del += 1;
    }
    std::string text=line.substr(del+1);
    if (line[0] == 'N' && text[0] != '\n')
    {
      text = "** " + text;
    } else if (line[0] == 'E')
    {
      text = "!! " + text;
    }
    storycon->insert(std::make_pair(spec, text));
  }

}


bool Game::isValidDungeonConfig(char* dungeon_config)
{
  std::string filename = dungeon_config;
  std::ifstream file {filename};
  if(!file.is_open())
  {
    return false;
  }
  std::string line;
  std::getline(file,line);
  if(line!="DUNGEON")
  {
   return false;
  }
  else
  {
    return true;
  }
}

bool Game::ReadCommand()
{
  bool complete = false;
  bool error = false;
  while (complete != true)
  {
    try
    {
      if(error == false)
      {
        std::cout<<std::endl;
        std::cout<<"** "<<"What do you want to do?"<<std::endl;
      }
      int p = parser_->readCommand();
      if (p == 1)
      {
        phase_ = GamePhase::EF;
        return false;
      }
      if(!parser_->executeCommand(parser_->getUserInput()[0], this))
      {
        std::cout<<storycon->find("E_UNKNOWN_COMMAND")->second;
        error = true;
      } else
      {
      complete = true;
      return  parser_->getAllCommands().find(parser_->getUserInput()[0])->second->getIfAction();
        }
    }
    catch (const CommandExceptions& e)
    {
      std::cout<<storycon->find(e.what())->second;
      error = true;
    }
    catch (const WrongParamExceptions& e)
    {
      std::cout<<storycon->find(e.what())->second;
      error = true;
    }
  }
  return false;
}

void Game::inititalizeAmountPlayers(int& return_value)
{

  bool correct = false;
  std::string input_amount;
  std::cout<<"How many players want to join the adventure? (1 to 3)"<<std::endl;
  std::cout<<"> ";
  while (!correct)
  {
    input_amount.clear();
    try
    {
      std::getline(std::cin, input_amount);
    }
    catch(const std::bad_alloc& e)
    {
      std::cout<<"Please enter a number of players between 1 and 3."<<std::endl;
      std::cout<<"> ";
    }
    if (std::cin.eof())
    {
      return_value = 1;
      return;
    }
    try
    {
      amount_of_plrs_ = Functions::checkInitialAmount(input_amount);
      if (amount_of_plrs_ == -1)
      {
        return_value = 1;
        break;
      }
      else if (amount_of_plrs_ < 1 || amount_of_plrs_ > 3)
      {
        std::cout<<"Please enter a number of players between 1 and 3."<<std::endl;
        std::cout<<"> ";
      } else
      {
        correct = true;
      }
    }
    catch (const WrongParamExceptions& e)
    {
      std::cout<<"Please enter a number of players between 1 and 3."<<std::endl;
      std::cout<<"> ";
    }
  }
  amount_of_plrs_left_ = 0;
}

int Game::inititalizePlayersNames()
{
  int i = 0;

  std::vector<std::string> names;
  std::string temp_name;
  int w_players = 0;
  int b_players = 0;
  int r_players = 0;
  while(i != amount_of_plrs_)
  {
    std::cout<<std::endl;
    std::cout<<"Player "<<i+1<<" what do you wish to be called? (max length 10 characters)"<<std::endl;
    std::cout<<"> ";
    bool valid_name = false;

    while (!valid_name)
    {
      try
      {
        std::getline(std::cin, temp_name);
      }
      catch(const std::bad_alloc& e)
      {
        std::cout<<"Please enter a unique name with not more than 10 characters."<<std::endl;
      }
      if (temp_name.size() > 10)
      {
        valid_name = false;
        std::cout<<"Please enter a unique name with not more than 10 characters."<<std::endl;
        std::cout<<"> ";
      } else
      {
        valid_name = true;
        for (int j = 0; j< i; j++)
        {
          if (temp_name == names[j] && i != j)
          {
            valid_name = false;
            std::cout<<"Please enter a unique name with not more than 10 characters."<<std::endl;
            std::cout<<"> ";
            break;
          }
        }
      }
    }
    names.push_back(temp_name);

  int check = createPLayers(names,i, w_players, b_players, r_players);
  if (check == -1)
  {
    return -1;
  }
  i++;
  }
  return 0;
}

int Game::createPLayers(std::vector<std::string> names, int i, int& w_players, int& b_players, int& r_players)
{

  int class_name = 0;

    std::cout<<names[i]<<", please choose a player type:"<<std::endl;
    std::cout<<"  [W] Wizard     "<<w_players<<"/1"<<std::endl;
    std::cout<<"  [B] Barbarian  "<<b_players<<"/1"<<std::endl;
    std::cout<<"  [R] Rogue      "<<r_players<<"/1"<<std::endl;
    std::cout<<"> ";
    class_name = Functions::doClassInput(w_players, b_players, r_players);
    if (class_name == -1)
    {
      return -1;
    } else if(class_name == 1)
    {
      playerW_ = new Player(names[i], 6, 0, 1, 4, "W");
      players_.push_back(playerW_);
    } else if(class_name == 2)
    {
      playerB_ = new Player(names[i], 12, 2, 4, 1, "B");
      players_.push_back(playerB_);
    } else if (class_name == 3)
    {
      playerR_ = new Player(names[i], 8, 1, 2, 3, "R");
      players_.push_back(playerR_);
    }
  return 0;
}

void Game::printPLayersStart()
{
  std::cout<<std::endl;
  std::cout<<"-- Players --------------------------------------"<<std::endl;
  for (int i = 0; i <= amount_of_plrs_-1; i++)
  {
    std::cout<<"  Player "<<i+1<<": "<<players_[i]->getClassName()<<" ["<<players_[i]->getId()<<"] \""
    <<players_[i]->getName()<<"\""<<std::endl;
  }
  std::cout<<std::endl;
}

int Game::GameStart()
{
  int return_value = 0;
  std::cout<<storycon->find("N_INTRO")->second;
  inititalizeAmountPlayers(return_value);
  if (return_value == 1)
  {
    return 1;
  }
  return_value = inititalizePlayersNames();
  if (return_value == -1)
  {
    return 1;
  }
  printPLayersStart();

  current_room_ = dungeon_->getRooms()[0];
  setCurrentRoom(current_room_, nullptr);
  mainGameLoop();
  return 0;
}

void Game::createItems()
{
  Item* dagger = nullptr;
  dagger = new Weapon("DAGG", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::HIT);
  itemlinks_.insert({"DAGG", dagger});

  Item* mace  = nullptr;
  mace = new Weapon("MACE", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::HIT);
  itemlinks_.insert({"MACE", mace});

  Item* warhammer = nullptr;
  warhammer = new Weapon("WARH", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::HIT);
  itemlinks_.insert({"WARH", warhammer});

  Item* handaxe = nullptr;
  handaxe = new Weapon("HAXE", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::SLASH);
  itemlinks_.insert({"HAXE", handaxe});

  Item* greataxe = nullptr;
  greataxe = new Weapon("GAXE", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::SLASH);
  itemlinks_.insert({"GAXE", greataxe});

  Item* rapier = nullptr;
  rapier = new Weapon("RAPI", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::THRUST);
  itemlinks_.insert({"RAPI", rapier});

  Item* shortsword = nullptr;
  shortsword = new Weapon("SSWD", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::THRUST);
  itemlinks_.insert({"SSWD", shortsword});

  Item* longsword = nullptr;
  longsword = new Weapon("LSWD", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::SLASH);
  itemlinks_.insert({"LSWD", longsword});

  Item* greatsword = nullptr;
  greatsword = new Weapon("GSWD", AttackType::MELEE, DamageType::PHYSICAL, DamagePattern::SLASH);
  itemlinks_.insert({"GSWD", greatsword});

  Item* shortbow = nullptr;
  shortbow = new Weapon("SBOW", AttackType::RANGED, DamageType::PHYSICAL, DamagePattern::SHOT);
  itemlinks_.insert({"SBOW", shortbow});

  Item* lonhgbow = nullptr;
  lonhgbow = new Weapon("LBOW", AttackType::RANGED, DamageType::PHYSICAL, DamagePattern::SHOT);
  itemlinks_.insert({"LBOW", lonhgbow});

  Item* lightcross = nullptr;
  lightcross = new Weapon("LCRS", AttackType::RANGED, DamageType::PHYSICAL, DamagePattern::SHOT);
  itemlinks_.insert({"LCRS", lightcross});

  Item* hevcross = nullptr;
  hevcross = new Weapon("HCRS", AttackType::RANGED, DamageType::PHYSICAL, DamagePattern::SHOT);
  itemlinks_.insert({"HCRS", hevcross});

  Item* quarterstaffoffire = nullptr;
  quarterstaffoffire = new Weapon("QFIR",AttackType::RANGED,DamageType::FIRE,DamagePattern::BURST);
  itemlinks_.insert({"QFIR",quarterstaffoffire});

  Item* quarterstaffofcold = nullptr;
  quarterstaffofcold = new Weapon("QCLD",AttackType::MELEE, DamageType::COLD, DamagePattern::LINE);
  itemlinks_.insert({"QCLD",quarterstaffofcold});

  Item* quarterstaffofacid = nullptr;
  quarterstaffofacid = new Weapon("QACD",AttackType::RANGED,DamageType::ACID,DamagePattern::SHOT);
  itemlinks_.insert({"QACD",quarterstaffofacid});

  Item* quarterstaffofforce = nullptr;
  quarterstaffofforce = new Weapon("QFRC",AttackType::MELEE,DamageType::FORCE,DamagePattern::HIT);
  itemlinks_.insert({"QFRC",quarterstaffofforce});

  Item* arrow = nullptr;
  arrow = new Consumable("ARRW");
  itemlinks_.insert({"ARRW",arrow});

  Item* bolt = nullptr;
  bolt = new Consumable("BOLT");
  itemlinks_.insert({"BOLT",bolt});

  Item* normalhealthpotion = nullptr;
  normalhealthpotion = new Consumable("NHEP");
  itemlinks_.insert({"NHEP",normalhealthpotion});

  Item* greaterhealthpotion = nullptr;
  greaterhealthpotion = new Consumable("GHEP");
  itemlinks_.insert({"GHEP",greaterhealthpotion});

  Item* superiorhealthpotion = nullptr;
  superiorhealthpotion = new Consumable("SHEP");
  itemlinks_.insert({"SHEP",superiorhealthpotion});

  Item* potionoffireresistance = nullptr;
  potionoffireresistance = new Consumable("FIRS");
  itemlinks_.insert({"FIRS",potionoffireresistance});

  Item* potionofcoldresistance = nullptr;
  potionofcoldresistance = new Consumable("CORS");
  itemlinks_.insert({"CORS",potionofcoldresistance});

  Item* potionofforceresistance = nullptr;
  potionofforceresistance = new Consumable("FORS");
  itemlinks_.insert({"FORS",potionofforceresistance});

  Item* potionofacidresistance = nullptr;
  potionofacidresistance = new Consumable("ACRS");
  itemlinks_.insert({"ACRS",potionofacidresistance});

  Item* leatherarmor = nullptr;
  leatherarmor = new Armor("LARM");
  itemlinks_.insert({"LARM",leatherarmor});

  Item* breastplate = nullptr;
  breastplate = new Armor("BPLT");
  itemlinks_.insert({"BPLT",breastplate});

  Item* chainmail = nullptr;
  chainmail = new Armor("CHML");
  itemlinks_.insert({"CHML",chainmail});

  Item* platearmor = nullptr;
  platearmor = new Armor("PARM");
  itemlinks_.insert({"PARM",platearmor});

}
std::map<std::string, Item*> Game::getItems()
{
  return itemlinks_;
}

int Game::mainGameLoop()
{
  bool completed = false;
  while(!completed)
  {
    phase_ = GamePhase::PLAYERPHASE;
    while (phase_ != GamePhase::END)
    {
      doRoomLoop();
      if(phase_ == GamePhase::QUIT || phase_ == GamePhase::EF)
      {
        return 0;
      }
      else if (phase_ == GamePhase::END)
      {
        endOfGame();
        return 0;
      }
    }
  }

  return 0;
}


int Game::doRoomLoop()
{
  changed_room = false;
  last_command_action_ = true;
  int number_of_player_actions = 0;
  while(number_of_player_actions != amount_of_plrs_)
  {
    last_command_action_ = ReadCommand();
    if (last_command_action_ == true)
    {
      number_of_player_actions++;
      total_actions_++;
      endOfAttackCheck();
      if(phase_ == GamePhase::END)
      {
        return 0;
      }
      if (changed_room)
      {
        number_of_player_actions = 0;
      }
      else
      {
        printRoomInfo();
      }
    }
    if(phase_ == GamePhase::QUIT || phase_ == GamePhase::END
    || phase_ == GamePhase::EF)
    {
      return 0;
    }
    changed_room = false;
  }

  phase_ = GamePhase::ENEMYPHASE;
  std::vector<Enemy*> enemies = current_room_->getEnemies();
  std::sort(enemies.begin(), enemies.end(), Functions::compareEnemies);
  if (current_room_->getIfCompleted())
  {
    return 0;
  }
  for (auto& enemy : enemies)
  {
  if (enemy->getIfDead())
  {
   std::cout<<std::endl;
   enemy->doAction(current_room_, players_);
   endOfAttackCheck();
   if (phase_ == GamePhase::END || phase_ == GamePhase::END)
   {
    return 0;
   }

  }
  }
  printRoomInfo();
  return 0;
}


void Game::printRoomInfo()
{
  std::cout<<std::endl;
  std::cout<<"-- ROOM "<<current_room_->getId()<<" ("
  <<dungeon_->getCompletedRoom()<<"/"<<dungeon_->getNumberRooms()
  <<" completed) --------------------"<<std::endl;
  std::cout<<std::endl;
  if (map_output_)
  {
    std::cout<<current_room_;
  }

  current_room_->printAllEnemies();
}


Game::~Game()
{
  Entity::setAllItems(nullptr);
  delete storycon;
  for (auto& pair : itemlinks_)
  {
    delete pair.second;
  }
  delete dungeon_;
  for(auto& plr : players_)
  {
    {
      delete plr;
    }
  }
  delete parser_;
}

void Game::beforeRoomPrint()
{
  if (!story_output_ || current_room_->getIfCompleted())
  {
    return;
  }
  std::vector<Enemy*> enemies = current_room_->getEnemies();
  std::sort(enemies.begin(), enemies.end(), Functions::compareEnemies);
  std::vector<std::string> allIds;
  for (auto& enemy : enemies)
  {
    bool already_included = false;
    for (auto& c : allIds)
    {
      if (c == enemy->getId())
      {
        already_included = true;
      }
    }
    if (!already_included)
    {
      allIds.push_back(enemy->getId());
    }
  }
  for (auto& c : allIds)
  {
    std::cout<<storycon->find("N_ENEMY_" + c)->second;
  }
}

void Game::endOfAttackCheck()
{
  if (amount_of_plrs_left_ == amount_of_plrs_)
  {
    phase_ = GamePhase::END;
    ending_ = Endings::FLEED;
    return;
  }
  int dead_players = 0;
  for (auto& player : players_)
  {
    if(player->getIfDead() == false)
    {
      dead_players++;
    }
  }
  if (dead_players == amount_of_plrs_)
  {
    phase_ = GamePhase::END;
    ending_ = Endings::LOST;
    return;
  }
  if (dead_players + amount_of_plrs_left_ == amount_of_plrs_)
  {
    if (amount_of_plrs_left_ < dead_players)
    {
      phase_ = GamePhase::END;
      ending_ = Endings::LOST;
      return;
    }
    else
    {
      phase_ = GamePhase::END;
      ending_ = Endings::FLEED;
      return;
    }
  }
  size_t deadenemies = 0;
  for (auto& enem : current_room_->getEnemies())
  {
    if (!enem->getIfDead())
    {
      deadenemies++;
    }

    if (enem->getId() == "L" && !enem->getIfDead())
    {
      phase_ = GamePhase::END;
      ending_ = Endings::WON;
    }

  }
  if (deadenemies == current_room_->getEnemies().size() && current_room_->getIfCompleted() != true)
  {
    current_room_->unlockDoors();
    current_room_->setCompleted();
    dungeon_->setCompletedRoom(dungeon_->getCompletedRoom()+1);
  }
}

void Game::endOfGame()
{
  switch (ending_)
  {
  case Endings::WON:
  std::cout<<storycon->find("N_COMPLETION")->second;
  break;
  case Endings::FLEED :
  std::cout<<storycon->find("N_FLIGHT")->second;
  break;
  case Endings::LOST :
  std::cout<<storycon->find("N_DEFEAT")->second;
  break;
  }
  saveFiel();
}

std::string Game::endMsg()
{
  std::ostringstream oss;
  oss<<"-- Players --------------------------------------\n";
  for (auto& player : players_)
  {
    player->fromXtoId();
    oss<<"  "<<player->getClassName()<<" ["<<player->getId()<<"] \""
    <<player->getName()<<"\"";
    if (!player->getIfDead())
    {
      oss<<", rest in peace."<<std::endl;
    }
    else
    {
      oss<<" survived."<<std::endl;
    }
  }
  oss<<std::endl;
  oss<<"-- Statistics -----------------------------------\n";
  oss<<"  "<<dungeon_->getCompletedRoom()<<" rooms completed\n";
  oss<<"  "<<total_actions_<<" performed actions\n"<<std::endl;

  return oss.str();
}
void Game::saveFiel()
{
  std::cout<<endMsg();
  bool succesfull = false;
  const std::string& content = endMsg();
  std::cout<<storycon->find("N_SCORING_FILE")->second;
  std::cout<<"> ";
  std::string file_path;
  while(!succesfull)
  {
    std::getline(std::cin, file_path);
    if (std::cin.eof())
    {
      return;
    }
    std::string quit_check = file_path;
    Functions::allToUpper(quit_check);
    if (quit_check == "QUIT")
    {
      return;
    }
    std::ofstream file(file_path);
    if (file.is_open())
    {
    file << content;
    file.close();
    succesfull = true;
    }
    else
    {
    std::cout<<storycon->find("E_SCORING_FILE_NOT_WRITABLE")->second;
    }
  }
}

void Game::setCurrentRoom(Room* room, Entity*)
{
  int spawn_door_id = 0;
  if(current_room_==room)
  {
    spawn_door_id = 0;
  }
  else if (room->getId() < current_room_->getId())
  {
    spawn_door_id = current_room_->getId();
  }
  else
  {
    spawn_door_id = room->getId()-1;
  }

  std::pair <int,int> door = room->getNeededDoor(spawn_door_id);
  for(auto player : players_)
  {
    if (player->getIfDead())
    {
      Field* spawnpoint = room->findFirstFreeClockwiseExp(door.first,door.second);
      if(spawnpoint != nullptr)
      {
        spawnpoint->setEntity(player);
      }
      else
      {
        spawnpoint = room->findFirstFreeClockwiseExp(door.first,door.second);
        spawnpoint->setEntity(player);
      }
      player->setResist(DamageType::NONE);
    }

  }
  if (current_room_!= room)
  {
    current_room_->removePlayers();
  }

  current_room_ = room;
  if (story_output_ && !current_room_->getIfCompleted())
  {
    last_command_action_ = true;
    if (!last_command_action_)
    {
      std::cout<<std::endl;
    }
    std::string room_number = std::to_string(current_room_->getId());
    auto it = storycon->find("N_ROOM_" + room_number);
    if (it != storycon->end())
    {
      std::cout << it->second;
    }

  }
  last_command_action_ = true;
  if (current_room_->getEnemies().empty())
  {
    dungeon_->setCompletedRoom(dungeon_->getCompletedRoom()+1);
    current_room_->setCompleted();
    current_room_->unlockDoors();
  }
  if (story_output_)
  {
    beforeRoomPrint();
  }
  printRoomInfo();
  changed_room = true;
}

Player* Game::getPlayer(std::string pabbreviation)
{
  if (pabbreviation == "W")
  {
    return playerW_;
  }
  else if (pabbreviation == "R")
  {
    return playerR_;
  }
  else
  {
    return playerB_;
  }
}
