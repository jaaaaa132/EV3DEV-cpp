#include "robot.h"
#include "sensor.h"
#include <string>
#include <limits>

Robot::Robot(Motor& p_left_arm, Motor& p_right_arm, Motor& p_left_motor, Motor& p_right_motor, Sensor& p_gyro, Sensor& p_color_left, Sensor& p_color_right, float p_wheel_base_width, float p_wheel_circumference, bool p_left_motor_inverted, bool p_right_motor_inverted, Position starting_position):
  Diferential_drive(p_left_motor, p_right_motor, p_gyro, p_color_left, p_color_right, p_wheel_base_width, p_wheel_circumference, p_left_motor_inverted, p_right_motor_inverted, starting_position),
  left_arm(&p_left_arm),
  right_arm(&p_right_arm){ 
}

Robot::~Robot(){

}

void Robot::follow_program(std::string file_path){
  std::ifstream file;
  file.open(file_path);
  if(!file.is_open())  throw std::runtime_error("cann't open given path_file in Robot::follow_program()");
  
  char action;
  std::string arguments[5];
  Motor* arm = nullptr;
  while(file >> action){
    switch(action){
      case 'O': for(int i = 0; i < 3; i++) file >> arguments[i]; reset_position(Position(stof(arguments[0]), stof(arguments[1]), stof(arguments[2]))); break;
      case 'K': file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); break;
      case 'S': for(int i = 0; i < 5; i++) file >> arguments[i]; go_to_position_straight(Position(stof(arguments[0]), stof(arguments[1]), 0), stof(arguments[2]), stoi(arguments[3]), "1" == arguments[4]);  break;
      case 'C': for(int i = 0; i < 5; i++) file >> arguments[i]; go_to_position_curve(Position(stof(arguments[0]), stof(arguments[1]), 0), stof(arguments[2]), stoi(arguments[3]), "1" == arguments[4]);  break; 
      case 'T': for(int i = 0; i < 4; i++) file >> arguments[i]; move_tank_direct_timed(stoi(arguments[0]), stoi(arguments[1]), stoi(arguments[2]), arguments[3].c_str()); break;
      case 'A': for(int i = 0; i < 3; i++) file >> arguments[i]; rotate_to_abs_angle(stof(arguments[0]), stof(arguments[1]), stoi(arguments[2]));  break;
      case 'P': for(int i = 0; i < 5; i++) file >> arguments[i]; go_to_position_straight(Position(stof(arguments[0]), stof(arguments[1]), 0), stof(arguments[2]), stoi(arguments[3]), std::stof(arguments[4]));  break;
      case 'F': for(int i = 0; i < 3; i++) file >> arguments[i]; go_until_reflection(stoi(arguments[0]), "1" == arguments[1], stoi(arguments[2]));
      case 'L': arm = left_arm;
      case 'R': if(action == 'R') arm = right_arm;
        file >> action;
        switch(action){
          case 'R': for(int i = 0; i < 2; i++) file >> arguments[i]; arm->run(stoi(arguments[0]), stoi(arguments[1]));  break;
          case 'S': for(int i = 0; i < 1; i++) file >> arguments[i]; arm->run_direct(0); arm->stop(arguments[0].c_str());  break; // run_direct(0) to reset polarity
          case 'A': for(int i = 0; i < 3; i++) file >> arguments[i]; arm->run_to_abs_pos(stoi(arguments[0]), stoi(arguments[1]), arguments[2].c_str());  break;
          case 'M': for(int i = 0; i < 3; i++) file >> arguments[i]; arm->run_to_rel_pos(stoi(arguments[0]), stoi(arguments[1]), arguments[2].c_str());  break;
          case 'T': for(int i = 0; i < 3; i++) file >> arguments[i]; arm->run_for_time(stoi(arguments[0]), stoi(arguments[1]), arguments[2].c_str());  break;
          case 'D': for(int i = 0; i < 2; i++) file >> arguments[i]; arm->run_direct(stoi(arguments[0]), "1" == arguments[1]); break;
          case 'J': for(int i = 0; i < 4; i++) file >> arguments[i]; arm->run_direct_for_time(stoi(arguments[0]), stoi(arguments[1]), "1" == arguments[2], arguments[3].c_str()); break;
          case 'W': arm->wait_for_stop();
        }
    }
  }
}
