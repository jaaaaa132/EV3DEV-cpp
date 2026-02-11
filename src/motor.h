#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <fcntl.h>
#include <unistd.h>

class Motor{
private:

protected:
  static constexpr int max_loop_count = 10;
	bool connected = false;
  std::string directory;
	int speed_file;
  int position_file;
  int position_sp_file;
  int acceleration_file;
  int deacceleration_file;
  int stop_action_file;
	int command_file;
  int duty_cycle_file;
  int polarity_file;
  int state_file;
  int time_file;
  
  void debug_output_file(const std::string file_name);
  template<typename FileType> bool check_file(FileType& file, const std::string debug_name = "not specified");
  void open_files();

public:
  Motor();
  ~Motor();
  Motor(Motor&&) = default;
  Motor& operator=(Motor&&) = default;
  static std::array<Motor, 4> find_motors();
	bool is_connected();
  void set_directory(std::string p_directory);
	void run(int speed, int acceleration);
  void run_to_abs_pos(int position, int speed, const char* stop_action = "hold");
  void run_to_rel_pos(int position, int speed, const char* stop_action = "hold");
  void run_for_time(int time_ms, int speed, const char* stop_action = "hold");
  void stop(const char* stop_action = "hold");
  void run_direct(int duty_cycle, bool inverted = false);
  void run_direct_for_time(int duty_cycle, int time_ms, bool inverted = false, const char* stop_action = "hold");
  std::string get_state();
  int get_position();
  void set_position(int new_position);
  void wait_for_stop();
};

