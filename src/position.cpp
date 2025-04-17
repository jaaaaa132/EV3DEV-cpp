#include"position.h"
  
Position::Position(float position_x, float position_y, float p_angle) : x(position_x), y(position_y), angle(p_angle){
  
}
  
Position& Position::operator=(const Position& other){
  if(&other != this){
    x = other.x;
    y = other.y;
    angle = other.angle;
  }

  return *this;
}

Position& Position::operator=(const Position_atomic& other){
  x = other.x.load();
  y = other.y.load();
  angle = other.angle.load();

  return *this;
}
  
Position_atomic::Position_atomic(float position_x, float position_y, float p_angle){
  x.store(position_x);
  y.store(position_y);
  angle.store(p_angle);
}
  
Position_atomic& Position_atomic::operator=(const Position_atomic& other){
  if(&other != this){
    x.store(other.x.load());
    y.store(other.y.load());
    angle.store(other.angle.load());
  }
    
  return *this;
}

Position_atomic& Position_atomic::operator=(const Position& other){
  x.store(other.x);
  y.store(other.y);
  angle.store(other.angle);
  return *this;
}

Position_atomic::operator Position() const{
  Position p;
  p = *this;
  return p;
}
