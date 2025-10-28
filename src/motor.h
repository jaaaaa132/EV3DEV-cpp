#pragma once

#include <iostream>
#include <fstream>
#include <array>

class Motor{
private:

protected:
	bool connected = false;
  std::string directory;
	std::ofstream speed_file;
  std::fstream  position_file;
  std::ofstream position_sp_file;
  std::ofstream acceleration_file;
  std::ofstream deacceleration_file;
  std::ofstream stop_action_file;
	std::ofstream command_file;
  std::ofstream duty_cycle_file;
  std::fstream polarity_file;
  std::ifstream state_file;
  std::ofstream time_file;
  
  void debug_output_file(std::string file_name);
  template<typename FileType> bool check_file(FileType& file, std::string debug_name = "not specified");
  void open_files();
  bool are_files_opened();

public:
  Motor();
  ~Motor();
  Motor(Motor&&) = default;
  Motor& operator=(Motor&&) = default;
  static std::array<Motor, 4> find_motors();
	bool is_connected();
  void set_directory(std::string p_directory);
	void run(int speed, int acceleration);
  void run_to_abs_pos(int position, int speed, std::string stop_action = "hold", int acceleration = 0, int deacceleration = 0);
  void run_to_rel_pos(int position, int speed, std::string stop_action = "hold", int acceleration = 0, int deacceleration = 0);
  void run_for_time(int time_ms, int speed, std::string stop_action = "hold", int acceleration = 0, int deacceleration = 0);
  void stop(std::string stop_action = "hold");
  void run_direct(int duty_cycle, bool inverted = false);
  void run_direct_for_time(int duty_cycle, int time_ms, bool inverted = false, std::string stop_action = "hold");
  std::string get_state();
  int get_position();
  void set_position(int new_position);
  void wait_for_stop();
};

