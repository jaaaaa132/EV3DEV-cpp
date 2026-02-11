#pragma once

#include <math.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "position.h"
#include "motor.h"
#include "sensor.h"

class Diferential_drive {
  private:

  protected:
    const float pi = 3.14159;
    Motor* const left_motor;
    Motor* const right_motor;
    const bool left_motor_inverted;
    const bool right_motor_inverted;
    Sensor* const gyro;
    Sensor* const color;
    const float wheel_base_width; // in cm
    const float wheel_circumference;   // in cm
    Position_atomic position;
    std::atomic<bool> tracking_position;
    std::thread position_tracker;

    float normalize_angle(float angle); // returns value in interval <pi ; -pi>  
    void track_position();

  public:
    Diferential_drive(Motor& p_left_motor, Motor& p_right_motor, Sensor& p_gyro, Sensor& p_color, const float p_wheel_base_width, const float p_wheel_diameter, const bool p_left_motor_inverted = false, const bool p_right_motor_inverted = false, const Position starting_position = Position(0, 0, 0));
    ~Diferential_drive();

    Position live_position();
    void reset_position(Position new_position);

    void go_to_position_curve(const Position target_position, const float precision, const int max_motor_speed = 100, const bool forward_only = false); // angle in position is ignored
    void go_to_position_straight(const Position target_position, const float angle_precision, const int max_motor_speed = 100, const bool forward_only = false); // angle in position is ignored
    void move_tank_direct_timed(const int left_motor_speed, const int right_motor_speed, const int time_ms, const char* stop_action);
    void rotate_to_abs_angle(const float angle, const float precision, const int max_speed = 100);
    void rotate_to_position(const Position target, const float precision, const int max_speed = 100, const float offset = 0); // angle in position is ignored
    void go_until_reflection(const int reflection, const bool darker, const int speed);
    void follow_path_curve(const std::string file_path, const float precision, const float angle_precision, const int max_speed = 100);
    void follow_path_straight(const std::string file_path, const float angle_precision, const int max_speed = 100);
};

