
#include<iostream>
#include<vector>
#include<math.h>
#include<stdexcept>
#include<chrono>
#include<thread>
#include<fstream>
#include<experimental/filesystem>
#include"position.h"
#include"motor.h"
#include"sensor.h"
#include"diferential_drive.h"

int main (){

	// find all connected motors
  std::vector<std::string> motor_directories;	
	std::string motor_path = "/sys/class/tacho-motor";
  for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::directory_iterator(motor_path)) {
    if (std::experimental::filesystem::is_directory(entry.path())) {
      motor_directories.push_back(entry.path().filename().string());
    }
  }

	Motor motor_a, motor_b, motor_c, motor_d;
	
	// setting defaul values and finding ports of motors
	for(std::size_t i = 0; i < motor_directories.size(); i++){
    // finding what ports is motor connected to
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
    // letting motor variable know where to find files for its port
		switch(port){
		case 0:	motor_a.set_directory(motor_directories.at(i));break;
		case 1:	motor_b.set_directory(motor_directories.at(i));break;
		case 2:	motor_c.set_directory(motor_directories.at(i));break;
		case 3:	motor_d.set_directory(motor_directories.at(i));break;
		}
	   
	}
  
  // find all connected sensors
  std::vector<std::string> sensor_directories;	
	std::string sensor_path = "/sys/class/lego-sensor";
  for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::directory_iterator(sensor_path)) {
    if (std::experimental::filesystem::is_directory(entry.path())) {
      sensor_directories.push_back(entry.path().filename().string());
    }
  }

	Sensor sensor_1, sensor_2, sensor_3, sensor_4;
	
	// setting defaul values and finding ports of sensors
	for(std::size_t i = 0; i < sensor_directories.size(); i++){
    // finding what ports sensor is connected to
		std::string address_file_path = "/sys/class/lego-sensor/" + sensor_directories.at(i) + "/address";
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
		port = address.at(12) - '1';
    // letting motor variable know where to find files for its port
		switch(port){
		case 0:	sensor_1.set_directory(sensor_directories.at(i));break;
		case 1:	sensor_2.set_directory(sensor_directories.at(i));break;
		case 2:	sensor_3.set_directory(sensor_directories.at(i));break;
		case 3:	sensor_4.set_directory(sensor_directories.at(i));break;
		}
 
	}


	// check if motors are connected
	if(!motor_c.is_connected()){
		std::cout << "motor C not connected" << std::endl;
		std::cerr << "motor C not connected" << std::endl;
		return 1;
	}
  if(!motor_b.is_connected()){
		std::cout << "motor B not connected" << std::endl;
		std::cerr << "motor B not connected" << std::endl;
		return 1;
	}

  // use motors
  Diferential_drive diferential_drive(motor_b, motor_c, 17.6, 9.5, false, false, Position(0, 0, 0));
  //diferential_drive.go_to_position(Position(0, 50, 0), 1, 100, false);
  //diferential_drive.go_to_position(Position(50, 50, 0), 1, 100, false);
  //diferential_drive.go_to_position(Position(50, 0, 0), 1, 100, false);
  //diferential_drive.go_to_position(Position(0, 0, 0), 1, 100, false);
  //diferential_drive.rotate_to_abs_angle(3.1415, 0.01745329 / 2, 100);
  //diferential_drive.rotate_to_abs_angle(0, 0.01745329 / 2, 100);
	diferential_drive.follow_path("/home/robot/libc++/src/test.path", 1, 0.01745329 /2, 100);
  //motor_b.set_position(500);
  //std::cout << motor_b.get_position() << std::endl;
  return 0;
}

