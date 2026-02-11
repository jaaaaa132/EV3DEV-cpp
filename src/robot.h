#pragma once

#include <vector>
#include "diferential_drive.h"
#include "sensor.h"

class Robot: public Diferential_drive {
  private:
    
  protected:
    Motor* left_arm;
    Motor* right_arm;
  public:
    Robot(Motor& p_left_arm, Motor& p_right_arm, Motor& p_left_motor, Motor& p_right_motor, Sensor& p_gyro, Sensor& p_color, const float p_wheel_base_width, const float p_wheel_circumference, const bool p_left_motor_inverted = false, const bool p_right_motor_inverted = false, const Position starting_position = Position(0, 0, 0));
    ~Robot();

    void follow_program(const std::string file_path);
};