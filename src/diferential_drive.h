#pragma once

#include<math.h>
#include<atomic>
#include<chrono>
#include<thread>
#include"position.h"
#include"motor.h"

class Diferential_drive {
  private:

  protected:
    const float pi = 3.14159;
  Motor* left_motor;
    Motor* right_motor;
    bool left_motor_inverted;
    bool right_motor_inverted;
    float wheel_base_width; // in cm
    float wheel_diameter;   // in cm
    Position_atomic position;
    std::atomic<bool> tracking_position;
    std::thread position_tracker;

    float normalize_angle(float angle); // returns value in interval <pi ; -pi>  
    void track_position();

  public:
    Diferential_drive(Motor& p_left_motor, Motor& p_right_motor, float p_wheel_base_width, float p_wheel_diameter, bool p_left_motor_inverted = false, bool p_right_motor_inverted = false, Position starting_position = Position(0, 0, 0));
    ~Diferential_drive();

    Position live_position();

    void go_to_position_curve(Position target_position, float precision, int max_motor_speed = 100, bool forward_only = false); // angle in position is ignored
    void go_to_position_straight(Position target_position, float angle_precision, int max_motor_speed = 100, bool forward_only = false); // angle in position is ignored
    void rotate_to_abs_angle(float angle, float precision, int max_speed = 100);
    void rotate_to_position(Position target, float precision, int max_speed = 100, float offset = 0); // angle in position is ignored
    void follow_path_curve(std::string file_path, float precision, float angle_precision, int max_speed = 100);
    void follow_path_straight(std::string file_path, float precision, float angle_precision, int max_speed = 100);
};

