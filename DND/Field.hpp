#ifndef FIELD_HPP
#define FIELD_HPP

#include "Entity.hpp"
#include <iostream>

class Field
{
private:
  int posX_;
  int posY_;
  Entity* current_entity_;

public:

  friend std::ostream& operator<<(std::ostream& stream, const Field& field);

  Field(int x, int y);

  void setEntity(Entity *entity);

  Entity* getEntity();
  int getX();
  int getY();

  Field(const Field&) = delete;

  ~Field();
};



#endif
