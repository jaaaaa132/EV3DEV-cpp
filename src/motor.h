#pragma once

#include<iostream>
#include<fstream>

class Motor{
private:

protected:
	bool connected = false;
  std::string directory;
	std::ofstream speed_file;
  std::fstream  position_file;
  std::ofstream position_sp_file;
  std::ofstream acceleration_file;
  std::ofstream stop_action_file;
	std::ofstream command_file;
  std::ofstream duty_cycle_file;
  std::fstream polarity_file;
  std::ifstream state_file;
  
  void debug_output_file(std::string file_name);
  template<typename FileType> bool check_file(FileType& file, std::string debug_name = "not specified");
  void open_files();
  bool are_files_opened();

public:
	bool is_connected();
  void set_directory(std::string p_directory);
	void run(int speed, int acceleration);
  void run_to_abs_pos(int position, int speed, std::string stop_action = "hold");
  void run_to_rel_pos(int position, int speed, std::string stop_action = "hold");
  void run_direct(int duty_cycle, bool inverted = false);
  std::string get_state();
  int get_position();
  void set_position(int new_position);
};

