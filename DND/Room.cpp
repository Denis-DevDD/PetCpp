



#include "Room.hpp"
#include "Exceptions.hpp"
#include <algorithm>
#include <cmath>

Room::Room(std::string line)
{
  completion_status_ = false;
  number_of_enemies_ = 0;

  size_t line_index = 0;

  std::string r_id;
  while(line.at(line_index)!=':')
  {
    r_id.push_back(line.at(line_index));
    line_index++;
  }
  room_id_=Functions::stringToInteger(r_id);

  line_index++;
  std::string dim_x;
  while(line.at(line_index)!=',')
  {
    dim_x.push_back(line.at(line_index));
    line_index++;
  }
  dimension_X_=Functions::stringToInteger(dim_x);

  line_index++;
  std::string dim_y;
  while(line.at(line_index)!=';')
  {
    dim_y.push_back(line.at(line_index));
    line_index++;
  }
  dimension_Y_=Functions::stringToInteger(dim_y);



  std::vector<Field*> x_fields;
  for(int a = 0; a<dimension_Y_;a++)
  {
    x_fields.clear();
    for(int b = 0; b<dimension_X_;b++)
    {
      Field* field = new Field(b,a);
      x_fields.push_back(field);
    }
    fields_.push_back(x_fields);
  }


  line_index++;
  std::string entities_list = line.substr(line_index);

  size_t del = 0;
  std::string sub_list;
 while((del = entities_list.find(';'))!=std::string::npos)
  {
    sub_list = entities_list.substr(0,del);
    size_t sub_index = 2;
      std::string en_pos_x_str;
      while(sub_list.at(sub_index)!=',')
      {
        en_pos_x_str.push_back(sub_list.at(sub_index));
        sub_index++;
      }
      int en_pos_x=Functions::stringToInteger(en_pos_x_str)-1;

      sub_index++;
      std::string en_pos_y_str;
      while(sub_list.at(sub_index)!=':')
      {
        en_pos_y_str.push_back(sub_list.at(sub_index));
        sub_index++;
      }
      int en_pos_y=Functions::stringToInteger(en_pos_y_str)-1;
      sub_index++;

      if(sub_list.at(0)=='D')
      {
        std::string leads_to_room_str;
        while(sub_index<sub_list.size())
        {
          leads_to_room_str.push_back(sub_list.at(sub_index));
          sub_index++;
        }
        int leads_to_room = Functions::stringToInteger(leads_to_room_str);
        PositionLocked *door = new PositionLocked("D",true,-1,leads_to_room);
        door->setPos(en_pos_x,en_pos_y);
        fields_.at(en_pos_x).at(en_pos_y)->setEntity(door);
      }
      else if(sub_list.at(0)=='Z')
      {
        std::string zombie_id_str;
        while(sub_index<sub_list.size())
        {
          zombie_id_str.push_back(sub_list.at(sub_index));
          sub_index++;
        }
        int zombie_id = Functions::stringToInteger(zombie_id_str);
        Enemy *zombie = new Enemy(15,1,3,0,zombie_id,"Z");
        zombie->setPos(en_pos_x,en_pos_y);
        fields_.at(en_pos_x).at(en_pos_y)->setEntity(zombie);
        number_of_enemies_++;
        enemies_.push_back(zombie);
      }
      else if(sub_list.at(0)=='G')
      {
        std::string goblin_id_str;
        while(sub_index<sub_list.size())
        {
          goblin_id_str.push_back(sub_list.at(sub_index));
          sub_index++;
        }
        int Goblin_id = Functions::stringToInteger(goblin_id_str);
        Enemy *goblin = new Enemy(10,4,3,4,Goblin_id,"G");
        goblin->setPos(en_pos_x,en_pos_y);
        fields_.at(en_pos_x).at(en_pos_y)->setEntity(goblin);
        number_of_enemies_++;
        enemies_.push_back(goblin);
      }
      else if(sub_list.at(0)=='L')
      {
        std::string lich_id_str;
        while(sub_index<sub_list.size())
        {
          lich_id_str.push_back(sub_list.at(sub_index));
          sub_index++;
        }
        int lich_id = Functions::stringToInteger(lich_id_str);
        Enemy *lich = new Enemy(50,6,2,8,lich_id,"L");
        lich->setPos(en_pos_x,en_pos_y);
        fields_.at(en_pos_x).at(en_pos_y)->setEntity(lich);
        number_of_enemies_++;
        enemies_.push_back(lich);
      }
      else if(sub_list.at(0)=='T')
      {
        sub_list.push_back(',');
        std::string dice_str;
        while(sub_list.at(sub_index)!=',')
        {
          dice_str.push_back(sub_list.at(sub_index));
          sub_index++;
        }
        int dice = Functions::stringToInteger(dice_str);
        PositionLocked *treasure = new PositionLocked("T",true,dice,-1);
        treasure->setPos(en_pos_x,en_pos_y);
        fields_.at(en_pos_x).at(en_pos_y)->setEntity(treasure);

        sub_index++;
        std::string item_to_add;
        std::string amount_str;
        bool item_flag = false;
        bool amount_flag = false;
        while(sub_index<sub_list.size())
        {
          if((sub_list.at(sub_index)!=',')&&!item_flag&&!amount_flag)
          {
            item_to_add.push_back(sub_list.at(sub_index));
          }
          else if(sub_list.at(sub_index)!=','&&item_flag&&!amount_flag)
          {
            amount_str.push_back(sub_list.at(sub_index));
          }

          if(sub_list.at(sub_index)==',')
          {
            if(!item_flag)
            {
              item_flag = true;
            }
            else if(item_flag && !amount_flag)
            {
              amount_flag = true;
            }
          }

          if(item_flag && amount_flag)
          {
            int amount = Functions::stringToInteger(amount_str);

            auto item_it = treasure->getAllItems()->find(item_to_add);
            if(item_it!=treasure->getAllItems()->end())
            {
              Item* item1 = item_it->second;
              treasure->addLoot(item1,amount);
            }
            amount_str.clear();
            item_to_add.clear();
            item_flag = false;
            amount_flag = false;
          }
          sub_index++;
        }
      }
      entities_list.erase(0,del+1);
    }

}



