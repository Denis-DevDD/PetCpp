#ifndef GAME_HPP
#define GAME_HPP
#include <map>
#include <string>
#include "CommandParser.hpp"
#include "Dungeon.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "Item.hpp"
class Command;
class CommandParser;
enum class Endings
{
  LOST, WON, FLEED,
};
enum GamePhase {ENEMYPHASE, PLAYERPHASE, END, QUIT, EF};
class Game
{
private:
  GamePhase phase_;
  bool story_output_;
  bool map_output_;
  std::map<std::string, std::string>* storycon;
  Dungeon *dungeon_;
  Room* current_room_;
  CommandParser* parser_;
  bool last_command_action_;
  Player *playerW_;
  Player *playerR_;
  Player *playerB_;
  int total_actions_;
  bool changed_room;
  int amount_of_plrs_left_;
  int amount_of_plrs_;
  Endings ending_;
  std::vector<Player*> players_;
  std::map<std::string, Item*> itemlinks_;
  std::map<std::string, Item*>* all_items_;

public:

  Game(char* dungeon_config, char* story_config);

  ~Game();

  bool ReadCommand();

  bool isValidStoryCon(char* stroy_config);

  void parseConfig(char* story_config);

  bool isValidDungeonConfig(char* dungeon_config);

  void inititalizeAmountPlayers(int& return_value);

  int inititalizePlayersNames();

  int createPLayers(std::vector<std::string> names, int i, int& w_players, int& b_players, int& r_players);

  void printPLayersStart();

  int GameStart();

  int mainGameLoop();

  int doRoomLoop();

  void printRoomInfo();

  void setPhase(GamePhase phase)
  {
    phase_ = phase;
  }

  void createItems();

  std::map<std::string, Item*> getItems();

  CommandParser* getParser() {return parser_;}

  void endOfAttackCheck();

  std::vector<Player*> getPlayers() {return players_;}

  bool getStoryOutput() {return story_output_;}

  void setStoryOutput(bool b) {story_output_ = b;}

  bool getMapOutput() {return map_output_;}

  void setMapOutput(bool b) {map_output_ = b;}

  Dungeon* getDungeon() {return dungeon_;}

  Room* getCurrentRoom() {return current_room_;}

  int getAmountOfPlayer() {return amount_of_plrs_;}

  void setLeftPlayers(int left_plrs) {amount_of_plrs_left_ = amount_of_plrs_left_+left_plrs;}

  void setCurrentRoom(Room* room, Entity*);

  Player* getPlayer(std::string pabbreviation);

  void beforeRoomPrint();

  std::string getStoryMessage(std::string key)
  {
    return storycon->find(key)->second;
  }

  void endOfGame();

  std::string endMsg();

  void saveFiel();

  Game(const Game&) = delete;
};


#endif
