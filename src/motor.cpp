#include"motor.h"
#include <cmath>
#include <fstream>
#include <stdexcept>

void Motor::open_files(){
	speed_file.open(std::string("/sys/class/tacho-motor/")        + directory + std::string("/speed_sp"),     std::fstream::out);
  position_file.open(std::string("/sys/class/tacho-motor/")     + directory + std::string("/position"));
  position_sp_file.open(std::string("/sys/class/tacho-motor/")  + directory + std::string("/position_sp"),  std::fstream::out);
  acceleration_file.open(std::string("/sys/class/tacho-motor/") + directory + std::string("/ramp_up_sp"),   std::fstream::out);
  stop_action_file.open(std::string("/sys/class/tacho-motor/")  + directory + std::string("/stop_action"),  std::fstream::out);
  command_file.open(std::string("/sys/class/tacho-motor/")      + directory + std::string("/command"),      std::fstream::out);
  duty_cycle_file.open(std::string("/sys/class/tacho-motor/")   + directory + std::string("/duty_cycle_sp"),std::fstream::out);
  polarity_file.open(std::string("/sys/class/tacho-motor/")     + directory + std::string("/polarity"));
}

void Motor::debug_output_file(std::string file_name){
  bool const cout_output = true;
  bool const cerr_output = true;

  if(cout_output)	std::cout << "couldnt open: " << file_name << " from: " << directory << std::endl;
	if(cerr_output)	std::cerr << "couldnt open: " << file_name << " from: " << directory << std::endl;
}

template<typename FileType> bool Motor::check_file(FileType& file, std::string debug_name){
  if(!file.is_open()){
    debug_output_file(debug_name);
    return false;
  }
  else{
    return true;
  }
}

bool Motor::are_files_opened(){
  return  check_file(speed_file, "speed_file") &&
          check_file(position_file, "position_file") &&
          check_file(position_sp_file, "position_sp_file") &&
          check_file(acceleration_file, "acceleration_file") &&
          check_file(stop_action_file, "stop_action_file") &&
	        check_file(command_file, "command_file") &&	
          check_file(duty_cycle_file, "duty_cycle_file") &&
	        check_file(polarity_file, "polarity_file");
}

bool Motor::is_connected(){
  return connected;
}


void Motor::set_directory(std::string p_directory){
	directory = p_directory;
	connected = true;
	open_files();	
}

void Motor::run(int speed, int acceleration){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }

  // run motor
  acceleration_file << std::to_string(acceleration);
	acceleration_file.flush();
	speed_file << std::to_string(speed);
	speed_file.flush();
	command_file << "run-forever";
	command_file.flush();
}

void Motor::run_to_abs_pos(int position, int speed, std::string stop_action){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
	  
  // run motor to absolute position
  position_sp_file << std::to_string(position);
  position_sp_file.flush();
	speed_file << std::to_string(speed);
	speed_file.flush();
  stop_action_file << stop_action;
  stop_action_file.flush();
	command_file << "run-to-abs-pos";
	command_file.flush();
}
void Motor::run_to_rel_pos(int position, int speed, std::string stop_action){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
	
  // run motor to relative position
  position_sp_file << std::to_string(position);
  position_sp_file.flush();
	speed_file << std::to_string(speed);
	speed_file.flush();
  stop_action_file << stop_action;
  stop_action_file.flush();
	command_file << "run-to-rel-pos";
	command_file.flush();
}

void Motor::run_direct(int duty_cycle, bool inverted){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }

  if(inverted){
    polarity_file << "inversed";
  }
  else{
    polarity_file << "normal";
  }
  polarity_file.flush();

  duty_cycle_file << std::to_string(duty_cycle);
  duty_cycle_file.flush();
  command_file << "run-direct";
  command_file.flush();
}

int Motor::get_position(){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
  
  int pos;
  position_file.clear(); // Clear any stream error flags.
  position_file.seekg(0, std::ios::beg); 
  position_file >> pos; 
 
  // to be fixed: reliaes on position to be set to high number at start
  return abs(pos);
}

void Motor::set_position(int new_position){
  if(!are_files_opened()){
    open_files();
    if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }

  position_file << std::to_string(new_position);
  position_file.flush();
  return;
}