std::ostream& operator<<(std::ostream& stream, const Room* room)
{
  std::cout<<"    ";
  for (int i = 1; i <= room->dimension_X_; i++)
  {
    if (i > 9)
    {
      std::cout<<" "<<i<<" ";
    }
    else
    {
      std::cout<<" "<<i<<"  ";
    }
  }
  std::cout<<std::endl;
  std::cout<<"   +";
  for(int i = 1; i <= room->dimension_X_; i++)
  {
    std::cout<<"---+";
  }
  std::cout<<std::endl;

  for (int Y = 0; Y < room->dimension_Y_; Y++)
  {
    if (Y+1 > 9)
    {
      std::cout<<" "<<Y+1<<"|";
    }
    else
    {
      std::cout<<" "<<Y+1<<" |";
    }
    for(int X = 0; X < room->dimension_X_; X++)
    {
      std::cout<<*room->fields_[Y][X];
    }
    std::cout<<std::endl;
    std::cout<<"   +";
    for (int X = 0; X < room->dimension_X_; X++)
    {
        std::cout<<"---+";
    }
    std::cout<<std::endl;
  }

  return stream;
}

void Room::printAllEnemies()
{

  std::sort(enemies_.begin(), enemies_.end(), Functions::compareEnemies);
  size_t deadenemies = 0;
  for (auto& enem : enemies_)
  {
    if (!enem->getIfDead())
    {
      deadenemies++;

    }
  }
  if (deadenemies == enemies_.size())
  {
    std::cout<<std::endl;
    return;
  }
  std::cout<<"   ";
  for (auto* enemy : enemies_)
  {
    if (enemy->getIfDead())
    {
      std::cout<<enemy;
      if (enemy != enemies_[number_of_enemies_ - 1])
      {
        std::cout<<", ";
      }
    }
  }
    std::cout<<std::endl;
  }


