#pragma once

#include<vector>
#include"diferential_drive.h"

class Robot: public Diferential_drive {
  private:
    
  protected:
    Motor* left_arm;
    Motor* right_arm;
  public:
    Robot(Motor& p_left_arm, Motor& p_right_arm, Motor& p_left_motor, Motor& p_right_motor, float p_wheel_base_width, float p_wheel_circumference, bool p_left_motor_inverted = false, bool p_right_motor_inverted = false, Position starting_position = Position(0, 0, 0));
    ~Robot();

    void follow_program(std::string file_path);
};