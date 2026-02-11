#include "motor.h"
#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <experimental/filesystem>

void Motor::open_files(){
	speed_file =          open((std::string("/sys/class/tacho-motor/") + directory + std::string("/speed_sp")).c_str(),      O_WRONLY);
  position_file =       open((std::string("/sys/class/tacho-motor/") + directory + std::string("/position")).c_str(),      O_RDWR | O_CLOEXEC);
  position_sp_file =    open((std::string("/sys/class/tacho-motor/") + directory + std::string("/position_sp")).c_str(),   O_WRONLY);
  acceleration_file =   open((std::string("/sys/class/tacho-motor/") + directory + std::string("/ramp_up_sp")).c_str(),    O_WRONLY);
  deacceleration_file = open((std::string("/sys/class/tacho-motor/") + directory + std::string("/ramp_down_sp")).c_str(),  O_WRONLY);
  stop_action_file =    open((std::string("/sys/class/tacho-motor/") + directory + std::string("/stop_action")).c_str(),   O_WRONLY);
  command_file =        open((std::string("/sys/class/tacho-motor/") + directory + std::string("/command")).c_str(),       O_WRONLY);
  duty_cycle_file =     open((std::string("/sys/class/tacho-motor/") + directory + std::string("/duty_cycle_sp")).c_str(), O_WRONLY);
  polarity_file =       open((std::string("/sys/class/tacho-motor/") + directory + std::string("/polarity")).c_str(),      O_WRONLY);
  state_file =          open((std::string("/sys/class/tacho-motor/") + directory + std::string("/state")).c_str(),         O_RDONLY);
  time_file =           open((std::string("/sys/class/tacho-motor/") + directory + std::string("/time_sp")).c_str(),       O_WRONLY);
}

static bool write_c_str(int fd, const char* buf, size_t len) {
    return write(fd, buf, len) == (ssize_t)len;
}

static bool write_int_fd(int fd, const int value) {
    char tmp[7];
    int n = snprintf(tmp, sizeof(tmp), "%d", value);
    return n >= 0 && write_c_str(fd, tmp, (size_t)n);
}

static void read_fd(int fd, char* buf, size_t bufsize) {
    lseek(fd, 0, SEEK_SET);
    ssize_t n = read(fd, buf, bufsize - 1);
    if (n >= 0) buf[n] = '\0';
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
  for(int i  = 0; i < max_loop_count; i++){ if (
    write_int_fd(acceleration_file, acceleration) &&
	  write_int_fd(speed_file, speed) &&
	  write_c_str(command_file, "run-forever", sizeof("run-forever"))
  ){
    return;
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void Motor::run_to_abs_pos(int position, int speed, const char* stop_action){  
  write_int_fd(position_sp_file, position);
	write_int_fd(speed_file, speed);
  write_c_str(stop_action_file, stop_action, strlen((stop_action))+1);
	write_c_str(command_file, "run-to-abs-pos", sizeof("run-to-abs-pos"));
}
void Motor::run_to_rel_pos(int position, int speed, const char* stop_action){
  write_int_fd(position_sp_file, position);
	write_int_fd(speed_file, speed);
  write_c_str(stop_action_file, stop_action, strlen((stop_action))+1);
	write_c_str(command_file, "run-to-rel-pos", sizeof("run-to-rel-pos"));
}

void Motor::run_for_time(int time_ms, int speed, const char* stop_action){
  write_int_fd(time_file, time_ms);
	write_int_fd(speed_file, speed);
  write_c_str(stop_action_file, stop_action, strlen((stop_action))+1);
  write_c_str(command_file, "run-timed", sizeof("run-timed"));
}

void Motor::stop(const char* stop_action){
  write_c_str(stop_action_file, stop_action, strlen((stop_action))+1);
  write_c_str(command_file, "stop", sizeof("stop"));
}

void Motor::run_direct(int duty_cycle, bool inverted){
  if(inverted) write_c_str(polarity_file, "inversed", sizeof("inversed"));
  else write_c_str(polarity_file, "normal", sizeof("normal"));
  write_int_fd(duty_cycle_file, duty_cycle);
  write_c_str(command_file, "run-direct", sizeof("run-direct"));
}

void Motor::run_direct_for_time(int duty_cycle, int time_ms, bool inverted, const char* stop_action){
  if(inverted) write_c_str(polarity_file, "inversed", sizeof("inversed"));
  else write_c_str(polarity_file, "normal", sizeof("normal"));
  write_int_fd(duty_cycle_file, duty_cycle);
  write_c_str(command_file, "run-direct", sizeof("run-direct"));

  std::thread t([this, time_ms, stop_action]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(time_ms));
    stop(stop_action);
  });
  t.detach();
}

std::string Motor::get_state(){
  char state[50];
  read_fd(state_file, state, sizeof(state));
  return state;
}

int Motor::get_position(){
  char pos[50];
  read_fd(position_file, pos, sizeof(pos));
  return atoi(pos);
}

void Motor::set_position(int new_position){
  write_int_fd(position_file, new_position);
}

void Motor::wait_for_stop(){
  while(std::string(get_state()).find("running") != std::string::npos) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
  }
}
