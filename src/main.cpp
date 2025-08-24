#include <iostream>
#include <array>
#include "position.h"
#include "motor.h"
#include "sensor.h"
#include "diferential_drive.h"
#include "robot.h"

int main (){
	std::array<Motor, 4> motors = Motor::find_motors();
	std::array<Sensor, 4> sensors = Sensor::find_sensors();
	
	

	// check if motors are connected
	if(!motors.at(2).is_connected()){
		std::cout << "motor C not connected" << std::endl;
		std::cerr << "motor C not connected" << std::endl;
		return 1;
	}
	if(!motors.at(1).is_connected()){
		std::cout << "motor B not connected" << std::endl;
		std::cerr << "motor B not connected" << std::endl;
		return 1;
	}

	// use motors
	//Diferential_drive diferential_drive(motor_b, motor_c, 17.6, 9.5, false, false, Position(0, 0, 0));
	//Robot robot(motor_a, motor_d, motor_b, motor_c, 17.6, 9.5, false, false, Position(0, 0, 0));
	std::fstream configuration_file("/home/robot/libc++/src/test.conf");
	float base_width, wheel_diameter, starting_position_x, starting_position_y, starting_position_angle;
	bool left_motor_inverted, right_motor_inverted;
	configuration_file >> base_width >> wheel_diameter >> starting_position_x >> starting_position_y >> starting_position_angle >> left_motor_inverted >> right_motor_inverted;
	Robot robot(motors.at(0), motors.at(3), motors.at(1), motors.at(2), base_width, wheel_diameter, left_motor_inverted, right_motor_inverted, Position(starting_position_x, starting_position_y, starting_position_angle));
	configuration_file.close();
	robot.follow_program("/home/robot/libc++/src/test.prgm");

	return 0;
}