Room::~Room()
{
  for (int Y = 0; Y < dimension_Y_; Y++)
  {
    for  (int X = 0; X < dimension_X_; X++)
    {
      delete fields_[Y][X];
    }
  }
}

Field* Room::getNeededRoom(std::string id)
{
  if (id != "W" && id != "B" && id != "R")
  {
    return nullptr;
  }


  for (int i = 0; i < dimension_Y_; i++)
  {
    for (int j = 0; j < dimension_X_; j++)
    {
      if (fields_[i][j]->getEntity() != nullptr)
      {
        if (fields_[i][j]->getEntity()->getId() == id)
        {
          return fields_[i][j];
        }
      }
    }
  }
  return nullptr;
}

std::pair<int,int> Room::getNeededDoor(int leads_to_room)
{
  for(int i = 0; i < dimension_Y_; i++)
  {
    for(int j = 0; j < dimension_X_; j++)
    {
      if(fields_[i][j]->getEntity() != nullptr)
      {
        if(fields_[i][j]->getEntity()->getId()=="D")
        {
          if(fields_[i][j]->getEntity()->getNumber()==leads_to_room)
          {
            return {i,j};
          }
        }
      }
    }
  }
  return {0,0};
}


bool Room::inBound(int y, int x,int dim_x, int dim_y)
{
  if(x >= 0 && x < dim_x && y >=0 && y < dim_y)
  {
    return true;
  }
  return false;
}


Field* Room::findFirstFreeClockwise(int y, int x)
{
  std::vector<std::pair<int,int>> clockwise = {{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}};

  for(auto direction : clockwise)
  {
    int field_y = y+direction.first;
    int field_x = x+direction.second;

    if(inBound(field_y,field_x,dimension_X_,dimension_Y_))
    {
      if(fields_[field_y][field_x]->getEntity()==nullptr)
      {
        return fields_[field_y][field_x];
      }
    }
  }
  return nullptr;
}

Field* Room::findFirstFreeClockwiseExp(int y, int x)
{
  int ring = 1;
  bool found = false;
  while(!found)
  {
    std::vector<std::pair<int, int>> directions;

      for (int i = -ring; i <= ring; i++)
      {
        if (i >= 0)
        {
          directions.push_back({-ring, i});
        }
      }

      for (int i = -ring + 1; i <= ring; i++)
      {
        directions.push_back({i, ring});
      }

      for (int i = ring - 1; i >= -ring; i--)
      {
        directions.push_back({ring, i});
      }

      for (int i = ring - 1; i >= -ring; i--)
      {
        directions.push_back({i, -ring});
      }



      for (auto& direction : directions)
      {
        int field_y = y + direction.first;
        int field_x = x + direction.second;
        if(inBound(field_y,field_x,dimension_X_,dimension_Y_))
        {
          if(fields_[field_y][field_x]->getEntity()==nullptr)
          {
            return fields_[field_y][field_x];
          }
        }
      }
      ring++;

  }
  return nullptr;
}


void Room::canMoveTO(Field* field, int y, int x)
{
  if (x - field->getX() > 1 || x - field->getX() < -1)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }

  if (y - field->getY() > 1 || y - field->getY() < -1)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }

  if (fields_[y][x]->getEntity() == nullptr)
  {
    return;
  }
  else if (fields_[y][x]->getEntity()->getId() == "D")
  {
    return;
  }
  else
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
}

void Room::canLoot(Field* field,int y, int x)
{
  if (x - field->getX() > 1 || x - field->getX() < -1)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  if (y - field->getY() > 1 || y - field->getY() < -1)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  if (fields_[y][x]->getEntity() == nullptr)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  if( fields_[y][x]->getEntity()->getId()!="T" && fields_[y][x]->getEntity()->getId()!="X")
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  return;
}


