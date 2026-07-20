#include "Dungeon.hpp"

Dungeon::Dungeon(char* dungeon_config, Player *playerW, Player *playerR, Player *playerB)
{
  playerW_ = playerW;
  playerR_ = playerR;
  playerB_ = playerB;
  number_of_rooms_=0;
  number_of_completed_rooms_=0;

  std::string filename = dungeon_config;
  std::ifstream file{filename};
  std::string line;
  std::getline(file,line);
  while(std::getline(file,line))
  {
    Room *room = new Room(line);
    rooms_.push_back(room);
    number_of_rooms_++;
  }
  current_room_ = rooms_.at(0);
}

void Dungeon::setCompletedRoom(int nmbr)
{
  number_of_completed_rooms_ = nmbr;
}

int Dungeon::getNumberRooms()
{
  return number_of_rooms_;
}

int Dungeon::getCompletedRoom()
{
  return number_of_completed_rooms_;
}

void Dungeon::setPlayers(Player *playerW, Player *playerR, Player *playerB)
{
  playerW_ = playerW;
  playerR_ = playerR;
  playerB_ = playerB;
}

std::vector<Room*> Dungeon::getRooms()
{
  return rooms_;
}

Dungeon::~Dungeon()
{
  for (auto& room : rooms_)
  {
    delete room;
  }
}
