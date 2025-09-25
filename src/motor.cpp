#include "motor.h"
#include <cmath>
#include <fstream>
#include <ratio>
#include <stdexcept>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <experimental/filesystem>

void Motor::open_files(){
	speed_file.open(std::string("/sys/class/tacho-motor/")        + directory + std::string("/speed_sp"),     std::fstream::out);
  position_file.open(std::string("/sys/class/tacho-motor/")     + directory + std::string("/position"));
  position_sp_file.open(std::string("/sys/class/tacho-motor/")  + directory + std::string("/position_sp"),  std::fstream::out);
  acceleration_file.open(std::string("/sys/class/tacho-motor/") + directory + std::string("/ramp_up_sp"),   std::fstream::out);
  stop_action_file.open(std::string("/sys/class/tacho-motor/")  + directory + std::string("/stop_action"),  std::fstream::out);
  command_file.open(std::string("/sys/class/tacho-motor/")      + directory + std::string("/command"),      std::fstream::out);
  duty_cycle_file.open(std::string("/sys/class/tacho-motor/")   + directory + std::string("/duty_cycle_sp"),std::fstream::out);
  polarity_file.open(std::string("/sys/class/tacho-motor/")     + directory + std::string("/polarity"));
  state_file.open(std::string("/sys/class/tacho-motor/")        + directory + std::string("/state"),        std::fstream::in);
  time_file.open(std::string("/sys/class/tacho-motor/")         + directory + std::string("/time_sp"),       std::fstream::out);
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
	        check_file(polarity_file, "polarity_file") &&
          check_file(state_file, "state_file") &&
          check_file(time_file, "time_file");
}

Motor::Motor(){

}

Motor::~Motor(){

}

std::array<Motor, 4> Motor::find_motors(){
  // find all connected motors
	std::vector<std::string> motor_directories;	
	std::string motor_path = "/sys/class/tacho-motor";
	for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::directory_iterator(motor_path)) {
		if (std::experimental::filesystem::is_directory(entry.path())) {
      	motor_directories.push_back(entry.path().filename().string());
    }
  }

	std::array<Motor, 4> motors;
	
	for(std::size_t i = 0; i < motor_directories.size(); i++){
		// finding port of motor
    std::string address_file_path = "/sys/class/tacho-motor/" + motor_directories.at(i) + "/address";
		std::ifstream address_file;
		address_file.open(address_file_path, std::fstream::in);
		if(!address_file.is_open()){
			std::cout << "file could not be opened: " << address_file_path << std::endl;
			std::cerr << "file could not be opened: " << address_file_path << std::endl;
			continue;
		}
		int port;
		std::string address;
		address_file >> address;
		port = address.at(13) - 'A';
    // configuring motor to find it's directory
		switch(port){
			case 0:	motors.at(0).set_directory(motor_directories.at(i));break;
			case 1:	motors.at(1).set_directory(motor_directories.at(i));break;
			case 2:	motors.at(2).set_directory(motor_directories.at(i));break;
			case 3:	motors.at(3).set_directory(motor_directories.at(i));break;
		}
	   
	}
  return motors;
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

void Motor::run_for_time(int time_ms, int speed, std::string stop_action){
 if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
	
  // run motor for time
  time_file << std::to_string(time_ms);
  time_file.flush();
	speed_file << std::to_string(speed);
	speed_file.flush();
  stop_action_file << stop_action;
  stop_action_file.flush();
	command_file << "run-timed";
	command_file.flush(); 
}

void Motor::stop(std::string stop_action){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }

  stop_action_file << stop_action;
  stop_action_file.flush();
  command_file << "stop";
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

void Motor::run_direct_for_time(int duty_cycle, int time_ms, bool inverted, std::string stop_action){
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

  std::thread t([this, time_ms, stop_action]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
    stop(stop_action);
  });
  t.detach();
}

std::string Motor::get_state(){
  if(!are_files_opened()){
    open_files();
		if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
  
  std::string state;
  state_file.clear(); // Clear any stream error flags.
  state_file.seekg(0, std::ios::beg); 
  state_file >> state; 

  return state;
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

  return pos;
}

void Motor::set_position(int new_position){
  if(!are_files_opened()){
    open_files();
    if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
  
  position_file.clear();
  position_file.seekp(0, std::ios::beg);
  position_file << std::to_string(new_position);
  position_file.flush();
  return;
}

void Motor::wait_for_stop(){
  if(!are_files_opened()){
    open_files();
    if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }

  while(get_state().find("running") != std::string::npos || get_state().find("ramping") != std::string::npos) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
