
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

  
  sensor_3.set_mode("COL-REFLECT");
  sensor_2.set_mode("US-DIST-CM");
  sensor_1.set_mode("IR_PROX");
  int const white_minimum = 25;
  while(sensor_4.get_value(0) == 0){
   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(5500));
  motor_b.run_direct(100);
  motor_c.run_direct(100, true);
  motor_d.run_direct(30);
  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  motor_d.run_direct(30, true);
  motor_b.run_direct(60);
  motor_c.run_direct(60);
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now(); 
  while(sensor_1.get_value(0) > 60  && sensor_2.get_value(0) > 200 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < 3000){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  while(true){
    motor_b.run_direct(100);
    motor_c.run_direct(100, true);
    try{
    if(sensor_3.get_value(0) > white_minimum){
      motor_b.run_direct(100, true);
      motor_c.run_direct(100);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      while(sensor_4.get_value(0) == 1){
      	motor_b.run_direct(100, true);
    	motor_c.run_direct(100);
      }
      motor_b.run_direct(60);
      motor_c.run_direct(60);
      std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now(); 
      while(sensor_1.get_value(0) > 60 && sensor_2.get_value(0) > 200 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() < 3800){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }
    }
    catch(std::runtime_error& error){
      std::cout << error.what() << std::endl;
    }
  }

	return 0;
}