void Room::checkRange(int y, int x)
{
  if (y < 0 || y > dimension_Y_)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
  if (x < 0 || x > dimension_X_)
  {
    throw CommandExceptions("E_INVALID_POSITION");
  }
}

Field* Room::getNeededEnemy(Enemy* enemy)
{
  for (int i = 0; i < dimension_Y_; i++)
  {
    for (int j = 0; j < dimension_X_; j++)
    {
      if (fields_[i][j]->getEntity() != nullptr)
      {
        if (fields_[i][j]->getEntity() == enemy)
        {
          return fields_[i][j];
        }
      }
    }
  }
  return nullptr;
}




bool Room::inBorders(int y, int x)
{
  if (y < 0 || y >= dimension_Y_)
  {
    return false;
  }
  if (x < 0 || x >= dimension_X_)
  {
    return false;
  }
  return true;
}
void Room::unlockDoors()
{
 for (int Y = 0; Y < dimension_Y_; Y++)
  {
    for  (int X = 0; X < dimension_X_; X++)
    {
      Entity* ent = fields_[Y][X]->getEntity();
      if (ent != nullptr)
      {
        if (ent->getId() == "D")
        {
          ent->setLock(false);
        }
      }
    }
  }
}

void Room::removePlayers()
{
  for (int Y = 0; Y < dimension_Y_; Y++)
  {
    for  (int X = 0; X < dimension_X_; X++)
    {
      Entity* ent = fields_[Y][X]->getEntity();
      if (ent != nullptr)
      {
        if (ent->getId() == "W"|| ent->getId() == "B"|| ent->getId() == "R")
        {
          fields_[Y][X]->setEntity(nullptr);
        }
      }
    }
  }
}
Field* Room::findFirstFreeClockwisePlr(int y, int x)
{
  int ring = 1;
  bool found = false;
  while(!found)
  {
    std::vector<std::pair<int, int>> directions;

      for (int i = -ring; i <= ring; i++)
      {
        if (i >= 0)
        {
          directions.push_back({-ring, i});
        }
      }

      for (int i = -ring + 1; i <= ring; i++)
      {
        directions.push_back({i, ring});
      }

      for (int i = ring - 1; i >= -ring; i--)
      {
        directions.push_back({ring, i});
      }

      for (int i = ring - 1; i >= -ring; i--)
      {
        directions.push_back({i, -ring});
      }



      for (auto& direction : directions)
      {
        int field_y = y + direction.first;
        int field_x = x + direction.second;
        if(inBound(field_y,field_x,dimension_X_,dimension_Y_))
        {
          if(fields_[field_y][field_x]->getEntity()!=nullptr)
          {
            if (fields_[field_y][field_x]->getEntity()->getIfplayer())
            {
            return fields_[field_y][field_x];
            }
          }
        }
      }
      ring++;

  }
  return nullptr;
}

Field* Room::findEntById(std::string id, std::vector<std::pair<int, int>>& added_coords)
{
  for (int i = 0; i < dimension_Y_; i++)
  {
    for (int j = 0; j < dimension_X_; j++)
    {
      if (fields_[i][j]->getEntity() != nullptr)
      {
        if (fields_[i][j]->getEntity()->getId() == id)
        {
          int x = fields_[i][j]->getX();
          int y = fields_[i][j]->getY();
          std::pair<int, int> coords = {y, x};

          bool already_added = false;
          for (const auto& coord : added_coords)
          {
            if (coord == coords)
            {
              already_added = true;
              break;
            }
          }
          if (already_added)
          {
            continue;
          }
            added_coords.push_back({fields_[i][j]->getY(), fields_[i][j]->getX()});
            return fields_[i][j];
        }
      }
    }
  }
  return nullptr;
}
