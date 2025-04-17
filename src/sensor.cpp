#include"sensor.h"
#include<iostream>
#include<fstream>

void Sensor::open_files(){
  mode_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/mode"), std::fstream::out);
  value0_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value0"), std::fstream::in);
  value1_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value1"), std::fstream::in);
  value2_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value2"), std::fstream::in);
  value3_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value3"), std::fstream::in);
  value4_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value4"), std::fstream::in);
  value5_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value5"), std::fstream::in);
  value6_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value6"), std::fstream::in);
  value7_file.open(std::string("/sys/class/lego-sensor/") + directory + std::string("/value7"), std::fstream::in);
  if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
}

void Sensor::debug_output_file(std::string file_name){
  bool const cout_output = true;
  bool const cerr_output = true;

  if(cout_output)	std::cout << "couldnt open: " << file_name << " from: " << directory << std::endl;
  if(cerr_output)	std::cerr << "couldnt open: " << file_name << " from: " << directory << std::endl;
}

template<typename FileType> bool Sensor::check_file(FileType& file, std::string debug_name){
  if(!file.is_open()){
    debug_output_file(debug_name);
    return false;
  }
  else{
    return true;
  }
} 
bool Sensor::are_files_opened(){
  return  check_file(mode_file, "mode file") &&
          check_file(value0_file, "value0 file") &&
          check_file(value1_file, "value1 file") &&
          check_file(value2_file, "value2 file") &&
          check_file(value3_file, "value3 file") &&
          check_file(value4_file, "value4 file") &&
          check_file(value5_file, "value5 file") &&
          check_file(value6_file, "value6 file") &&
          check_file(value7_file, "value7 file");
}

int Sensor::read_value(std::ifstream& file){
  if(!are_files_opened()){
    open_files();
	    if(!are_files_opened()) throw std::runtime_error("cann't open files from: " + directory);
  }
  
  int value;
  file.clear(); // Clear any stream error flags.
  file.seekg(0, std::ios::beg); 
  file >> value; 
 
  return value;
}

Sensor::Sensor(){

}

Sensor::~Sensor(){

}

void Sensor::set_directory(std::string p_directory){
  directory = p_directory;
  connected = true;
  open_files();
}

bool Sensor::is_connected(){
  return connected;
}

int Sensor::get_value(int value_index){
  switch(value_index){
    case 0: return read_value(value0_file);
    case 1: return read_value(value1_file);
    case 2: return read_value(value2_file);
    case 3: return read_value(value3_file);
    case 4: return read_value(value4_file);
    case 5: return read_value(value5_file);
    case 6: return read_value(value6_file);
    case 7: return read_value(value7_file);
    default: throw std::invalid_argument("value file of given index doesn't exist");
  }
  return 0;
}

void Sensor::set_mode(std::string mode){
  mode_file << mode;
  mode_file.flush();
}

