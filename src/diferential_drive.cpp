#include"diferential_drive.h"
#include "position.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

float Diferential_drive::normalize_angle(float angle){ // returns value in interval <pi ; -pi>
  while(angle > pi) angle -= pi*2;
  while(angle < -pi) angle += pi*2;
  return angle;
}

void Diferential_drive::track_position(){
  Position p_position;
  int left_motor_last_pos, right_motor_last_pos;

  try{ // trying once again (already implemented in motor.cpp)
    // relies on setting position to high value
    left_motor_last_pos = abs(left_motor->get_position());
    right_motor_last_pos = abs(right_motor->get_position());
  }
  catch(std::runtime_error& error){
    std::cout << error.what() << std::endl;
    try{
      left_motor_last_pos = abs(left_motor->get_position());
      right_motor_last_pos = abs(right_motor->get_position());
    }
    catch(std::runtime_error& error){
      std::cout << "rethrowing error from tracking_position() in Diferential_drive: " << error.what() << std::endl;
      throw error;
    }
  }

  while(tracking_position.load()){
    p_position = position;
    int left_motor_pos, right_motor_pos;
    
    try{ // trying once again (already implemented in motor.cpp) 
      left_motor_pos = abs(left_motor->get_position());
      right_motor_pos = abs(right_motor->get_position());
    }
    catch(std::runtime_error& error){
      std::cout << error.what() << std::endl;
      try{
        left_motor_pos = abs(left_motor->get_position());
        right_motor_pos = abs(right_motor->get_position());
      }
      catch(std::runtime_error& error){
        std::cout << "rethrowing error from tracking_position() in Diferential_drive: " << error.what() << std::endl;
        throw error;
      }
    }

    int left_motor_pos_dif = (left_motor_pos - left_motor_last_pos) * (left_motor_inverted ? -1 : 1);
    int right_motor_pos_dif = (right_motor_pos - right_motor_last_pos) * (right_motor_inverted ? -1 : 1);
    float left_wheel_dist = wheel_circumference * left_motor_pos_dif / 360;
    float right_wheel_dist = wheel_circumference * right_motor_pos_dif / 360;
    float average_distance = (left_wheel_dist + right_wheel_dist) / 2;
    float orientation_dif = (right_wheel_dist - left_wheel_dist) / wheel_base_width;
    p_position.x -= average_distance * sin(position.angle + orientation_dif / 2);
    p_position.y += average_distance * cos(position.angle + orientation_dif / 2);
    p_position.angle += orientation_dif;

    position = p_position;
    left_motor_last_pos = left_motor_pos;
    right_motor_last_pos = right_motor_pos;
    //std::cout << position.x << " " << position.y << " " << position.angle << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
}

Diferential_drive::Diferential_drive(Motor& p_left_motor, Motor& p_right_motor, float p_wheel_base_width, float p_wheel_circumference, bool p_left_motor_inverted, bool p_right_motor_inverted, Position starting_position): 
  left_motor(&p_left_motor),
  right_motor(&p_right_motor), 
  left_motor_inverted(p_left_motor_inverted),      
  right_motor_inverted(p_right_motor_inverted),
  wheel_base_width(p_wheel_base_width),
  wheel_circumference(p_wheel_circumference){
      
  position = starting_position;
  left_motor->set_position(1000000);  // large value so tracking would work with run_direct
  right_motor->set_position(1000000);
  tracking_position.store(true);
  position_tracker = std::thread(&Diferential_drive::track_position, this);
}

Diferential_drive::~Diferential_drive(){
  tracking_position.store(false);
  position_tracker.join();
}

Position Diferential_drive::live_position(){
  return position;
}

void Diferential_drive::go_to_position_curve(Position target_position, float precision, int max_motor_speed, bool forward_only){
  // comented out Derivative calculation

  //std::chrono::steady_clock::time_point last_time_measurement = std::chrono::steady_clock::now();
  //float last_goal_distance = sqrt(pow(target_position.x - Position(position).x, 2) + pow(target_position.y - Position(position).y, 2));
  //float last_goal_angle_diffrence = normalize_angle(atan2(Position(position).x - target_position.x, target_position.y - Position(position).y) - Position(position).angle);
   
  while(true){
    float goal_distance = sqrt(pow(target_position.x - Position(position).x, 2) + pow(target_position.y - Position(position).y, 2));
    if (goal_distance < precision) break;
    float goal_angle_diffrence = normalize_angle(atan2(Position(position).x - target_position.x, target_position.y - Position(position).y) - Position(position).angle);
        
    std::cout << "distance dif: " << goal_distance << " angle dif: " << goal_angle_diffrence << " x: " << Position(position).x << " y: " << Position(position).y << " angle: " << Position(position).angle << "\n";
        
    // calculate time for PD
    //std::chrono::steady_clock::time_point new_time_measurement = std::chrono::steady_clock::now();
    //std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(new_time_measurement - last_time_measurement);
        
    // value ranges from 0 to 1 calculated using PD
    const float P_angle_const = 2.5;
    //const float D_angle_const = 0; // D not needed should be removed from equsion not tested if works
    const float P_distance_const = 4.5;
    float angular_modifier = 1 - fmin(1, fmax(0, abs(fmin(abs(goal_angle_diffrence),abs(pi - goal_angle_diffrence)) / (pi / 2) * P_angle_const /*+ (last_goal_angle_diffrence - goal_angle_diffrence) / time_span.count() * D_angle_const*/) / wheel_circumference * wheel_base_width));  
    float speed_modifier = fmin(1, fmax(0, goal_distance / P_distance_const * wheel_circumference));
        
    int left_motor_speed = max_motor_speed * speed_modifier, right_motor_speed = max_motor_speed * speed_modifier;
        
    // righthand or lefthand turn
    if(goal_angle_diffrence > 0)  left_motor_speed *= angular_modifier;
    else  right_motor_speed *= angular_modifier;

    // forwards or backwards
    bool backwards = false;
    if(abs(goal_angle_diffrence) > pi / 2 && !forward_only){
      backwards = true;
    }
    try{  
      left_motor->run_direct(left_motor_speed, left_motor_inverted ? !backwards : backwards);
      right_motor->run_direct(right_motor_speed, right_motor_inverted ? !backwards : backwards);
    }
    catch(std::runtime_error& error){
      std::cout << error.what() << std::endl;
      try{ // trying once again (already implemented in motor.cpp)
        left_motor->run_direct(left_motor_speed, left_motor_inverted ? !backwards : backwards);
        right_motor->run_direct(right_motor_speed, right_motor_inverted ? !backwards : backwards);
      }
      catch(std::runtime_error& error){
        std::cout << "rethrowing error from go_to_position_curve in Diferential_drive: " << error.what() << std::endl;
        throw error;
      }
    }
    //last_time_measurement = new_time_measurement;
    //last_goal_distance = goal_distance;
    //last_goal_angle_diffrence = goal_angle_diffrence;
  }
  
  try{
    // resets polarity propably not needed
    left_motor->run_direct(0);
    right_motor->run_direct(0);
    
    left_motor->stop();
    right_motor->stop();
  }
  catch(std::runtime_error& error){
    std::cout << "rethrowing error from go_to_position_curve in Diferential_drive: " << error.what() << std::endl;
      throw error;
  }
} 

void Diferential_drive::go_to_position_straight(Position target_position, float angle_precision, int max_motor_speed, bool forward_only){
  bool backwards = false;
  if(!forward_only && abs(normalize_angle(atan2(Position(position).x - target_position.x, target_position.y - Position(position).y) - Position(position).angle)) > pi/2 ){
    rotate_to_position(target_position, angle_precision, max_motor_speed, pi);
    backwards = true;
  }
  else{
    rotate_to_position(target_position, angle_precision, max_motor_speed);
    backwards = false;
  }
  float target_distance = sqrt(pow(target_position.x - position.x, 2) + pow(target_position.y - position.y, 2)) * (backwards ? -1 : 1);
  float target_distance_wheele_deg = target_distance / wheel_circumference * 360;
  left_motor->run_to_rel_pos(target_distance_wheele_deg * (left_motor_inverted ? -1 : 1), max_motor_speed * 10.5);
  right_motor->run_to_rel_pos(target_distance_wheele_deg * (right_motor_inverted ? -1 : 1), max_motor_speed * 10.5);
  //std::cout << (target_distance_wheele_deg * (right_motor_inverted ? -1 : 1)) << std::endl;
  left_motor->wait_for_stop();
  right_motor->wait_for_stop();
}

void Diferential_drive::move_tank_direct_timed(int left_motor_speed, int right_motor_speed, int time_ms, std::string stop_action){
  left_motor->run_direct(abs(left_motor_speed), left_motor_inverted ? left_motor_speed > 0 : left_motor_speed < 0);
  right_motor->run_direct(abs(right_motor_speed), right_motor_inverted ? right_motor_speed > 0 : right_motor_speed < 0);
  
  std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
  
  left_motor->run_direct(0); // reseting polarity propably not needed
  right_motor->run_direct(0); // reseting polarity propably not needed
  left_motor->stop(stop_action);
  right_motor->stop(stop_action);
}

void Diferential_drive::rotate_to_abs_angle(float angle, float precision, int max_speed){
  float angle_dif = normalize_angle(position.angle - angle);
  constexpr float P_const = 25;
  while(abs(angle_dif) > precision){
    int speed = std::fmin(1, std::fmax(-1, angle_dif * P_const));
    //std::cout << angle_dif << std::endl;
    left_motor->run_direct(abs(speed) * max_speed, left_motor_inverted ? !(speed < 0) : speed < 0);
    right_motor->run_direct(abs(speed) * max_speed, !right_motor_inverted ? !(speed < 0) : speed < 0);
    angle_dif = normalize_angle(position.angle - angle);
  }

  // resets polarity propably not needed
  left_motor->run_direct(0);
  right_motor->run_direct(0);

  left_motor->stop();
  right_motor->stop();
}

void Diferential_drive::rotate_to_position(Position target, float precision, int max_speed, float offset){
  rotate_to_abs_angle(atan2(Position(position).x - target.x, target.y - Position(position).y) + offset, precision, max_speed);
}

void Diferential_drive::follow_path_curve(std::string file_path, float precision, float angle_precision, int max_speed){
  std::ifstream file;
  file.open(file_path);
  if(!file.is_open())  throw std::runtime_error("cann't open given path_file in Diferential_drive::follow_path_curve()");

  float x,y; 
  while(file >> x){
    if(file >> y) go_to_position_curve(Position(x, y, 0), precision, max_speed, false);
    else rotate_to_abs_angle(x, angle_precision, max_speed);
  }    
  file.close();
}

void Diferential_drive::follow_path_straight(std::string file_path, float angle_precision, int max_speed){
  std::ifstream file;
  file.open(file_path);
  if(!file.is_open())  throw std::runtime_error("cann't open given path_file in Diferential_drive::follow_path_straight()");

  float x,y;
  while(file >> x){
    if(file >> y) go_to_position_straight(Position(x, y, 0), angle_precision, max_speed, false);
    else rotate_to_abs_angle(x, angle_precision, max_speed);
  }
}
