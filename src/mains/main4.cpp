#include <chrono>
#include <iostream>
#include <array>
#include <fstream>
#include <linux/input.h>
#include <thread>
#include <unistd.h>
#include "../position.h"
#include "../motor.h"
#include "../sensor.h"
#include "../diferential_drive.h"
#include "../robot.h"

int main (){
	std::ifstream button_input("/dev/input/event1", std::ios::binary);
	if(!button_input.is_open()){
		std::cout << "button input file could not be opened" << std::endl;
		std::cerr << "button input file could not be opened" << std::endl;
		return 1;
	}
	input_event button_event;

	std::array<Motor, 4> motors = Motor::find_motors();
	std::array<Sensor, 4> sensors = Sensor::find_sensors();	

	// check if motors are connected
	if(!motors.at(0).is_connected()){
		std::cout << "motor A not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "motor A not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}
	if(!motors.at(1).is_connected()){
		std::cout << "motor B not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "motor B not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}
	if(!motors.at(2).is_connected()){
		std::cout << "motor C not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "motor C not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}
	if(!motors.at(3).is_connected()){
		std::cout << "motor D not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "motor D not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}

	// check if sensors are connected
	if(!sensors.at(0).is_connected()){
		std::cout << "sensor 1 \"gyro\" not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "sensor 1 \"gyro\" not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}

	if(!sensors.at(1).is_connected()){
		std::cout << "sensor 2 \"color left\" not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "sensor 2 \"color left\" not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}

	if(!sensors.at(2).is_connected()){
		std::cout << "sensor 3 \"color right\" not connected, continue:	NO <-> YES" << std::endl;
		std::cerr << "sensor 3 \"color right\" not connected" << std::endl;
		while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ // reaad input 
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_RIGHT) break;
			if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_LEFT) return 1;
		}
	}

	// fun experiment 
	int run;
	std::fstream("runs.txt", std::ios::in) >> run;
	run++;
	std::fstream("runs.txt", std::ios::out | std::ios::trunc) << run;
	
	std::fstream configuration_file("robot_4.conf");
	float base_width, wheel_diameter, starting_position_x, starting_position_y, starting_position_angle;
	bool left_motor_inverted, right_motor_inverted;
	configuration_file >> base_width >> wheel_diameter >> starting_position_x >> starting_position_y >> starting_position_angle >> left_motor_inverted >> right_motor_inverted;
	Robot robot(motors.at(0), motors.at(3), motors.at(1), motors.at(2), sensors.at(0), sensors.at(1), sensors.at(2), base_width, wheel_diameter, left_motor_inverted, right_motor_inverted, Position(starting_position_x, starting_position_y, starting_position_angle));
	configuration_file.close();

	// Move motors to mechanical limit (max angle) for consistent alignment
	motors.at(0).run_direct(30, true);
	motors.at(3).run_direct(30, true);

	// wait for midle button
	while(true) if (button_input.read(reinterpret_cast<char*>(&button_event), sizeof(button_event))){ 
		if (button_event.type == EV_KEY && button_event.value == 1 && button_event.code == KEY_ENTER) break;
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
	
	motors.at(0).run_direct(0, false);
	motors.at(0).stop("brake");
	motors.at(0).set_position(0);
	motors.at(3).run_direct(0, false);
	motors.at(3).stop("brake");
	motors.at(3).set_position(0);

	// wait for finger to be far away from robot
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	robot.follow_program("program_4.prgm");

	return 0;
}

