#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>
#include "Room.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Entity.hpp"
#include "PositionLocked.hpp"
#include <string>
#include <fstream>

class Dungeon
{
private:
  Player *playerW_;
  Player *playerR_;
  Player *playerB_;

  int number_of_rooms_;
  int number_of_completed_rooms_;
  std::vector<Room*> rooms_;
  Room *current_room_;
public:
  Dungeon(char* dungeon_config, Player *playerW, Player *playerR, Player *playerB);

  void setCompletedRoom(int nmbr);
  void setPlayers(Player *playerW, Player *playerR, Player *playerB);
  int getNumberRooms();
  int getCompletedRoom();
  std::vector<Room*> getRooms();


  Dungeon(const Dungeon&) = delete;

  ~Dungeon ();
};

#endif
