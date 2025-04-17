#pragma once

#include<atomic>

struct Position;
struct Position_atomic;

struct Position{
public:
  float x,y,angle;
  
  Position(float position_x = 0, float position_y = 0, float p_angle = 0);
  
  Position& operator=(const Position& other);
  Position& operator=(const Position_atomic& other);
};

struct Position_atomic{
public:
  std::atomic<float> x,y,angle;
  
  Position_atomic(float position_x = 0, float position_y = 0, float p_angle = 0);
  
  Position_atomic& operator=(const Position_atomic& other);
  Position_atomic& operator=(const Position& other);
  operator Position() const;
};


