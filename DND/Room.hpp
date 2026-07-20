#ifndef ROOM_HPP
#define ROOM_HPP
#include "Field.hpp"
#include <vector>
#include <string>
#include "Functions.hpp"
#include "Entity.hpp"
#include "Character.hpp"
#include "Enemy.hpp"
#include "PositionLocked.hpp"
#include "Player.hpp"
#include <sstream>

class Room
{
private:
  int room_id_;
  int dimension_X_;
  Field* executor_field_;
  int dimension_Y_;
  static int executor_x_;
  static int executor_y_;
  std::vector<std::vector<Field*>> fields_;
  bool completion_status_;
  int number_of_enemies_;
  std::vector<Enemy*> enemies_;
public:
  friend std::ostream& operator<<(std::ostream& srteam, const Room* room);
  Room(std::string line);

  void printAllEnemies();

  int getId() {return room_id_;}

  std::vector<std::vector<Field*>> getFields()
  {
    return fields_;
  }

  Field* getNeededRoom(std::string id);

  bool getIfCompleted() {return completion_status_;}

  void setCompleted() {completion_status_ = true;}

  std::pair<int,int> getNeededDoor(int leads_to_room);

  bool inBound(int y, int x,int dim_x, int dim_y);

  Field* findFirstFreeClockwise(int y, int x);

  Field* findFirstFreeClockwiseExp(int y, int x);

  void canMoveTO(Field* field, int y, int x);

  void canLoot(Field* field,int y, int x);

  void checkRange(int y, int x);

  int getDimX() const {return dimension_X_;}

  int getDimY() const {return dimension_Y_;}

  void setExecutorY(int y) {executor_y_ = y;}

  void setExecutorX(int x) {executor_x_ = x;}

  std::vector<Enemy*> getEnemies() const {return enemies_;}

  ~Room();

  Field* getNeededEnemy(Enemy* enemy);

  bool inBorders(int y, int x);

  void unlockDoors();

  void removePlayers();

  Field* findEntity(Entity* ent);

  Field* findFirstFreeClockwisePlr(int y, int x);

  Field* findEntById(std::string id, std::vector<std::pair<int, int>>& added_coords);

  Room(const Room&) = delete;
};


#endif
